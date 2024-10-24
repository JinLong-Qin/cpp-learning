# 命令的介绍见01中的CMakeLists.txt文件

# 制作库 
#[[
库的名字格式： lib+库名字+后缀，制作库的时候需要指定的就是库名字
后缀：
            windows            linux
动态库        .dll              .so   
静态库        .lib               .a                

]]
cmake_minimum_required(VERSION 3.15) 

project(CALC) 

set(CMAKE_CXX_STANDARD 11)

# set(HOME /mnt/nvme1/1/cmake/01)
# set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add_executable(app ${SRC_LIST})

# 指定库文件保存的路径
# set(HOME /mnt/nvme1/1/cmake/02)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)

add_library(calc SHARED ${SRC_LIST})
# add_library(calc STATIC ${SRC_LIST}) # 不能同时生成动态库和静态库
# 参数1 库名字
# 参数2 [动态|静态]：[SHARED | STATIC]
# 参数3 制作这个库的源文件



# 动态库具有可执行权限
# 制作的库实际上是将给定的源文件转换为二进制文件，所以在给别人用的时候，一方面要将制作的库给对方，还要将其对应的头文件(保存在include目录中的)给对方


