cmake_minimum_required(VERSION 3.15)
project(calc)

# 搜索源文件
aux_source_directory(./ SRC) # 搜索当前目录，搜索到的源文件保存在SRC变量中

# 指定头文件的目录
include_directories(${HEAD_PATH})
# 执行静态库生成的目录
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

# 将原文件转为静态库
add_library(${CALC_LIB} STATIC ${SRC}) # 库名，静态标志，要转换库的源文件