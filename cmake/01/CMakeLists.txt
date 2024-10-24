# 单行注释
#[[
    多行
    注释

    cmakefile中，所有东西都是字符串， 哪怕是123， 也是字符串，想要得到数值类型，需要转换

    CMake是一个项目构建工具
]]

cmake_minimum_required(VERSION 3.15) # 指定cmake的最低版本，可选命令
project(CALC) # 指定，cmake构建的这个项目的名字

# # 可以只指定项目名字
# project(<PROJECT-NAME> [<language-name>...]) 
# # 也可以指定其他内容
# project(<PROJECT-NAME>
#        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]] # 项目的版本号
#        [DESCRIPTION <project-description-string>] # 项目的介绍
#        [HOMEPAGE_URL <url-string>] # 项目链接
#        [LANGUAGES <language-name>...]) # 项目使用的语言

#[[
set命令
1. 定义变量 如下文的 SRC_LIST
2. 为固定的宏设置值 如 CMAKE_CXX_STANDARD, EXECUTABLE_OUTPUT_PATH

]]
# 设置c++标准
#增加-std=c++11
set(CMAKE_CXX_STANDARD 11)
#[[
#增加-std=c++14
set(CMAKE_CXX_STANDARD 14)
#增加-std=c++17
set(CMAKE_CXX_STANDARD 17)
]]
# 也可以在执行cmake命令的时候,通过增加参数来指定cpp的标准
#[[
#增加-std=c++11
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=11
#增加-std=c++14
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=14
#增加-std=c++17
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=17

]]

# 使用set在CMake中指定 [可执行程序输出] 的路径,建议 [使用绝对路径]
# 可执行程序输出 指的是,使用cmake命令生成makefile之后, 使用make生成可执行程序的保存目录
# 设定的目录如果不存在,在执行的时候,会自动创建这个目录
set(HOME /mnt/nvme1/1/cmake/01)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)





# 使用set设置变量 set(变量名 [参数]) 参数之间可以用空格，也可以用分号 
# set(SRC_LIST add.cpp div.cpp main.cpp mult.cpp sub.cpp) # 当文件多了之后，一个个写就不合适了
# ----当所有源文件和头文件都和CMakeLists.txt同一个目录中
# 方法1
# 宏 PROJECT_SOURCE_DIR 中保存的是执行cmake命令的时候，命令中跟随的路径，
# aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST) # 这表示检索这个目录，将检索到的源文件保存到SRC变量中
# 方法2
# 宏 CMAKE_CURRENT_SOURCE_DIR 中保存的是CMakeLists.txt所在的目录，
# file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# 参数1 [GLOB | GLOB_RECURSE]: 检索给定的目录 | 递归检索给定的目录
# 参数2 变量名，
# 参数3 给定的要检索的目录(${CMAKE_CURRENT_SOURCE_DIR})和检索规则(*.cpp)

# -----当呈项目结构的时候，即cpp文件在src h文件在include
file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# 设置头文件引用目录,这样头文件从根目录移动到include目录后，就不用一个个修改头文件的引用了
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_include_directories()


add_executable(app ${SRC_LIST}) # 定义工程会生成的可执行程序  
#[[
add_executable(可执行程序名 源文件名称)
源文件名可以是一个也可以是多个，如有多个可用空格或;间隔

# 样式1
add_executable(app add.c div.c main.c mult.c sub.c)
# 样式2
add_executable(app add.c;div.c;main.c;mult.c;sub.c)

]]

# 执行cmake命令
# cmake CMakeLists.txt所在的目录 

#[[
cmake -S . -B build
-S . 表示CMakeLists.txt文件目录
-B build 表示cmake命令生成的内容的输出目录
]]
# 然后在build目录中执行 make  表示通过makefile文件生成可执行文件