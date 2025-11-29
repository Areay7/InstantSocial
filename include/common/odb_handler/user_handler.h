#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include "odb_client.h"
#include "user_entity.h"
#include "user_entity-odb.hxx"

namespace InstantSocial
{
    class UserHandler
    {
        public:
            using Ptr = std::shared_ptr<UserHandler>;
            UserHandler(const std::shared_ptr<odb::core::database> &db) : m_db(db) {}
            bool Insert(const std::shared_ptr<UserEntity> &user);
            bool Update(const std::shared_ptr<UserEntity> &user);
            std::shared_ptr<UserEntity> GetByUserID(const std::string &user_id);
            std::shared_ptr<UserEntity> GetByPhone(const std::string &phone);
            std::shared_ptr<UserEntity> GetByEmail(const std::string &email);
            std::shared_ptr<UserEntity> GetByNickname(const std::string &nickname);
            std::vector<UserEntity> GetByMultiUsers(const std::vector<std::string> &user_id_list);
            

        private:
            std::shared_ptr<odb::core::database> m_db;
    };
}

#endif // USER_HANDLER_H