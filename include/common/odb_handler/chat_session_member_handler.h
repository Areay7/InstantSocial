#ifndef CHAT_SESSION_MEMBER_HANDLER_H
#define CHAT_SESSION_MEMBER_HANDLER_H

#include "odb_client.h"
#include "chat_session_member_entity.h"
#include "chat_session_member_entity-odb.hxx"

namespace InstantSocial
{
    class ChatSessionMemberHandler
    {
        public:
        using Ptr = std::shared_ptr<ChatSessionMemberHandler>;
        ChatSessionMemberHandler(const std::shared_ptr<odb::database> &db) : m_db(db) {}

        bool Apeend(ChatSessionMemberEntity &entity);
        bool Apeend(std::vector<ChatSessionMemberEntity> &entity_list);
        bool RemoveBySessionIdAndUserId(ChatSessionMemberEntity &entity);
        bool RemoveAllBySessionId(const std::string &session_id);
        std::vector<std::string> GetMemberListBySessionId(const std::string &session_id);

        private:
        std::shared_ptr<odb::database> m_db;

    };
}

#endif