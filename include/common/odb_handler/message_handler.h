#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "odb_client.h"
#include "message_entity.h"
#include "message_entity-odb.hxx"

namespace InstantSocial 
{
    class MessageHandler 
    {
    public:
        using Ptr = std::shared_ptr<MessageHandler>;
        MessageHandler(const std::shared_ptr<odb::core::database> &db) : m_db(db) {}
        ~MessageHandler() = default;

        bool Insert(MessageEntity &message);
        bool Remove(const std::string &message_id);
        std::vector<MessageEntity> GetRecent(const std::string &session_id, int32_t count);
        std::vector<MessageEntity> GetByTimeRange(const std::string &session_id,
                                                 const boost::posix_time::ptime &start_time,
                                                 const boost::posix_time::ptime &end_time);
    
    private:
        std::shared_ptr<odb::core::database> m_db;
    };
}

#endif // MESSAGE_HANDLER_H