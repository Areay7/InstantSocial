#ifndef MESSAGE_ENTITY_H
#define MESSAGE_ENTITY_H

#include <string>
#include <cstddef>
#include <odb/core.hxx>
#include <odb/nullable.hxx>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace InstantSocial 
{
    enum class MessageType : uint8_t {
    TEXT = 0,
    IMAGE = 1,
    FILE = 2
    };

    #pragma db object table("message")
    class MessageEntity 
    {
    public:
        MessageEntity() = default;

        MessageEntity(const std::string &message_id,
                      const std::string &session_id,
                      const std::string &user_id,
                      MessageType message_type,
                      const boost::posix_time::ptime &timestamp)
            : m_message_id(message_id),
              m_session_id(session_id),
              m_user_id(user_id),
              m_message_type(message_type),
              m_create_time(timestamp) {}

        void message_id(const std::string &message_id) { m_message_id = message_id; }
        const std::string &message_id() const { return m_message_id; }

        void session_id(const std::string &session_id) { m_session_id = session_id; }
        const std::string &session_id() const { return m_session_id; }

        void user_id(const std::string &user_id) { m_user_id = user_id; }
        const std::string &user_id() const { return m_user_id; }

        void message_type(MessageType message_type) { m_message_type = message_type; }
        MessageType message_type() const { return m_message_type; }

        void create_time(const boost::posix_time::ptime &create_time) { m_create_time = create_time; }
        boost::posix_time::ptime create_time() const { return m_create_time; }

        void content(const std::string &content) { m_content = content; }
        std::string content() const { if (m_content) return *m_content; else return std::string(); }

        void file_id(const std::string &file_id) { m_file_id = file_id; }
        std::string file_id() const { if (m_file_id) return *m_file_id; else return std::string(); }

        void file_name(const std::string &file_name) { m_file_name = file_name; }
        std::string file_name() const { if (m_file_name) return *m_file_name; else return std::string(); }

        void file_path(const std::string &file_path) { m_file_path = file_path; }
        std::string file_path() const { if (m_file_path) return *m_file_path; else return std::string(); }

        void file_size(const unsigned int &file_size) { m_file_size = file_size; }
        unsigned int file_size() const { if (m_file_size) return *m_file_size ; else return 0; }

    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long m_id;
        #pragma db type("VARCHAR(64)") index unique
        std::string m_message_id;
        #pragma db type("VARCHAR(64)") index
        std::string m_session_id;               // 所属会话ID
        #pragma db type("VARCHAR(64)")
        std::string m_user_id;                  // 发送者用户ID
        #pragma db type("TINYINT")
        MessageType m_message_type;             // 消息类型  0-文本 1-图片 2-文件 3-语音 4-视频
        #pragma db type("TIMESTAMP")
        boost::posix_time::ptime m_create_time; // 消息创建时间

        #pragma db type("TEXT")
        odb::nullable<std::string> m_content;   // 消息内容（文本消息存储文本，其他类型存储描述信息）
        #pragma db type("VARCHAR(64)")
        odb::nullable<std::string> m_file_id;   // 文件消息存储文件ID
        #pragma db type("VARCHAR(128)")
        odb::nullable<std::string> m_file_name; // 文件名称
        #pragma db type("VARCHAR(128)")
        odb::nullable<std::string> m_file_path; // 文件存储路径
        #pragma db type("INTEGER")
        odb::nullable<unsigned int> m_file_size; // 文件大小（字节）
    };
}

        

#endif // MESSAGE_ENTITY_H