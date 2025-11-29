#include "redis_client.h"
#include "logger.h"

namespace InstantSocial
{
    std::shared_ptr<sw::redis::Redis> RedisFactory::Create(const std::string &host, int port, bool keep_alive, const std::string &password, int db)
    {
        try {
            sw::redis::ConnectionOptions connection_options;
            connection_options.host = host;
            connection_options.port = port;
            connection_options.db = db;
            connection_options.keep_alive = keep_alive;
            connection_options.socket_timeout = std::chrono::milliseconds(3000); 
            connection_options.connect_timeout = std::chrono::milliseconds(3000);
            
            if (!password.empty()) {
                connection_options.password = password;
            }

            auto redis_client = std::make_shared<sw::redis::Redis>(connection_options);
            
            redis_client->ping();
            
            return redis_client;
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to create Redis client: {} ", e.what());
            return nullptr;
        }
    }

    void Session::Append(const std::string &ssid, const std::string &uid)
    {
        if (!m_redis_client) return;
        m_redis_client->set(ssid, uid);
    }

    void Session::Remove(const std::string &ssid)
    {
        if (!m_redis_client) return;
        m_redis_client->del(ssid);
    }

    sw::redis::OptionalString Session::GetUid(const std::string &ssid)
    {
        if (!m_redis_client) return sw::redis::OptionalString();
        return m_redis_client->get(ssid);
    }

    void Codes::Append(const std::string &cid, const std::string &code, const std::chrono::milliseconds &t)
    {
        if (!m_redis_client) return;
        m_redis_client->set(cid, code , t);
    }

    void Codes::Remove(const std::string &cid)
    {
        if (!m_redis_client) return;
        m_redis_client->del(cid);
    }

    sw::redis::OptionalString Codes::GetCode(const std::string &cid)
    {
        if (!m_redis_client) return sw::redis::OptionalString();
        return m_redis_client->get(cid);
    }
} // namespace InstantSocial