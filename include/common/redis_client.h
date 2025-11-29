#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <sw/redis++/redis++.h>
#include <iostream>

namespace InstantSocial
{
    class RedisFactory
    {
    public:
        RedisFactory() = default;
        static std::shared_ptr<sw::redis::Redis> Create(const std::string &host, int port, bool keep_alive,  const std::string &password = "", int db = 0);
    };

    class Session
    {
    public:
        using Ptr = std::shared_ptr<Session>;
        Session(const std::shared_ptr<sw::redis::Redis> &redis_client) : m_redis_client(redis_client) {}
        void Append(const std::string &ssid, const std::string &uid);
        void Remove(const std::string &ssid);
        sw::redis::OptionalString GetUid(const std::string &ssid);

    private:
        std::shared_ptr<sw::redis::Redis> m_redis_client;
    };

    class Codes
    {
    public:
        using Ptr = std::shared_ptr<Codes>;
        Codes(const std::shared_ptr<sw::redis::Redis> &redis_client) : m_redis_client(redis_client) {}
        void Append(const std::string &cid, const std::string &code, const std::chrono::milliseconds &t = std::chrono::milliseconds(300000));
        void Remove(const std::string &cid);
        sw::redis::OptionalString GetCode(const std::string &cid);

    private:
        std::shared_ptr<sw::redis::Redis> m_redis_client;
    };
} // namespace InstantSocial

#endif // REDIS_CLIENT_H