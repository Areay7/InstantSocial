#ifndef RABBITMQ_H
#define RABBITMQ_H

#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#include <iostream>
#include <functional>
#include "logger.h"

namespace InstantSocial
{
    class RabbitMQHandler
    {
        public:
            using MessageCallback = std::function<void(const char*, size_t)>;

            RabbitMQHandler(const std::string &user, const std::string &password, const std::string &host, int32_t port, bool use_ssl = false);
            ~RabbitMQHandler();

            void DeclareComponents(const std::string &exchange, const std::string &queue, const std::string &routingKey = "routing_key", AMQP::ExchangeType type = AMQP::direct);
            void PublishMessage(const std::string &exchange, const std::string &msg, const std::string &routingKey = "routing_key");
            void ConsumeMessage(const std::string &queue, MessageCallback &cb);

        private:
            static void WatchdogCallback(struct ev_loop *loop, ev_async *w, int32_t revents);
        
        private:
            struct ev_async m_async_watcher;
            struct ev_loop *m_event_loop;

            std::unique_ptr<AMQP::TcpConnection> m_connection;
            std::unique_ptr<AMQP::TcpChannel> m_channel;
            std::unique_ptr<AMQP::LibEvHandler> m_handler;
            std::thread m_loop_thread;
    };
}

#endif