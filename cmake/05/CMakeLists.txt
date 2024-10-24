# 字符串拼接
# CMakeLists.txt文件中所有变量内容都是字符串，即便是没有使用双引号括起来

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

# set
set(tmp hello world)
set(tmp1 ${tmp} ${SRC_LIST})
message(${tmp})
message(${tmp1})

# 使用list进行追加
list(APPEND tmp "xx1" "ss2" ${SRC_LIST})
message(${tmp})

# 在list中删除对应的字符串
# list中底层存储的字符串中的分隔是分号，目的是为了方便查找和删除对应的值
# 要删除的内容一定要对应上每个分号中的全部内容
message("============") 
message(${SRC_LIST}) # SRC_LIST原本的内容
list(REMOVE_ITEM SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
message(${SRC_LIST})
message("============") 

# list获取list的长度
# list(LENGTH 要获取的list 用来保存长度的新变量)
# 要注意，这个长度也是字符串类型

# list获取对应索引的元素
# list(GET 要操作的list 索引1 [索引2...] 保存这个索引值的变量)
# 索引从0开始，也可以是负数 -1表示最后一个元素； 当索引超过list长度（不论正负），就报错

# 讲list使用连接符拼接成一个字符串
# list (JOIN 操作的list 指定的连接符 保存新的字符串)

# 查找list是否存在指定的元素
# list(FIND 要操作的list 要查找的元素 新变量（保存找到元素的索引，没找到就返回-1）)

# list指定位置插入若干元素
# list(INSERT <list> <element_index> <element> [<element> ...])

# 将元素插入到列表的0索引位置
# list (PREPEND <list> [<element> ...])

# 将列表中最后元素移除
# list (POP_BACK <list> [<out-var>...])

# 将列表中第一个元素移除
# list (POP_FRONT <list> [<out-var>...])

# 将指定索引的元素从列表中移除
# list (REMOVE_AT <list> <index> [<index> ...])

# 移除列表中的重复元素
# list (REMOVE_DUPLICATES <list>)

# 列表翻转
# list(REVERSE <list>)

# 列表排序
# list (SORT <list> [COMPARE <compare>] [CASE <case>] [ORDER <order>])
#[[
COMPARE：指定排序方法。有如下几种值可选：
    STRING:按照字母顺序进行排序，为默认的排序方法
    FILE_BASENAME：如果是一系列路径名，会使用basename进行排序
    NATURAL：使用自然数顺序排序
CASE：指明是否大小写敏感。有如下几种值可选：
    SENSITIVE: 按照大小写敏感的方式进行排序，为默认值
    INSENSITIVE：按照大小写不敏感方式进行排序
ORDER：指明排序的顺序。有如下几种值可选：
    ASCENDING:按照升序排列，为默认值
    DESCENDING：按照降序排列

]]








