# 链接库
# lib1 中是静态库
# lib2 中是动态库
# 由于库本质上就是二进制表示的源文件，所以这里将src目录删除了

cmake_minimum_required(VERSION 3.15) 
project(CALC) 
set(CMAKE_CXX_STANDARD 11)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 如果是自定义的库，需要指定库的路径； 如果是系统提供的库，就不用指定这个路径
# 包含自定义库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib1)
# 链接静态库,
link_libraries(calc)
# 参数，给定不定数量个的库名（或库的全称 lib库名.后缀名），以空格分隔


add_executable(app ${SRC})

# 动态库具有可执行权限
# 制作的库实际上是将给定的源文件转换为二进制文件，所以在给别人用的时候，一方面要将制作的库给对方，还要将其对应的头文件(保存在include目录中的)给对方


