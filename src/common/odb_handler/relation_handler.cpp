#include "relation_handler.h"
#include "logger.h"

namespace InstantSocial 
{
    bool RelationHandler::Insert(const std::string &user_id, const std::string &peer_id) 
    {
        try 
        {
            RelationEntity r1(user_id, peer_id);
            RelationEntity r2(peer_id, user_id);
            odb::transaction t(m_db->begin());
            m_db->persist(r1);
            m_db->persist(r2);
            t.commit();
            LOG_INFO("Inserted relation ({} -> {}) successfully", user_id, peer_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Insert relation ({} -> {}) failed: {}", user_id, peer_id, e.what());
            return false;
        }
        return true;
    }

    bool RelationHandler::Remove(const std::string &user_id, const std::string &peer_id) 
    {
        try 
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<RelationEntity> Query;
            m_db->erase_query<RelationEntity>(Query::user_id == user_id && Query::peer_id == peer_id);
            m_db->erase_query<RelationEntity>(Query::user_id == peer_id && Query::peer_id == user_id);
            t.commit();
            LOG_INFO("Removed relation ({} -> {}) successfully", user_id, peer_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Remove relation ({} -> {}) failed: {}", user_id, peer_id, e.what());
            return false;
        }
        return true;
    }

    bool RelationHandler::Exists(const std::string &user_id, const std::string &peer_id) 
    {
        typedef odb::query<RelationEntity> Query;
        typedef odb::result<RelationEntity> Result;
        bool found = false;
        try 
        {
            odb::transaction t(m_db->begin());
            Result r = m_db->query<RelationEntity>(Query::user_id == user_id && Query::peer_id == peer_id);
            // 在事务提交前检查结果
            found = !r.empty();
            t.commit();
            LOG_INFO("Checked existence of relation ({} -> {}): {}", user_id, peer_id, found);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Check existence of relation ({} -> {}) failed: {}", user_id, peer_id, e.what());
        }
        return found;
    }

    std::vector<std::string> RelationHandler::GetPeers(const std::string &user_id) 
    {
        std::vector<std::string> peers;
        try 
        {
            odb::transaction t(m_db->begin());
            typedef odb::query<RelationEntity> Query;
            typedef odb::result<RelationEntity> Result;
            Result r = m_db->query<RelationEntity>(Query::user_id == user_id);
            for (auto it = r.begin(); it != r.end(); ++it) 
            {
                peers.push_back(it->peer_id());
            }
            t.commit();
            LOG_INFO("Retrieved {} peers for user {}", peers.size(), user_id);
        } 
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get peers for user {} failed: {}", user_id, e.what());
        }
        return peers;
    }
}