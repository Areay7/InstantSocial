cmake_minimum_required(VERSION 3.5)

project(InstantSocial)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    message("********** WIN32 ***********")
elseif(UNIX)
    message("********** UNIX ***********")
    # set(LIB_DIR "/Users/areay7/QtCode/QtProject/RabbitMQ_consumer/thirdparty")
    # add_definitions(-D ICE_CPP11_MAPPING)
    add_compile_options("-g")
    add_compile_options("-O2")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/common)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/message)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/message)

# 添加测试目录（需要在 common 之后，因为测试依赖 ODB 生成的文件）
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()