# 测试说明

本目录包含使用 Google Test (gtest) 框架编写的单元测试。

## 构建测试

1. 确保已安装 Google Test：
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libgtest-dev
   
   # 或者从源码编译
   ```

2. 在项目根目录的 build 目录中运行 cmake（**重要：必须在项目根目录的 build 目录，而不是 test/build**）：
   ```bash
   # 在项目根目录
   cd build
   cmake ..
   make -j 4
   ```
   
   **注意**：测试必须在项目根目录的 `build` 目录中构建，因为测试需要访问 `src/common` 构建时生成的 ODB 文件。

3. 运行测试：
   ```bash
   # 运行所有测试
   ./bin/odb_handler_tests
   
   # 或者使用 ctest
   ctest
   ```

## 测试内容

- **UserHandler 测试**: 测试用户相关的数据库操作
- **MessageHandler 测试**: 测试消息相关的数据库操作
- **ChatSessionHandler 测试**: 测试聊天会话相关的数据库操作
- **RelationHandler 测试**: 测试关系相关的数据库操作
- **FriendApplyHandler 测试**: 测试好友申请相关的数据库操作
- **ChatSessionMemberHandler 测试**: 测试会话成员相关的数据库操作

## 注意事项

- 测试需要连接到数据库，请确保数据库服务正在运行
- 测试使用的数据库配置在 `odb_handler_test.cpp` 的 `SetUp()` 方法中
- 测试会创建和删除测试数据，请使用测试数据库

