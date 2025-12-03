#include "odb_handler_test.h"
#include "logger.h"
#include "user_handler.h"
#include "message_handler.h"
#include "chat_session_handler.h"
#include "relation_handler.h"
#include "friend_apply_handler.h"
#include "chat_session_member_handler.h"
#include "user_entity.h"
#include "message_entity.h"
#include "chat_session_entity.h"
#include "relation_entity.h"
#include "friend_apply_entity.h"
#include "chat_session_member_entity.h"
#include <boost/date_time/posix_time/posix_time.hpp>
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

    // 测试 UserHandler
    void TestUserHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 UserHandler ===");
        UserHandler handler(db);
        
        // 1. 测试插入用户
        LOG_INFO("--- 测试插入用户 ---");
        auto user1 = std::make_shared<UserEntity>("test_user001", "测试用户1", "password123");
        user1->phone("13800138001");
        user1->email("test1@example.com");
        user1->description("这是测试用户1");
        
        if (handler.Insert(user1)) 
        {
            LOG_INFO("插入用户成功: {}", user1->user_id());
        } 
        else 
        {
            LOG_ERROR("插入用户失败");
        }
        
        // 2. 测试查询用户
        LOG_INFO("--- 测试查询用户 ---");
        auto queried_user = handler.GetByUserID("test_user001");
        if (queried_user) 
        {
            LOG_INFO("通过ID查询成功: {} - {} - {}", 
                    queried_user->user_id(), 
                    queried_user->nickname(), 
                    GetStringOrDefault(queried_user->phone(), "无"));
        }
        
        auto user_by_phone = handler.GetByPhone("13800138001");
        if (user_by_phone) 
        {
            LOG_INFO("通过手机号查询成功: {} - {}", 
                    user_by_phone->user_id(), 
                    user_by_phone->nickname());
        }
        
        auto user_by_email = handler.GetByEmail("test1@example.com");
        if (user_by_email) 
        {
            LOG_INFO("通过邮箱查询成功: {} - {}", 
                    user_by_email->user_id(), 
                    user_by_email->nickname());
        }
        
        auto user_by_nickname = handler.GetByNickname("测试用户1");
        if (user_by_nickname) 
        {
            LOG_INFO("通过昵称查询成功: {} - {}", 
                    user_by_nickname->user_id(), 
                    user_by_nickname->nickname());
        }
        
        // 3. 测试更新用户
        LOG_INFO("--- 测试更新用户 ---");
        if (queried_user) 
        {
            queried_user->nickname("测试用户1(已更新)");
            queried_user->description("更新后的描述");
            if (handler.Update(queried_user)) 
            {
                LOG_INFO("用户更新成功");
            }
        }
        
        // 4. 测试批量查询
        LOG_INFO("--- 测试批量查询 ---");
        std::vector<std::string> user_ids = {"test_user001", "test_user002"};
        auto users = handler.GetByMultiUsers(user_ids);
        LOG_INFO("批量查询结果: 找到 {} 个用户", users.size());
    }

    // 测试 MessageHandler
    void TestMessageHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 MessageHandler ===");
        MessageHandler handler(db);
        
        // 1. 测试插入消息
        LOG_INFO("--- 测试插入消息 ---");
        std::string session_id = "test_session_001";
        std::string message_id1 = "msg_001";
        std::string message_id2 = "msg_002";
        
        auto now = boost::posix_time::second_clock::local_time();
        
        MessageEntity msg1(message_id1, session_id, "test_user001", 
                          MessageType::TEXT, now);
        msg1.content("这是一条测试文本消息");
        
        if (handler.Insert(msg1)) 
        {
            LOG_INFO("插入消息1成功: {}", message_id1);
        } 
        else 
        {
            LOG_ERROR("插入消息1失败");
        }
        
        auto later = now + boost::posix_time::seconds(10);
        MessageEntity msg2(message_id2, session_id, "test_user001", 
                          MessageType::TEXT, later);
        msg2.content("这是第二条测试消息");
        
        if (handler.Insert(msg2)) 
        {
            LOG_INFO("插入消息2成功: {}", message_id2);
        }
        
        // 2. 测试获取最近消息
        LOG_INFO("--- 测试获取最近消息 ---");
        auto recent_msgs = handler.GetRecent(session_id, 10);
        LOG_INFO("获取最近消息: 找到 {} 条消息", recent_msgs.size());
        for (const auto& msg : recent_msgs) 
        {
            LOG_INFO("消息: {} - {} - {}", 
                    msg.message_id(), 
                    msg.content(), 
                    boost::posix_time::to_simple_string(msg.create_time()));
        }
        
        // 3. 测试按时间范围查询
        LOG_INFO("--- 测试按时间范围查询 ---");
        auto start_time = now - boost::posix_time::minutes(1);
        auto end_time = now + boost::posix_time::minutes(1);
        auto range_msgs = handler.GetByTimeRange(session_id, start_time, end_time);
        LOG_INFO("按时间范围查询: 找到 {} 条消息", range_msgs.size());
        
        // 4. 测试删除消息
        LOG_INFO("--- 测试删除消息 ---");
        if (handler.Remove(message_id1)) 
        {
            LOG_INFO("删除消息成功: {}", message_id1);
        }
    }

    // 测试 ChatSessionHandler
    void TestChatSessionHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 ChatSessionHandler ===");
        ChatSessionHandler handler(db);
        
        // 1. 测试插入会话
        LOG_INFO("--- 测试插入会话 ---");
        std::string session_id1 = "test_session_001";
        std::string session_id2 = "test_group_001";
        
        ChatSessionEntity session1(session_id1, "单聊会话", ChatSessionType::SINGLE);
        if (handler.Insert(session1)) 
        {
            LOG_INFO("插入单聊会话成功: {}", session_id1);
        }
        
        ChatSessionEntity session2(session_id2, "群聊会话", ChatSessionType::GROUP);
        if (handler.Insert(session2)) 
        {
            LOG_INFO("插入群聊会话成功: {}", session_id2);
        }
        
        // 2. 测试查询会话
        LOG_INFO("--- 测试查询会话 ---");
        auto queried_session = handler.GetBySessionId(session_id1);
        if (queried_session) 
        {
            LOG_INFO("查询会话成功: {} - {} - {}", 
                    queried_session->chat_session_id(),
                    queried_session->chat_session_name(),
                    static_cast<int>(queried_session->chat_session_type()));
        }
        
        // 3. 测试获取单聊会话列表
        LOG_INFO("--- 测试获取单聊会话列表 ---");
        auto single_sessions = handler.GetSingleChatSessionListByUserId("test_user001");
        LOG_INFO("单聊会话列表: 找到 {} 个会话", single_sessions.size());
        
        // 4. 测试获取群聊会话列表
        LOG_INFO("--- 测试获取群聊会话列表 ---");
        auto group_sessions = handler.GetGroupChatSessionListByUserId("test_user001");
        LOG_INFO("群聊会话列表: 找到 {} 个会话", group_sessions.size());
        
        // 5. 测试删除会话
        LOG_INFO("--- 测试删除会话 ---");
        if (handler.RemoveBySessionId(session_id1)) 
        {
            LOG_INFO("删除会话成功: {}", session_id1);
        }
    }

    // 测试 RelationHandler
    void TestRelationHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 RelationHandler ===");
        RelationHandler handler(db);
        
        // 1. 测试插入关系
        LOG_INFO("--- 测试插入关系 ---");
        std::string user_id = "test_user001";
        std::string peer_id1 = "test_user002";
        std::string peer_id2 = "test_user003";
        
        if (handler.Insert(user_id, peer_id1)) 
        {
            LOG_INFO("插入关系成功: {} <-> {}", user_id, peer_id1);
        }
        
        if (handler.Insert(user_id, peer_id2)) 
        {
            LOG_INFO("插入关系成功: {} <-> {}", user_id, peer_id2);
        }
        
        // 2. 测试检查关系是否存在
        LOG_INFO("--- 测试检查关系是否存在 ---");
        if (handler.Exists(user_id, peer_id1)) 
        {
            LOG_INFO("关系存在: {} <-> {}", user_id, peer_id1);
        }
        
        if (!handler.Exists(user_id, "not_exist_user")) 
        {
            LOG_INFO("不存在的关系检查正确");
        }
        
        // 3. 测试获取所有关系
        LOG_INFO("--- 测试获取所有关系 ---");
        auto peers = handler.GetPeers(user_id);
        LOG_INFO("用户 {} 的关系列表: 找到 {} 个", user_id, peers.size());
        for (const auto& peer : peers) 
        {
            LOG_INFO("关系: {} <-> {}", user_id, peer);
        }
        
        // 4. 测试删除关系
        LOG_INFO("--- 测试删除关系 ---");
        if (handler.Remove(user_id, peer_id1)) 
        {
            LOG_INFO("删除关系成功: {} <-> {}", user_id, peer_id1);
        }
    }

    // 测试 FriendApplyHandler
    void TestFriendApplyHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 FriendApplyHandler ===");
        FriendApplyHandler handler(db);
        
        // 1. 测试插入好友申请
        LOG_INFO("--- 测试插入好友申请 ---");
        std::string event_id1 = "apply_001";
        std::string event_id2 = "apply_002";
        std::string user_id = "test_user001";
        std::string peer_id1 = "test_user002";
        std::string peer_id2 = "test_user003";
        
        FriendApplyEntity apply1(event_id1, user_id, peer_id1);
        if (handler.Insert(apply1)) 
        {
            LOG_INFO("插入好友申请成功: {} - {} -> {}", 
                    event_id1, user_id, peer_id1);
        }
        
        FriendApplyEntity apply2(event_id2, user_id, peer_id2);
        if (handler.Insert(apply2)) 
        {
            LOG_INFO("插入好友申请成功: {} - {} -> {}", 
                    event_id2, user_id, peer_id2);
        }
        
        // 2. 测试检查申请是否存在
        LOG_INFO("--- 测试检查申请是否存在 ---");
        if (handler.Exists(user_id, peer_id1)) 
        {
            LOG_INFO("好友申请存在: {} -> {}", user_id, peer_id1);
        }
        
        // 3. 测试获取申请列表
        LOG_INFO("--- 测试获取申请列表 ---");
        auto apply_users = handler.GetApplyUsers(user_id);
        LOG_INFO("用户 {} 的申请列表: 找到 {} 个", user_id, apply_users.size());
        for (const auto& peer : apply_users) 
        {
            LOG_INFO("申请: {} -> {}", user_id, peer);
        }
        
        // 4. 测试删除申请
        LOG_INFO("--- 测试删除申请 ---");
        if (handler.Remove(user_id, peer_id1)) 
        {
            LOG_INFO("删除好友申请成功: {} -> {}", user_id, peer_id1);
        }
    }

    // 测试 ChatSessionMemberHandler
    void TestChatSessionMemberHandler(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("=== 测试 ChatSessionMemberHandler ===");
        ChatSessionMemberHandler handler(db);
        
        // 1. 测试插入单个成员
        LOG_INFO("--- 测试插入单个成员 ---");
        std::string session_id = "test_session_001";
        ChatSessionMemberEntity member1(session_id, "test_user001");
        if (handler.Apeend(member1)) 
        {
            LOG_INFO("插入成员成功: {} - {}", session_id, member1.user_id());
        }
        
        // 2. 测试批量插入成员
        LOG_INFO("--- 测试批量插入成员 ---");
        std::vector<ChatSessionMemberEntity> members;
        members.push_back(ChatSessionMemberEntity(session_id, "test_user002"));
        members.push_back(ChatSessionMemberEntity(session_id, "test_user003"));
        if (handler.Apeend(members)) 
        {
            LOG_INFO("批量插入成员成功: {} 个成员", members.size());
        }
        
        // 3. 测试获取成员列表
        LOG_INFO("--- 测试获取成员列表 ---");
        auto member_list = handler.GetMemberListBySessionId(session_id);
        LOG_INFO("会话 {} 的成员列表: 找到 {} 个成员", session_id, member_list.size());
        for (const auto& user_id : member_list) 
        {
            LOG_INFO("成员: {}", user_id);
        }
        
        // 4. 测试删除单个成员
        LOG_INFO("--- 测试删除单个成员 ---");
        ChatSessionMemberEntity member_to_remove(session_id, "test_user001");
        if (handler.RemoveBySessionIdAndUserId(member_to_remove)) 
        {
            LOG_INFO("删除成员成功: {} - {}", session_id, member_to_remove.user_id());
        }
        
        // 5. 测试删除所有成员
        LOG_INFO("--- 测试删除所有成员 ---");
        if (handler.RemoveAllBySessionId(session_id)) 
        {
            LOG_INFO("删除所有成员成功: {}", session_id);
        }
    }

    // 测试所有 ODB Handler
    void TestAllODBHandlers(std::shared_ptr<odb::database> db)
    {
        LOG_INFO("==========================================");
        LOG_INFO("开始测试所有 ODB Handler");
        LOG_INFO("==========================================");
        
        try 
        {
            TestUserHandler(db);
            TestMessageHandler(db);
            TestChatSessionHandler(db);
            TestRelationHandler(db);
            TestFriendApplyHandler(db);
            TestChatSessionMemberHandler(db);
            
            LOG_INFO("==========================================");
            LOG_INFO("所有 ODB Handler 测试完成");
            LOG_INFO("==========================================");
        }
        catch (const std::exception& e) 
        {
            LOG_ERROR("测试过程中发生异常: {}", e.what());
        }
    }
}

