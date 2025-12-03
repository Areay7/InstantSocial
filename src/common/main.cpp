#include "logger.h"
#include "rabbitmq.h"
#include "redis_client.h"
#include "odb_client.h"
#include "odb_handler_test.h"
#include <memory>
#include <vector>
#include <string>



// 分离测试函数，确保每个测试独立运行
void TestRabbitMQ()
{
    LOG_INFO("=== 测试RabbitMQ ===");
    try 
    {
        // 在独立作用域中创建RabbitMQHandler
        std::unique_ptr<InstantSocial::RabbitMQHandler> rabbitmq;
        
        try 
        {
            rabbitmq = std::make_unique<InstantSocial::RabbitMQHandler>(
                "root", "123456", "192.168.113.205", 5672);
            
            rabbitmq->DeclareComponents("my_exchange", "my_queue", "my_routing_key");
            rabbitmq->PublishMessage("my_exchange", "Hello, RabbitMQ!", "my_routing_key");
            
            // 等待消息发送
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            LOG_INFO("RabbitMQ消息发送完成");
        }
        catch (const std::exception& e) 
        {
            LOG_ERROR("RabbitMQ操作失败: {}", e.what());
        }
        
        // 显式销毁rabbitmq对象
        rabbitmq.reset();
        
        // 确保资源释放
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        LOG_INFO("RabbitMQ测试完成");
    } 
    catch (const std::exception& e) 
    {
        LOG_ERROR("RabbitMQ测试失败: {}", e.what());
    }
}

void TestRedis()
{
    LOG_INFO("=== 测试Redis ===");
    try 
    {
        auto redis_client = InstantSocial::RedisFactory::Create("192.168.113.205", 6379, true, "", 0);
        if (redis_client) 
        {
            redis_client->set("sample_key", "sample_value");
            auto value = redis_client->get("sample_key");
            if (value) 
            {
                LOG_INFO("Redis测试成功: {}", *value);
            }
            
            redis_client->set("test_counter", "0");
            redis_client->incr("test_counter");
            auto counter = redis_client->get("test_counter");
            if (counter) 
            {
                LOG_INFO("Redis计数器测试: {}", *counter);
            }
        } 
        else 
        {
            LOG_ERROR("Redis连接创建失败");
        }
    } 
    catch (const std::exception& e) 
    {
        LOG_ERROR("Redis测试失败: {}", e.what());
    }
}

void TestDatabase()
{
    LOG_INFO("=== 测试数据库 ===");
    try 
    {
        auto odb_db = InstantSocial::ODBFactory::Create(
            InstantSocial::DatabaseType::MySQL, 
            "192.168.113.205", 
            "root", 
            "123456", 
            "test", 3306, 2, "utf8mb4"
        );
        
        if (odb_db) 
        {
            LOG_INFO("数据库连接成功");
            // 测试所有 ODB Handler
            InstantSocial::TestAllODBHandlers(odb_db);
        } 
        else 
        {
            LOG_ERROR("数据库连接失败");
        }
    } 
    catch (const std::exception& e) 
    {
        LOG_ERROR("数据库测试失败: {}", e.what());
    }
}

void TestIntegration()
{
    LOG_INFO("=== 综合测试 ===");
    LOG_INFO("综合测试完成");
}

int main()
{
    try 
    {
        // 1. 初始化日志
        InstantSocial::init_logger(false, "logs.txt", 0);
        LOG_INFO("Logger initialized successfully.");
        
        // 2. 测试RabbitMQ - 使用独立函数避免阻塞
        TestRabbitMQ();
        
        // 3. 测试Redis
        TestRedis();
        
        // 4. 测试数据库
        TestDatabase();
        
        // 5. 综合测试
        TestIntegration();
        
        LOG_INFO("=== 所有测试完成 ===");
        return 0;
    } 
    catch (const std::exception& e) 
    {
        LOG_ERROR("程序异常: {}", e.what());
        return -1;
    }
}