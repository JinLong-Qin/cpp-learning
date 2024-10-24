cmake_minimum_required(VERSION 3.15)
project(test1)

# 搜索源文件
aux_source_directory(./ SRC)

# 指定引入头文件的目录
include_directories(${HEAD_PATH})

# 给定自定义静态库的目录
link_directories(${LIB_PATH})
# 链接静态库
link_libraries(${CALC_LIB})

# 设置可执行文件存储目录
set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

add_executable(${APP_NAME_1} ${SRC})