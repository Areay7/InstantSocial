#include "service_channel.h"
#include "logger.h"

namespace InstantSocial
{
    bool ServiceChannel::Append(const std::string &host, int32_t timeout_ms, int32_t connect_timeout_ms, int max_retry, const std::string &protocol)
    {
        auto channel = std::make_shared<brpc::Channel>();
        brpc::ChannelOptions options;
        options.protocol = protocol;
        options.timeout_ms = timeout_ms;
        options.connect_timeout_ms = connect_timeout_ms;
        options.max_retry = max_retry;
        if (!channel->Init(host.c_str(), &options))
        {
            LOG_ERROR("Failed to initialize channel to host: {}", host);
            return false;
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_channels.push_back(channel);
            m_hosts[host] = channel;
        }
        return true;
    }

    bool ServiceChannel::Remove(const std::string &host)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_hosts.find(host);
        if (it != m_hosts.end())
        {
            LOG_WARN("Removed channel to host: {} , no find service : {}", host, m_service_name);
            return false;
        }
        else
        {
            for(auto vit = m_channels.begin(); vit != m_channels.end(); ++vit)
            {
                if(*vit == it->second)
                {
                    m_channels.erase(vit);
                    break;
                }
            }
            m_hosts.erase(it);
        }
        return true;
    }

    ServiceChannel::ChannelPtr ServiceChannel::Choose()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_channels.empty())
        {
            LOG_ERROR("No channel to choose from for service: {}", m_service_name);
            return ChannelPtr();
        }
        int32_t index = m_index++ % m_channels.size();
        return m_channels[index];
    }

    ServiceChannel::ChannelPtr ServiceManager::GetService(const std::string &service_name)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto it = m_services.find(service_name);
        if (it != m_services.end())
        {
            return it->second->Choose();
        }
        else
        {
            return nullptr;
        }
    }

    void ServiceManager::DeclareService(const std::string &service_name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_follow_services.insert(service_name);
    }

    void ServiceManager::OnServiceOnline(const std::string &service_instance, const std::string &host)
    {
        std::string service_name = GetServiceName(service_instance);
        ServiceChannel::Ptr service;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto fit = m_follow_services.find(service_name);
            if (fit == m_follow_services.end())
            {
                LOG_DEBUG("Service {} Online , but is not followed, skip", service_name);
                return;
            }
            auto sit = m_services.find(service_name);
            if (sit != m_services.end())
            {
                service = sit->second;
            }
            else
            {
                service = std::make_shared<ServiceChannel>(service_name);
                m_services[service_name] = service;
            }
        }
        if(service == nullptr)
        {
            LOG_ERROR("Failed to insert service: {}", service_name);
            return;
        }
        service->Append(host);
        LOG_DEBUG("Service {} online, host: {}", service_name, host);
    }

    void ServiceManager::OnServiceOffline(const std::string &service_instance, const std::string &host)
    {
        std::string service_name = GetServiceName(service_instance);
        ServiceChannel::Ptr service;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto fit = m_follow_services.find(service_name);
            if(fit == m_follow_services.end())
            {
                LOG_DEBUG("Service {} Offline , but is not followed, skip", service_name);
                return;
            }
            auto sit = m_services.find(service_name);
            if (sit == m_services.end())
            {
                LOG_WARN("Service {} Offline , but is not found, skip", service_name);
                return;
            }
            service = sit->second;
        }
        service->Remove(host);
        LOG_DEBUG("Service {} offline, host: {}", service_name, host);
    }

    std::string ServiceManager::GetServiceName(const std::string &service_instance)
    {
        auto pos = service_instance.find_last_of('/');
        if(pos == std::string::npos)
        {
            return service_instance;
        }
        return service_instance.substr(0, pos);
    }
}