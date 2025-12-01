#ifndef FRIEND_APPLY_HANDLER_H
#define FRIEND_APPLY_HANDLER_H

#include "odb_client.h"
#include "friend_apply_entity.h"
#include "friend_apply_entity-odb.hxx"

namespace InstantSocial 
{
    class FriendApplyHandler 
    {
    public:
        using Ptr = std::shared_ptr<FriendApplyHandler>;
        FriendApplyHandler(const std::shared_ptr<odb::core::database> &db) : m_db(db) {}
        ~FriendApplyHandler() = default;

        bool Insert(FriendApplyEntity &event);
        bool Remove(const std::string &user_id, const std::string &peer_id);
        bool Exists(const std::string &user_id, const std::string &peer_id);
        std::vector<std::string> GetApplyUsers(const std::string &user_id);

    private:
        std::shared_ptr<odb::core::database> m_db;
    };
}

#endif