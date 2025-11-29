#include "rabbitmq.h"
#include "logger.h"

namespace InstantSocial
{
    RabbitMQHandler::RabbitMQHandler(const std::string &user, const std::string &password, const std::string &host, int32_t port, bool use_ssl)
    {
        m_event_loop = ev_default_loop(0);
        m_handler = std::make_unique<AMQP::LibEvHandler>(m_event_loop);
        std::string protocol = use_ssl ? "amqps://" : "amqp://";
        std::string url = protocol + user + ":" + password + "@" + host + ":" + std::to_string(port);
        AMQP::Address address(url);
        m_connection = std::make_unique<AMQP::TcpConnection>(m_handler.get(), address);
        m_channel = std::make_unique<AMQP::TcpChannel>(m_connection.get());

        m_loop_thread = std::thread([this]() {
            ev_run(m_event_loop, 0);
        });
    }

    void RabbitMQHandler::DeclareComponents(const std::string &exchange, const std::string &queue, const std::string &routingKey, AMQP::ExchangeType type)
    {
        m_channel->declareExchange(exchange, type).onSuccess([exchange]() 
        {
            LOG_INFO("Exchange declared: {}", exchange);
        }).onError([exchange](const char* message) 
        {
            LOG_ERROR("Failed to declare exchange {}: {}", exchange, message);
        });

        m_channel->declareQueue(queue).onSuccess([this, exchange, queue, routingKey]() 
        {
            LOG_INFO("Queue declared: {}", queue);
            m_channel->bindQueue(exchange, queue, routingKey).onSuccess([exchange, queue, routingKey]() 
            {
                LOG_INFO("{} Queue {} bound to routing key {}",  exchange, queue, routingKey);
            }).onError([exchange, queue, routingKey](const char* message) 
            {
                LOG_ERROR("{} Failed to bind queue {} to routing key {}: {}", exchange, queue, routingKey, message);
            });
        }).onError([queue](const char* message) 
        {
            LOG_ERROR("Failed to declare queue {}: {}", queue, message);
        });
    }

    void RabbitMQHandler::PublishMessage(const std::string &exchange, const std::string &msg, const std::string &routingKey)
    {
        bool success = m_channel->publish(exchange, routingKey, msg);
        
        if (success) 
        {
            LOG_INFO("Message published to exchange {} with routing key {}", exchange, routingKey);
        } else 
        {
            LOG_ERROR("Failed to publish message to exchange {} with routing key {}", exchange, routingKey);
        }
    }

    void RabbitMQHandler::ConsumeMessage(const std::string &queue, MessageCallback &cb)
    {
        LOG_INFO("Starting to consume messages from queue {}", queue);
        m_channel->consume(queue, "consume-tag").onReceived([this, cb](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) 
        {
            cb(message.body(), message.bodySize());
            m_channel->ack(deliveryTag);
        }).onSuccess([queue]() 
        {
            LOG_INFO("Started consuming messages from queue {}", queue);
        }).onError([queue](const char* message) 
        {
            LOG_ERROR("Failed to start consuming messages from queue {}: {}", queue, message);
        });
    }

    void RabbitMQHandler::WatchdogCallback(struct ev_loop *loop, ev_async *w, int32_t revents)
    {
        ev_break(loop, EVBREAK_ALL);
    }

    RabbitMQHandler::~RabbitMQHandler()
    {
        // 1. ensure all AMQP objects destroyed before stopping loop
        m_channel.reset();
        m_connection.reset();

        // 2. stop the event loop
        ev_async_init(&m_async_watcher, WatchdogCallback);
        ev_async_start(m_event_loop, &m_async_watcher);
        ev_async_send(m_event_loop, &m_async_watcher);

        // 3. wait for thread
        if (m_loop_thread.joinable()) {
            m_loop_thread.join();
        }

        m_event_loop = nullptr;
    }
}