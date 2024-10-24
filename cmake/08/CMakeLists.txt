# 嵌套链接静态库
#[[
├── calc                    // 构建为库 calc
│   ├── add.cpp
│   ├── CMakeLists.txt
│   ├── div.cpp
│   ├── mult.cpp
│   └── sub.cpp
├── CMakeLists.txt
├── include
│   ├── head.h
│   └── sort.h
├── sort                    // 构建为库 sort, 
│   ├── CMakeLists.txt
│   ├── insert.cpp          // 这里使用了calc中的方法，因此sort库要链接calc
│   └── select.cpp
└── test
    ├── CMakeLists.txt
    └── sort.cpp


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
add_subdirectory(test)