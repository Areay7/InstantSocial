#include "etcd_client.h"
#include "logger.h"

namespace InstantSocial 
{
    ServiceRegistry::ServiceRegistry(const std::string& host) 
    {
        m_client = std::make_shared<etcd::Client>(host);
        m_keep_alive = m_client->leasekeepalive(3).get();
        m_lease_id = m_keep_alive->Lease();
    }

    ServiceRegistry::~ServiceRegistry() 
    {
        if (m_keep_alive) 
        {
            m_keep_alive->Cancel();
        }
    }

    bool ServiceRegistry::RegisterService(const std::string& key, const std::string& value, int ttl) 
    {
        if(ttl != 3) 
        {
            m_keep_alive->Cancel();
            m_keep_alive = m_client->leasekeepalive(ttl).get();
            m_lease_id = m_keep_alive->Lease();
        }
        auto resp = m_client->put(key, value, m_lease_id).get();
        if (!resp.is_ok())
        {
            LOG_ERROR("Failed to register service: {} -> {}, error: {}", key, value, resp.error_message());
            return false;
        }
        LOG_INFO("Service registered: {} -> {}", key, value);
        return true;
    }

    ServiceDiscovery::ServiceDiscovery(const std::string &host, 
                                       const std::string &basedir, 
                                       const NotifyCallback &put_cb, 
                                       const NotifyCallback &del_cb)
        : m_put_cb(put_cb), m_del_cb(del_cb) 
    {
        m_client = std::make_shared<etcd::Client>(host);
        auto resp = m_client->ls(basedir).get();
        if (!resp.is_ok())
        {
            LOG_ERROR("Failed to list services under {}: {}", basedir, resp.error_message());
            return;
        }

        // 初始化时加载当前目录下已有的服务
        for (const auto &kv : resp.values())
        {
            m_put_cb(kv.key(), kv.as_string());
        }

        // 使用 etcd::Watcher 监听目录变化（递归）
        m_watcher = std::make_shared<etcd::Watcher>(
            *m_client, 
            basedir, 
            std::bind(&ServiceDiscovery::CallBack, this, std::placeholders::_1),
            true);
    }

    ServiceDiscovery::~ServiceDiscovery()
    {
        if (m_watcher)
        {
            m_watcher->Cancel();
        }
    }

    void ServiceDiscovery::CallBack(const etcd::Response &resp)
    {
        if (!resp.is_ok())
        {
            LOG_ERROR("Failed to watch services: {}", resp.error_message());
            return;
        }
        for (auto const &ev : resp.events())
        {
            // 事件类型定义在 etcd::Event::EventType 中
            auto type = ev.event_type();
            if (type == etcd::Event::EventType::PUT)
            {
                const auto &val = ev.kv();
                m_put_cb(val.key(), val.as_string());
                LOG_DEBUG("Service added: {} -> {}", val.key(), val.as_string());
            }
            else if (type == etcd::Event::EventType::DELETE_)
            {
                const auto &val = ev.kv();
                const auto &prev_kv = ev.prev_kv();
                m_del_cb(prev_kv.key(), prev_kv.as_string());
                LOG_DEBUG("Service deleted: {}", prev_kv.key());
            }
        }
    }
}