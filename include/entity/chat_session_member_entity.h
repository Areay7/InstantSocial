#ifndef CHAT_SESSION_MEMBER_ENTITY_H
#define CHAT_SESSION_MEMBER_ENTITY_H

#include <string>
#include <cstddef> 
#include <odb/core.hxx>

namespace InstantSocial
{
    #pragma db object table("chat_session_member")
    class ChatSessionMemberEntity
    {
        public:
        ChatSessionMemberEntity() = default;
        ChatSessionMemberEntity(const std::string &session_id, const std::string &user_id) :
            m_session_id(session_id), m_user_id(user_id) {}
        ~ChatSessionMemberEntity() = default;

        void session_id(const std::string &session_id) { m_session_id = session_id; }
        const std::string &session_id() const { return m_session_id; }

        void user_id(const std::string &user_id) { m_user_id = user_id; }
        const std::string &user_id() const { return m_user_id; }

        private:
        friend class odb::access;
        #pragma db id auto
        unsigned long m_id;
        #pragma db type("VARCHAR(64)") index
        std::string m_session_id;
        #pragma db type("VARCHAR(64)") index
        std::string m_user_id;
    };
}

#endif