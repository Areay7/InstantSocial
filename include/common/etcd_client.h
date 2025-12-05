#ifndef ETCD_CLIENT_H
#define ETCD_CLIENT_H

#include <etcd/Value.hpp>
#include <etcd/Client.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/KeepAlive.hpp>
#include <functional>
#include "logger.h"

namespace InstantSocial 
{
    class ServiceRegistry 
    {
    public:
        using Ptr = std::shared_ptr<ServiceRegistry>;
        ServiceRegistry(const std::string& host);
        ~ServiceRegistry();
        bool RegisterService(const std::string& key, const std::string& value, int ttl = 3);

    private:
        std::shared_ptr<etcd::Client> m_client;
        std::shared_ptr<etcd::KeepAlive> m_keep_alive;
        uint64_t m_lease_id;
    };

    class ServiceDiscovery 
    {
    public:
        using Ptr = std::shared_ptr<ServiceDiscovery>;
        using NotifyCallback = std::function<void(const std::string& key, const std::string& value)>;
        ServiceDiscovery(const std::string &host, const std::string &basedir, const NotifyCallback &put_cb, const NotifyCallback &del_cb);
        ~ServiceDiscovery();

    private:
        void CallBack(const etcd::Response &resp);

    private:
        std::shared_ptr<etcd::Client> m_client;
        std::shared_ptr<etcd::Watcher> m_watcher;
        NotifyCallback m_put_cb;
        NotifyCallback m_del_cb;
    };
}

#endif