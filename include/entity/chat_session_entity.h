#ifndef CHAT_SESSION_ENTITY_H
#define CHAT_SESSION_ENTITY_H

#include <string>
#include <cstddef>
#include <odb/core.hxx>
#include <odb/nullable.hxx>
#include "chat_session_member_entity.h"

namespace InstantSocial 
{
    enum class ChatSessionType : uint8_t
    {
        SINGLE = 1,
        GROUP = 2
    };

    #pragma db object table("chat_session")
    class ChatSessionEntity
    {
    public:
        ChatSessionEntity() = default;
        ChatSessionEntity(const std::string &session_id,
            const std::string &session_name, const ChatSessionType &session_type) :
            m_chat_session_id(session_id),
            m_chat_session_name(session_name),
            m_chat_session_type(session_type){}

        void chat_session_id(const std::string &session_id){ m_chat_session_id = session_id; }
        std::string chat_session_id() const{ return m_chat_session_id; }

        void chat_session_name(const std::string &session_name){ m_chat_session_name = session_name; }
        std::string chat_session_name() const{ return m_chat_session_name; }

        void chat_session_type(const ChatSessionType &session_type){ m_chat_session_type = session_type; }
        ChatSessionType chat_session_type() const{ return m_chat_session_type; }
        
    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long m_id;
        #pragma db type("VARCHAR(64)") index unique
        std::string m_chat_session_id;
        #pragma db type("VARCHAR(64)")
        std::string m_chat_session_name;
        #pragma db type("TINYINT")
        ChatSessionType m_chat_session_type;
    };

    #pragma db view object(ChatSessionEntity = css)\
                    object(ChatSessionMemberEntity = csm1 : css::m_chat_session_id == csm1::m_session_id)\
                    object(ChatSessionMemberEntity = csm2 : css::m_chat_session_id == csm2::m_session_id)\
                    query((?))
    
    struct SingleChatSession
    {
        #pragma db column(css::m_chat_session_id)
        std::string m_chat_session_id;
        #pragma db column(csm2::m_user_id)
        std::string m_peer_id;
    };

    #pragma db view object(ChatSessionEntity = css)\
                    object(ChatSessionMemberEntity = csm : css::m_chat_session_id == csm::m_session_id)\
                    query((?))

    struct GroupChatSession
    {
        #pragma db column(css::m_chat_session_id)
        std::string m_chat_session_id;
        #pragma db column(css::m_chat_session_name)
        std::string m_chat_session_name;
    };
}

#endif