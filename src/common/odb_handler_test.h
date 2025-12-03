#ifndef ODB_HANDLER_TEST_H
#define ODB_HANDLER_TEST_H

#include "odb_client.h"
#include <memory>

namespace InstantSocial
{
    // 测试所有 ODB Handler 的函数
    void TestAllODBHandlers(std::shared_ptr<odb::database> db);
}

#endif // ODB_HANDLER_TEST_H

