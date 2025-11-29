#include "user_handler.h"
#include "logger.h"

namespace InstantSocial 
{
    bool UserHandler::Insert(const std::shared_ptr<UserEntity> &user)
    {
        try 
        {
            odb::core::transaction t(m_db->begin());
            m_db->persist(*user);
            t.commit();
            return true;
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Insert user {} failed: {}", user->user_id(), e.what());
            return false;
        }
    }

    bool UserHandler::Update(const std::shared_ptr<UserEntity> &user)
    {
        try 
        {
            odb::core::transaction t(m_db->begin());
            m_db->update(*user);
            t.commit();
            return true;
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Update user {} failed: {}", user->user_id(), e.what());
            return false;
        }
    }

    std::shared_ptr<UserEntity> UserHandler::GetByUserID(const std::string &user_id)
    {
        std::shared_ptr<UserEntity> res;
        try 
        {
            odb::core::transaction t(m_db->begin());
            typedef odb::query<UserEntity> Query;
            res.reset(m_db->query_one<UserEntity>(Query(odb::query<UserEntity>::user_id == user_id)));
            t.commit();
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get user by user_id {} failed: {}", user_id, e.what());
        }
        return res;
    }
    
    std::shared_ptr<UserEntity> UserHandler::GetByPhone(const std::string &phone)
    {
        std::shared_ptr<UserEntity> res;
        try 
        {
            odb::core::transaction t(m_db->begin());
            typedef odb::query<UserEntity> Query;
            res.reset(m_db->query_one<UserEntity>(Query(odb::query<UserEntity>::phone == phone)));
            t.commit();
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get user by phone {} failed: {}", phone, e.what());
        }
        return res;
    }

    std::shared_ptr<UserEntity> UserHandler::GetByEmail(const std::string &email)
    {
        std::shared_ptr<UserEntity> res;
        try 
        {
            odb::core::transaction t(m_db->begin());
            typedef odb::query<UserEntity> Query;
            res.reset(m_db->query_one<UserEntity>(Query(odb::query<UserEntity>::email == email)));
            t.commit();
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get user by email {} failed: {}", email, e.what());
        }
        return res;
    }

    std::vector<UserEntity> UserHandler::GetByMultiUsers(const std::vector<std::string> &user_id_list)
    {
        std::vector<UserEntity> res;
        try 
        {
            odb::core::transaction t(m_db->begin());
            
            if (!user_id_list.empty()) 
            {
                std::string condition = "user_id IN (";
                for (size_t i = 0; i < user_id_list.size(); ++i) 
                {
                    condition += "'" + user_id_list[i] + "'";
                    if (i < user_id_list.size() - 1) 
                    {
                        condition += ",";
                    }
                }
                condition += ")";
                
                typedef odb::query<UserEntity> Query;
                odb::result<UserEntity> result = m_db->query<UserEntity>(Query(condition));
                
                for (odb::result<UserEntity>::iterator it = result.begin(); it != result.end(); ++it) 
                {
                    res.push_back(*it);
                }
            }
            
            t.commit();
            LOG_INFO("Get {} users by user_id list", res.size());
        }
        catch (const std::exception &e) 
        {
            LOG_ERROR("Get users by user_id list failed: {}", e.what());
        }
        return res;
    }
}