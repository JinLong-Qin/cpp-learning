cmake_minimum_required(VERSION 3.15)
project(test2)

# 搜索源文件
aux_source_directory(./ SRC)

# 指定引入头文件的目录
include_directories(${HEAD_PATH})

# 给定自定义静态库的目录
link_directories(${LIB_PATH})
# 链接静态库 
link_libraries(${SORT_LIB})
# 由于sort lib在构建的时候链接了calc lib，所以这里不用再链接calc lib了

# 设置可执行文件存储目录
set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

add_executable(${APP_NAME_2} ${SRC})