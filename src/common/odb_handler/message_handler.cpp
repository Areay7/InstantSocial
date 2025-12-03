#include "message_handler.h"
#include "logger.h"
#include <algorithm>
#include <sstream>

namespace InstantSocial 
{
    bool MessageHandler::Insert(MessageEntity &message) 
    {
        try 
        {
            odb::transaction t(m_db->begin());
            m_db->persist(message);
            t.commit();
            LOG_INFO("Inserted message {} successfully", message.message_id());
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Insert message {} failed: {}", message.message_id(), e.what());
            return false;
        }
        return true;
    }

    bool MessageHandler::Remove(const std::string &session_id) 
    {
        try 
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<MessageEntity> Query;
            m_db->erase_query<MessageEntity>(Query::session_id == session_id);
            t.commit();
            LOG_INFO("Removed session {} successfully", session_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Remove session {} failed: {}", session_id, e.what());
            return false;
        }
        return true;
    }

    std::vector<MessageEntity> MessageHandler::GetRecent(const std::string &session_id, int32_t count) 
    {
        std::vector<MessageEntity> res;
        try 
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<MessageEntity> Query;
            typedef odb::result<MessageEntity> Result;

            // 构造查询条件：session_id='xx' ORDER BY create_time DESC LIMIT count
            std::stringstream cond;
            cond << "session_id='" << session_id << "' ";
            cond << "ORDER BY create_time DESC LIMIT " << count;

            Result r(m_db->query<MessageEntity>(Query(cond.str())));
            for (Result::iterator it(r.begin()); it != r.end(); ++it) 
            {
                res.push_back(*it);
            }

            // 逆序，使最早的消息在前
            std::reverse(res.begin(), res.end());

            t.commit();
            LOG_INFO("Retrieved {} recent messages for session {}", res.size(), session_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get recent messages for session {} failed: {}", session_id, e.what());
        }
        return res;
    }

    std::vector<MessageEntity> MessageHandler::GetByTimeRange(const std::string &session_id,
                                                              const boost::posix_time::ptime &start_time,
                                                              const boost::posix_time::ptime &end_time) 
    {
        std::vector<MessageEntity> res;
        try 
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<MessageEntity> Query;
            typedef odb::result<MessageEntity> Result;
            Result r = m_db->query<MessageEntity>(
                Query::session_id == session_id &&
                Query::create_time >= start_time && 
                Query::create_time <= end_time,
                "ORDER BY create_time ASC"
            );
            for (auto it = r.begin(); it != r.end(); ++it) 
            {
                res.push_back(*it);
            }
            t.commit();
            LOG_INFO("Retrieved {} messages for session {} in time range", res.size(), session_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get messages for session {} in time range failed: {}", session_id, e.what());
        }
        return res;
    }

}
