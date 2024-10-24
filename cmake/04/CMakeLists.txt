# 链接库
# lib1 中是静态库
# lib2 中是动态库
# 由于库本质上就是二进制表示的源文件，所以这里将src目录删除了

cmake_minimum_required(VERSION 3.15) 
project(CALC) 
set(CMAKE_CXX_STANDARD 11)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 给出自定义动态库的目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib2)

# ------ 动态库的链接
#[[
target_link_libraries(target(源文件|动态库|可执行文件) [权限 要连接的库])
    动态库的链接可以有权限控制: [PRIVATE|PUBLIC|INTERFACE] , 默认情况下都是PUBLIC
    动态库的链接具有传递性,如a 链接了 b, c 那么不做访问权限控制的情况下,a是可以使用b和c中的所有内容;此时如果有了d链接了a,那么d也将会拥有b和c中的所有内容
    但是:如果a链接了 PRIVATE b, 那么虽然此时d再链接a,就不能再获得到b中的任何内容了;即,PRIVATE只允许被控制的库的内容传递一次
    三种权限的范围大小是: PUBLIC > PRIVATE > INTERFACE
        PUBLIC会将库中的内容都导出target中,
        PRIVATE只会将库只导出到target中,并且不再会被继续传递
        INTERFACE不会将库的内容导出到target中,只会导出函数符号给target
]]


# ！动态库和静态库的调用方式的区别
# 如果是静态库，那么在生成可执行程序的时候，静态库的内容都会被打包到可执行程序中
# 但是动态库的话，动态库的内容不会被打包到可执行程序中，只有当可执行程序调用了动态库的内容的时候，才会将动态库加载到内存中


add_executable(app ${SRC})

#!! 动态库的链接一定要写在最后,因为第一个参数是要链接的主体,要保证链接的主体已经存在.在主体被生成之前,动态库并没有被链接的主体
target_link_libraries(app calc) 

# 动态库具有可执行权限
# 制作的库实际上是将给定的源文件转换为二进制文件，所以在给别人用的时候，一方面要将制作的库给对方，还要将其对应的头文件(保存在include目录中的)给对方


#[[
message([STATUS|WARNING|AUTHOR_WARNING|FATAL_ERROR|SEND_ERROR] "message to display" ...)

    (无) ：重要消息
    STATUS ：非重要消息
    WARNING：CMake 警告, 会继续执行
    AUTHOR_WARNING：CMake 警告 (dev), 会继续执行
    SEND_ERROR：CMake 错误, 继续执行，但是会跳过生成的步骤
    FATAL_ERROR：CMake 错误, 终止所有处理过程

    打印变量值的时候,需要使用${}括起来
]]

message("xxxxxxx")
message(STATUS "1xxxx")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
# message(FATAL_ERROR "2222222xxxxxxx")
message(STATUS "3333333xxxxxxx")