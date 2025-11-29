#ifndef USER_ENTITY_H
#define USER_ENTITY_H

#include <string>
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace InstantSocial
{
#pragma db object table("users")
class UserEntity
{
    public:
        UserEntity() = default;
        // 用户名构造函数  -- 用户ID, 昵称, 密码
        UserEntity(const std::string &user_id, const std::string &nickname, const std::string &password) :
            m_user_id(user_id), m_nickname(nickname), m_password(password) {}
        // 电话构造函数  -- 用户ID, 手机, 随机昵称
        UserEntity(const std::string &user_id, const std::string &phone) :
            m_user_id(user_id), m_phone(phone) {}
        // 邮箱构造函数  -- 用户ID, 邮箱, 随机昵称
        UserEntity(const std::string &user_id, const std::string &email, bool is_email) :
            m_user_id(user_id), m_email(email) {}

        void user_id(const std::string &val) { m_user_id = val; }
        std::string user_id() const { return m_user_id; }

        void nickname(const std::string &val) { m_nickname = val; }
        std::string nickname() const { if (m_nickname) return *m_nickname; else return std::string(); }

        void description(const std::string &val) { m_description = val; }
        std::string description() const { if (m_description) return *m_description; else return std::string(); }

        void password(const std::string &val) { m_password = val; }
        std::string password() const { if (m_password) return *m_password; else return std::string(); }

        void phone(const std::string &val) { m_phone = val; }
        std::string phone() const { if (m_phone) return *m_phone; else return std::string(); }

        void email(const std::string &val) { m_email = val; }
        std::string email() const { if (m_email) return *m_email; else return std::string(); }

        void avatar_id(const std::string &val) { m_avatar_id = val; }
        std::string avatar_id() const { if (m_avatar_id) return *m_avatar_id; else return std::string(); }

    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long m_id;
        #pragma db type("VARCHAR(64)") index unique
        std::string m_user_id;
        #pragma db type("VARCHAR(64)") index unique
        odb::nullable<std::string> m_nickname;
        #pragma db type("VARCHAR(64)")
        odb::nullable<std::string> m_password;
        #pragma db type("VARCHAR(64)") index unique
        odb::nullable<std::string> m_phone;
        #pragma db type("VARCHAR(64)") index unique
        odb::nullable<std::string> m_email; 
        #pragma db type("VARCHAR(64)")
        odb::nullable<std::string> m_avatar_id;
        #pragma db type("VARCHAR(64)")
        odb::nullable<std::string> m_description;



};
}

#endif // USER_ENTITY_H