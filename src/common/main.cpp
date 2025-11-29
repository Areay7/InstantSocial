#include "logger.h"
#include "rabbitmq.h"
#include "redis_client.h"
#include "odb_client.h"
#include "user_handler.h"
#include <memory>
#include <vector>
#include <string>

namespace InstantSocial
{
    // 辅助函数：检查字符串是否为空
    std::string GetStringOrDefault(const std::string& str, const std::string& default_val = "") 
    {
        return str.empty() ? default_val : str;
    }

    // 测试数据库操作的辅助函数
    class DatabaseTester 
    {
    public:
        static void RunTests(std::shared_ptr<odb::database> db) 
        {
            UserHandler handler(db);
            
            LOG_INFO("=== 开始数据库测试 ===");
            
            // 1. 测试插入用户
            TestInsertUsers(handler);
            
            // 2. 测试查询用户
            TestQueryUsers(handler);
            
            // 3. 测试更新用户
            TestUpdateUser(handler);
            
            // 4. 测试批量查询
            TestBatchQuery(handler);
            
            // 5. 测试删除用户
            TestDeleteUser(handler);
            
            LOG_INFO("=== 数据库测试完成 ===");
        }
        
    private:
        static void TestInsertUsers(UserHandler& handler) 
        {
            LOG_INFO("--- 测试插入用户 ---");
            
            // 创建测试用户
            auto user1 = std::make_shared<UserEntity>("user001", "张三", "password123");
            user1->phone("13800138001");
            user1->email("zhangsan@example.com");
            user1->description("这是一个测试用户");
            
            auto user2 = std::make_shared<UserEntity>("user002", "李四", "password456");
            user2->phone("13800138002");
            user2->email("lisi@example.com");
            
            auto user3 = std::make_shared<UserEntity>("user003", "王五", "password789");
            user3->phone("13800138003");
            
            // 插入用户
            if (handler.Insert(user1)) 
            {
                LOG_INFO("插入用户1成功: {}", user1->user_id());
            } 
            else 
            {
                LOG_ERROR("插入用户1失败");
            }
            
            if (handler.Insert(user2)) 
            {
                LOG_INFO("插入用户2成功: {}", user2->user_id());
            } 
            else 
            {
                LOG_ERROR("插入用户2失败");
            }
            
            if (handler.Insert(user3)) 
            {
                LOG_INFO("插入用户3成功: {}", user3->user_id());
            } 
            else 
            {
                LOG_ERROR("插入用户3失败");
            }
        }
        
        static void TestQueryUsers(UserHandler& handler) 
        {
            LOG_INFO("--- 测试查询用户 ---");
            
            // 1. 通过用户ID查询
            auto user1 = handler.GetByUserID("user001");
            if (user1) 
            {
                LOG_INFO("通过ID查询成功: {} - {} - {}", 
                        user1->user_id(), 
                        user1->nickname(), 
                        GetStringOrDefault(user1->phone(), "无"));
            } 
            else 
            {
                LOG_ERROR("通过ID查询失败: user001");
            }
            
            // 2. 通过手机号查询
            auto user2 = handler.GetByPhone("13800138002");
            if (user2) 
            {
                LOG_INFO("通过手机号查询成功: {} - {}", 
                        user2->user_id(), 
                        user2->nickname());
            } 
            else 
            {
                LOG_ERROR("通过手机号查询失败: 13800138002");
            }
            
            // 3. 通过邮箱查询
            auto user3 = handler.GetByEmail("lisi@example.com");
            if (user3) 
            {
                LOG_INFO("通过邮箱查询成功: {} - {}", 
                        user3->user_id(), 
                        user3->nickname());
            } 
            else 
            {
                LOG_ERROR("通过邮箱查询失败: lisi@example.com");
            }
            
            // 4. 查询不存在的用户
            auto notExist = handler.GetByUserID("not_exist");
            if (!notExist) 
            {
                LOG_INFO("查询不存在的用户返回nullptr，符合预期");
            }
        }
        
        static void TestUpdateUser(UserHandler& handler) 
        {
            LOG_INFO("--- 测试更新用户 ---");
            
            // 先查询用户
            auto user = handler.GetByUserID("user001");
            if (user) 
            {
                LOG_INFO("更新前用户信息: {} - {} - {}", 
                        user->user_id(), 
                        user->nickname(), 
                        GetStringOrDefault(user->description(), "无描述"));
                
                // 修改用户信息
                user->nickname("张三(已更新)");
                user->description("这是更新后的描述信息");
                user->avatar_id("avatar_001");
                
                if (handler.Update(user)) 
                {
                    LOG_INFO("用户更新成功");
                    
                    // 验证更新结果
                    auto updatedUser = handler.GetByUserID("user001");
                    if (updatedUser) 
                    {
                        LOG_INFO("更新后用户信息: {} - {} - {}", 
                                updatedUser->user_id(), 
                                updatedUser->nickname(), 
                                GetStringOrDefault(updatedUser->description(), "无描述"));
                    }
                } 
                else 
                {
                    LOG_ERROR("用户更新失败");
                }
            } 
            else 
            {
                LOG_ERROR("要更新的用户不存在");
            }
        }
        
        static void TestBatchQuery(UserHandler& handler) 
        {
            LOG_INFO("--- 测试批量查询 ---");
            
            std::vector<std::string> user_ids = {"user001", "user002", "user003", "not_exist"};
            
            auto users = handler.GetByMultiUsers(user_ids);
            LOG_INFO("批量查询结果: 找到 {} 个用户 (查询了 {} 个ID)", 
                    users.size(), user_ids.size());
            
            for (const auto& user : users) 
            {
                LOG_INFO("批量查询结果: {} - {} - {}", 
                        user.user_id(), 
                        user.nickname(), 
                        GetStringOrDefault(user.phone(), "无手机号"));
            }
        }
        
        static void TestDeleteUser(UserHandler& handler) 
        {
            LOG_INFO("--- 测试删除用户 ---");
            LOG_INFO("删除功能需要根据具体业务逻辑实现");
        }
    };
}


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
            InstantSocial::DatabaseTester::RunTests(odb_db);
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