#ifndef ODB_CLIENT_H
#define ODB_CLIENT_H

#include <string>
#include <memory>
#include <cstdlib>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include "logger.h"

namespace InstantSocial
{

    enum class DatabaseType : int {
    MySQL,      // MySQL数据库
    PostgreSQL, // PostgreSQL数据库  
    SQLite,     // SQLite数据库（文件数据库）
    Oracle,     // Oracle数据库
    SQLServer   // SQL Server数据库
    };

    class ODBFactory
    {
        public:
            static std::shared_ptr<odb::core::database> Create(   DatabaseType db_type,
                                                            const std::string &host,
                                                            const std::string &user,
                                                            const std::string &password,
                                                            const std::string &dbName,
                                                            unsigned int port = 3306,
                                                            int conn_pool_count = 1,
                                                            const std::string &cset = "utf8mb4"
                                                        );
            
    };
}

#endif // ODB_CLIENT_H