#ifndef SERVICE_CHANNEL_H
#define SERVICE_CHANNEL_H

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <brpc/channel.h>
#include "logger.h"

namespace InstantSocial
{
    class ServiceChannel
    {
    public:
        using Ptr = std::shared_ptr<ServiceChannel>;
        using ChannelPtr = std::shared_ptr<brpc::Channel>;
        ServiceChannel(const std::string &service_name) : m_index(0), m_service_name(service_name) {}
        ~ServiceChannel() = default;

        bool Append(const std::string &host, int32_t timeout_ms = -1, int32_t connect_timeout_ms = -1, int max_retry=3, const std::string &protocol = "baidu_std");
        bool Remove(const std::string &host);
        ChannelPtr Choose();


    private:
        std::mutex m_mutex;
        int32_t m_index; // 当前轮转下标计数器
        std::string m_service_name; // 服务名称
        std::vector<ChannelPtr> m_channels; // 当前服务对应的信道集合
        std::unordered_map<std::string, ChannelPtr> m_hosts; // 主机地址与信道映射表
    };

    class ServiceManager
    {
    public:
        using Ptr = std::shared_ptr<ServiceManager>;
        ServiceManager() = default;
        ~ServiceManager() = default;
        ServiceChannel::ChannelPtr GetService(const std::string &service_name);
        void DeclareService(const std::string &service_name);
        void OnServiceOnline(const std::string &service_instance, const std::string &host);
        void OnServiceOffline(const std::string &service_instance, const std::string &host);
    
    private:
        std::string GetServiceName(const std::string &service_instance);

    private:
        std::mutex m_mutex;
        std::unordered_set<std::string> m_follow_services; // 关注的其他服务名称集合
        std::unordered_map<std::string, ServiceChannel::Ptr> m_services; // 服务名称与服务信道映射表
    };
}

#endif