#ifndef RELATION_ENTITY_H
#define RELATION_ENTITY_H

#include <string>
#include <cstddef>
#include <odb/core.hxx>
#include <odb/nullable.hxx>

namespace InstantSocial 
{
    #pragma db object table("relation")
    class RelationEntity 
    {
    public:
        RelationEntity() = default;

        RelationEntity(const std::string &user_id, const std::string &peer_id)
            : m_user_id(user_id), m_peer_id(peer_id) {}

        void user_id(const std::string &user_id) { m_user_id = user_id; }
        const std::string &user_id() const { return m_user_id; }

        void peer_id(const std::string &peer_id) { m_peer_id = peer_id; }
        const std::string &peer_id() const { return m_peer_id; }

    private:
        friend class odb::access;

        #pragma db id auto
        unsigned long m_id;

        #pragma db type("VARCHAR(64)") index
        std::string m_user_id;

        #pragma db type("VARCHAR(64)")
        std::string m_peer_id;
    };
}

#endif // RELATION_ENTITY_H