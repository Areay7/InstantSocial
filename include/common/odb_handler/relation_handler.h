#ifndef RELATION_HANDLER_H
#define RELATION_HANDLER_H

#include "odb_client.h"
#include "relation_entity.h"
#include "relation_entity-odb.hxx"

namespace InstantSocial 
{
    class RelationHandler 
    {
    public:
        using Ptr = std::shared_ptr<RelationHandler>;
        RelationHandler(const std::shared_ptr<odb::core::database> &db) : m_db(db) {}
        ~RelationHandler() = default;

        bool Insert(const std::string &user_id, const std::string &peer_id);
        bool Remove(const std::string &user_id, const std::string &peer_id);
        bool Exists(const std::string &user_id, const std::string &peer_id);
        std::vector<std::string> GetPeers(const std::string &user_id);

    private:
        std::shared_ptr<odb::core::database> m_db;
    };
}

#endif // RELATION_HANDLER_H