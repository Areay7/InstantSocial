#include "chat_session_handler.h"
#include "logger.h"

namespace InstantSocial
{
    bool ChatSessionHandler::Insert(ChatSessionEntity &entity)
    {
        try
        {
            odb::transaction t(m_db->begin());
            m_db->persist(entity);
            t.commit();
            LOG_INFO("ChatSessionHandler::Insert success, session_id: {}", entity.chat_session_id());
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::Insert exception: {}", e.what());
            return false;
        }
        return true;
    }

    bool ChatSessionHandler::RemoveBySessionId(const std::string &session_id)
    {
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<ChatSessionEntity> Query;
            typedef odb::query<ChatSessionMemberEntity> MemberQuery;
            m_db->erase_query<ChatSessionEntity>(Query::chat_session_id == session_id);
            m_db->erase_query<ChatSessionMemberEntity>(MemberQuery::session_id == session_id);
            t.commit();
            LOG_INFO("ChatSessionHandler::RemoveBySessionId success, session_id: {}", session_id);
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::RemoveBySessionId exception: {}", e.what());
            return false;
        }
        return true;
    }

    bool ChatSessionHandler::RemoveByUserIdAndPeerId(const std::string &user_id, const std::string &peer_id)
    {
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<SingleChatSession> Query;
            auto res = m_db->query_one<SingleChatSession>
            (
                Query::csm1::user_id == user_id &&
                Query::csm2::user_id == user_id &&
                Query::css::chat_session_type == ChatSessionType::SINGLE
            );
            std::string chat_session_id = res->m_chat_session_id;
            typedef odb::query<ChatSessionEntity> ChatSessionQuery;
            typedef odb::query<ChatSessionMemberEntity> MemberQuery;
            m_db->erase_query<ChatSessionEntity>(ChatSessionQuery::chat_session_id == chat_session_id);
            m_db->erase_query<ChatSessionMemberEntity>(MemberQuery::session_id == chat_session_id);
            t.commit();
            LOG_INFO("ChatSessionHandler::RemoveByUserIdAndPeerId success, user_id: {}, peer_id: {}", user_id, peer_id);
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::RemoveByUserIdAndPeerId exception: {}", e.what());
            return false;
        }
        return true;
    }
    

    std::shared_ptr<ChatSessionEntity> ChatSessionHandler::GetBySessionId(const std::string &session_id)
    {
        std::shared_ptr<ChatSessionEntity> entity;
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<ChatSessionEntity> Query;
            entity.reset(m_db->query_one<ChatSessionEntity>(Query::chat_session_id == session_id));
            t.commit();
            LOG_INFO("ChatSessionHandler::GetBySessionId success, session_id: {}", session_id);
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::GetBySessionId exception: {}", e.what());
        }
        return entity;
    }

    std::vector<SingleChatSession> ChatSessionHandler::GetSingleChatSessionListByUserId(const std::string &user_id)
    {
        std::vector<SingleChatSession> session_list;
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<SingleChatSession> Query;
            typedef odb::result<SingleChatSession> Result;
            Result r(m_db->query<SingleChatSession>
            (
                Query::csm1::user_id == user_id &&
                Query::csm2::user_id != Query::csm1::user_id 
            ));
            for (const auto &session : r)
            {
                session_list.push_back(session);
            }
            t.commit();
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::GetSingleChatSessionListByUserId exception: {}", e.what());
        }
        return session_list;
    }

    std::vector<GroupChatSession> ChatSessionHandler::GetGroupChatSessionListByUserId(const std::string &user_id)
    {
        std::vector<GroupChatSession> session_list;
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<GroupChatSession> Query;
            typedef odb::result<GroupChatSession> Result;
            Result r(m_db->query<GroupChatSession>
            (
                Query::css::chat_session_type == ChatSessionType::GROUP &&
                Query::csm::user_id == user_id
            ));
            for (const auto &session : r)
            {
                session_list.push_back(session);
            }
            t.commit();
        }
        catch (const odb::exception &e)
        {
            LOG_ERROR("ChatSessionHandler::GetGroupChatSessionListByUserId exception: {}", e.what());
        }
        return session_list;
    }
} 