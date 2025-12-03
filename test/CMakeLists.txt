cmake_minimum_required(VERSION 3.5)

# 查找 Google Test
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# 包含目录
set(TEST_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/entity
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/common/odb_handler
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common
)

include_directories(${TEST_INCLUDE_DIRS})

# ODB 生成的文件路径（从 common 构建目录）
# 测试应该在项目根目录的 build 目录中运行（与 src/common 在同一构建树中）
# 这样 ODB 文件在 build/src/common 目录
set(ODB_BINARY_DIR ${CMAKE_BINARY_DIR}/src/common)

# 添加 ODB 生成的头文件路径
include_directories(${ODB_BINARY_DIR})

# ODB 生成的源文件
# 注意：这些文件在构建时由 src/common/CMakeLists.txt 生成
# 如果文件不存在，CMake 会在构建时生成它们（通过依赖关系）
set(ODB_SOURCES
    ${ODB_BINARY_DIR}/user_entity-odb.cxx
    ${ODB_BINARY_DIR}/relation_entity-odb.cxx
    ${ODB_BINARY_DIR}/message_entity-odb.cxx
    ${ODB_BINARY_DIR}/friend_apply_entity-odb.cxx
    ${ODB_BINARY_DIR}/chat_session_member_entity-odb.cxx
    ${ODB_BINARY_DIR}/chat_session_entity-odb.cxx
)

# 测试源文件
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/odb_handler_test.cpp
)

# 公共库源文件
set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/user_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/relation_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/message_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/friend_apply_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/chat_session_member_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/common/odb_handler/chat_session_handler.cpp
)

# 创建测试可执行文件
# 注意：ODB_SOURCES 可能还不存在，但会在构建时生成
add_executable(odb_handler_tests
    ${TEST_SOURCES}
    ${COMMON_SOURCES}
    ${ODB_SOURCES}
)

# 添加依赖，确保 common 目标先构建（这样 ODB 文件会先生成）
# 如果 test-common 目标存在，则依赖它
if(TARGET test-common)
    add_dependencies(odb_handler_tests test-common)
endif()

# 链接库
target_link_libraries(odb_handler_tests -lgtest -lgtest_main -lgmock -lspdlog -lamqpcpp -lhiredis -lredis++ -lodb-mysql -lodb -lodb-boost -lev -lfmt -lpthread -ldl)



# 设置输出目录
set_target_properties(odb_handler_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)

# 添加测试
add_test(NAME ODBHandlerTests COMMAND odb_handler_tests)

