#include "chat_session_member_handler.h"
#include "logger.h"

namespace InstantSocial
{
    bool ChatSessionMemberHandler::Apeend(ChatSessionMemberEntity &entity)
    {
        try
        {
            odb::transaction t(m_db->begin());
            m_db->persist(entity);
            t.commit();
            LOG_INFO("ChatSessionMemberHandler::Apeend success: session_id={}, user_id={}", entity.session_id(), entity.user_id());
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("ChatSessionMemberHandler::Apeend failed: session_id={}, user_id={}, error={}", entity.session_id(), entity.user_id(), e.what());
            return false;
        }
        return true;
    }

    bool ChatSessionMemberHandler::Apeend(std::vector<ChatSessionMemberEntity> &entity_list)
    {
        try
        {
            odb::transaction t(m_db->begin());
            for (auto &entity : entity_list)
            {
                m_db->persist(entity);
            }
            t.commit();
            LOG_INFO("ChatSessionMemberHandler::Apeend batch success: count={}", entity_list.size());
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("ChatSessionMemberHandler::Apeend batch failed: count={}, error={}", entity_list.size(), e.what());
            return false;
        }
        return true;
    }

    bool ChatSessionMemberHandler::RemoveBySessionIdAndUserId(ChatSessionMemberEntity &entity)
    {
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<ChatSessionMemberEntity> Query;
            m_db->erase_query<ChatSessionMemberEntity>(
                Query::session_id == entity.session_id() && 
                Query::user_id == entity.user_id()
            );
            t.commit();
            LOG_INFO("ChatSessionMemberHandler::RemoveBySessionIdAndUserId success: session_id={}, user_id={}", entity.session_id(), entity.user_id());
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("ChatSessionMemberHandler::RemoveBySessionIdAndUserId failed: session_id={}, user_id={}, error={}", entity.session_id(), entity.user_id(), e.what());
            return false;
        }
        return true;
    }

    bool ChatSessionMemberHandler::RemoveAllBySessionId(const std::string &session_id)
    {
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<ChatSessionMemberEntity> Query;
            m_db->erase_query<ChatSessionMemberEntity>(Query::session_id == session_id);
            t.commit();
            LOG_INFO("ChatSessionMemberHandler::RemoveAllBySessionId success: session_id={}", session_id);
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("ChatSessionMemberHandler::RemoveAllBySessionId failed: session_id={}, error={}", session_id, e.what());
           
            return false;
        }
        return true;
    }

    std::vector<std::string> ChatSessionMemberHandler::GetMemberListBySessionId(const std::string &session_id)
    {
        std::vector<std::string> member_list;
        try
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<ChatSessionMemberEntity> Query;
            typedef odb::result<ChatSessionMemberEntity> Result;
            Result r(m_db->query<ChatSessionMemberEntity>(Query::session_id == session_id));
            for (const auto &entity : r)
            {
                member_list.push_back(entity.user_id());
            }
            t.commit();
            LOG_INFO("ChatSessionMemberHandler::GetMemberListBySessionId success: session_id={}, count={}", session_id, member_list.size());
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("ChatSessionMemberHandler::GetMemberListBySessionId failed: session_id={}, error={}", session_id, e.what());
        }
        return member_list;
    }
}