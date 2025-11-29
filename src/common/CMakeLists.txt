project(test-common)

set(target "test-common")

set(PWD ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PWD}/../../bin)

set(odb_path ${PWD}/../../include/entity)

set(odb_files user_entity.h) 

include_directories(${PWD}/../../include/common)
include_directories(${PWD}/../../include/entity)
include_directories(${PWD}/../../include/common/odb_handler)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_program(ODB_COMPILER odb)
if(NOT ODB_COMPILER)
    message(FATAL_ERROR "ODB compiler not found! Please install ODB.")
endif()

foreach(odb_file ${odb_files})
    # 获取文件名（不含扩展名）- ODB 会自动去掉扩展名
    get_filename_component(odb_basename ${odb_file} NAME_WE)
    
    # 设置输入和输出文件路径
    set(odb_input ${odb_path}/${odb_file})
    set(odb_hxx_output ${CMAKE_CURRENT_BINARY_DIR}/${odb_basename}-odb.hxx)
    set(odb_cxx_output ${CMAKE_CURRENT_BINARY_DIR}/${odb_basename}-odb.cxx)
    set(odb_ixx_output ${CMAKE_CURRENT_BINARY_DIR}/${odb_basename}-odb.ixx)
    
    # 添加自定义命令生成ODB代码
    add_custom_command(
        OUTPUT ${odb_hxx_output} ${odb_cxx_output} ${odb_ixx_output}
        COMMAND ${ODB_COMPILER}
        ARGS 
            -d mysql 
            --std c++11 
            --generate-query 
            --generate-schema 
            --profile boost/date-time
            --output-dir ${CMAKE_CURRENT_BINARY_DIR}
            ${odb_input}
        DEPENDS ${odb_input}
        COMMENT "Generating ODB code for ${odb_file}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    # 将生成的源文件添加到列表
    list(APPEND odb_srcs ${odb_cxx_output})
    
endforeach()

set(COMMON_SOURCES 
${PWD}/main.cpp
${PWD}/logger.cpp
${PWD}/rabbitmq.cpp
${PWD}/redis_client.cpp
${PWD}/odb_client.cpp
${PWD}/odb_handler/user_handler.cpp
${odb_srcs}
)


add_executable(${target} ${COMMON_SOURCES})

target_link_libraries(${target} -lspdlog -lamqpcpp -lhiredis -lredis++ -lodb-mysql -lodb -lodb-boost -lev -lfmt -lpthread -ldl)