#ifndef CHAT_SESSION_HANDLER_H
#define CHAT_SESSION_HANDLER_H

#include "odb_client.h"
#include "chat_session_entity.h"
#include "chat_session_entity-odb.hxx"
#include "chat_session_member_handler.h"

namespace InstantSocial
{
    class ChatSessionHandler
    {
    public:
        using Ptr = std::shared_ptr<ChatSessionHandler>;
        ChatSessionHandler(const std::shared_ptr<odb::database> &db) : m_db(db) {}

        bool Insert(ChatSessionEntity &entity);
        bool RemoveBySessionId(const std::string &session_id);
        bool RemoveByUserIdAndPeerId(const std::string &user_id, const std::string &peer_id);
        std::shared_ptr<ChatSessionEntity> GetBySessionId(const std::string &session_id);
        std::vector<SingleChatSession> GetSingleChatSessionListByUserId(const std::string &user_id);
        std::vector<GroupChatSession> GetGroupChatSessionListByUserId(const std::string &user_id);
        
    private:
        std::shared_ptr<odb::database> m_db;
    };
}

#endif