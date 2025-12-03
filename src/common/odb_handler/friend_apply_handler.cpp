#include "friend_apply_handler.h"
#include "logger.h"

namespace InstantSocial {
    bool FriendApplyHandler::Insert(FriendApplyEntity &event)
    {
        try {
            odb::transaction t(m_db->begin());
            m_db->persist(event);
            t.commit();
            LOG_INFO("Insert FriendApply {} - {} success",  event.user_id(), event.peer_id());
        } catch (const std::exception &e) {
            LOG_ERROR("Insert FriendApply {} - {} failed: {}",  event.user_id(), event.peer_id(), e.what());
            return false;
        }
        return true;
    }

    bool FriendApplyHandler::Remove(const std::string &user_id, const std::string &peer_id)
    {
        try {
            odb::transaction t(m_db->begin());
            typedef odb::query<FriendApplyEntity> Query;
            m_db->erase_query<FriendApplyEntity>(Query::user_id == user_id && Query::peer_id == peer_id);
            t.commit();
            LOG_INFO("Remove FriendApply {} - {} success",  user_id, peer_id);
        } catch (const std::exception &e) {
            LOG_ERROR("Remove FriendApply {} - {} failed: {}",  user_id, peer_id, e.what());
            return false;
        }
        return true;
    }

    bool FriendApplyHandler::Exists(const std::string &user_id, const std::string &peer_id)
    {
        bool exists = false;
        try {
            odb::transaction t(m_db->begin());
            typedef odb::query<FriendApplyEntity> Query;
            typedef odb::result<FriendApplyEntity> Result;
            Result r(m_db->query<FriendApplyEntity>(Query::user_id == user_id && Query::peer_id == peer_id));
            exists = !r.empty();
            t.commit();
            LOG_INFO("Exists FriendApply {} - {} : {}",  user_id, peer_id, exists);
        } catch (const std::exception &e) {
            LOG_ERROR("Exists FriendApply {} - {} failed: {}",  user_id, peer_id, e.what());
            return false;
        }
        return exists;
    }

    std::vector<std::string> FriendApplyHandler::GetApplyUsers(const std::string &user_id)
    {
        std::vector<std::string> apply_users;
        try {
            odb::transaction t(m_db->begin());
            typedef odb::query<FriendApplyEntity> Query;
            typedef odb::result<FriendApplyEntity> Result;
            // 查询 user_id 发起的申请，返回被申请的用户列表
            Result r(m_db->query<FriendApplyEntity>(Query::user_id == user_id));
            for (const auto &entity : r) {
                apply_users.push_back(entity.peer_id());
            }
            t.commit();
            LOG_INFO("GetApplyUsers for {} success, count: {}",  user_id, apply_users.size());
        } catch (const std::exception &e) {
            LOG_ERROR("GetApplyUsers for {} failed: {}",  user_id, e.what());
        }
        return apply_users;
    }

}