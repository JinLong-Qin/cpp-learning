# 在同一个项目目录下，有时候会根据情况构建多个CMakeLists.txt文件去做控制，按照目录关系可以构建树关系
# 通过 add_subdirectory 来构建多个CMakeLists.txt文件的父子关系
# 要注意的是，父结点中创建的变量在子结点中可以使用，反之则不行

#[[
├── calc                    // 构建为库供test1使用
│   ├── add.cpp
│   ├── CMakeLists.txt
│   ├── div.cpp
│   ├── mult.cpp
│   └── sub.cpp
├── CMakeLists.txt
├── include
│   ├── head.h
│   └── sort.h
├── sort                    // 构建为库供test2使用
│   ├── CMakeLists.txt
│   ├── insert.cpp
│   └── select.cpp
├── test1
│   ├── calc.cpp
│   └── CMakeLists.txt
└── test2
    ├── CMakeLists.txt
    └── sort.cpp



打包成动态库和静态库的一些基本规则：
    动态库：适用于源文件很多，并且要求最后的可执行程序大小要小
    静态库：适用于源文件少，并且对最后的可执行程序的大小没有要求


！通过拆分CMakeLists.txt文件后，实际上整体写起来是简单了的。
    这样在下面的子目录中的CMakeLists.txt可以只关注一件事，要生成库的就只生成库，要生成可执行文件的就只生成可执行文件。

如果项目目录中存在很多模块，那么在每一个模块中设计一个CMakeList.txt文件是可以简化管理的
]]
cmake_minimum_required(VERSION 3.15)
project(test)

# 定义变量
# 静态库生成路径
set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# 可执行程序的存储目录
set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 头文件导入目录
set(HEAD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 静态库名字
set(CALC_LIB calc)
set(SORT_LIB sort)
# 可执行程序名字
set(APP_NAME_1 app1)
set(APP_NAME_2 app2)

# 添加子目录
add_subdirectory(calc)
add_subdirectory(sort)
add_subdirectory(test1)
add_subdirectory(test2)