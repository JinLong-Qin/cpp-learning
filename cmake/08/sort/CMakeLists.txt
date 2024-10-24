cmake_minimum_required(VERSION 3.15)
project(sort)

# 搜索源文件
aux_source_directory(./ SRC) # 搜索当前目录，搜索到的源文件保存在SRC变量中

# 指定头文件的目录
include_directories(${HEAD_PATH})
# 执行静态库生成的目录
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

# 因为insert中使用了calc的add方法，所以这里链接calc的静态库
# 设置自定义库的目录
link_directories(${LIB_PATH})
# 链接calc静态库
link_libraries(${CALC_LIB})

# 将原文件转为静态库
add_library(${SORT_LIB} STATIC ${SRC}) # 库名，静态标志，要转换库的源文件