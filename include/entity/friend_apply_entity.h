#ifndef FRIEND_APPLY_ENTITY_H
#define FRIEND_APPLY_ENTITY_H

#include <string>
#include <cstddef> 
#include <odb/core.hxx>

namespace InstantSocial 
{
    #pragma db object table("friend_apply")
    class FriendApplyEntity
    {
    public:
        FriendApplyEntity() = default;

        FriendApplyEntity(const std::string &event_id,
            const std::string &user_id, const std::string &peer_id) :
            m_event_id(event_id), m_user_id(user_id), m_peer_id(peer_id) {}

        void event_id(const std::string &event_id) { m_event_id = event_id; }
        const std::string &event_id() const { return m_event_id; }
        
        void user_id(const std::string &user_id) { m_user_id = user_id; }
        const std::string &user_id() const { return m_user_id; }

        void peer_id(const std::string &peer_id) { m_peer_id = peer_id; }
        const std::string &peer_id() const { return m_peer_id; }

    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long m_id;
        #pragma db type("VARCHAR(64)") index unique
        std::string m_event_id;
        #pragma db type("VARCHAR(64)") index
        std::string m_user_id;
        #pragma db type("VARCHAR(64)") index
        std::string m_peer_id;
    };
}

#endif // FRIEND_APPLY_ENTITY_H