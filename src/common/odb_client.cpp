#include "odb_client.h"

namespace InstantSocial
{
    std::shared_ptr<odb::core::database> ODBFactory::Create(DatabaseType db_type,
                                                        const std::string& host,
                                                        const std::string& user,
                                                        const std::string& password,
                                                        const std::string& dbName,
                                                        unsigned int port,
                                                        int conn_pool_count,
                                                        const std::string &cset)
    {
        switch (db_type)
        {
            case DatabaseType::MySQL:
            {
                try {
                    std::unique_ptr<odb::mysql::connection_pool_factory> cpf(
                        new odb::mysql::connection_pool_factory(conn_pool_count, 0));

                    auto db = std::make_shared<odb::mysql::database>(user,
                                                                password,
                                                                dbName,
                                                                host,
                                                                port,
                                                                "",
                                                                cset,
                                                                0,
                                                                std::move(cpf));
                    return db;
                } catch (const std::exception& e) {
                    LOG_ERROR("Create connection_pool_factory fail : {}", e.what());
                    return nullptr;
                }
            }
            default:
                LOG_ERROR("Unsupported database type");
                return nullptr;
        }
    }
}