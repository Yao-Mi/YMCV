# 版本判断
cmake_minimum_required(VERSION 3.0)
# 工程名
project(YMCV-ubuntu20)

# 设置编译选项
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_STANDARD 99)

#cmake预定义变量
#PROJECT_SOURCE_DIR：工程的根目录
#PROJECT_BINARY_DIR：运行 cmake 命令的目录，通常是 ${PROJECT_SOURCE_DIR}/build
#PROJECT_NAME：返回通过 project 命令定义的项目名称
#CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
#CMAKE_CURRENT_BINARY_DIR：target 编译目录
#CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
#CMAKE_CURRENT_LIST_LINE：当前所在的行
#CMAKE_MODULE_PATH：定义自己的 cmake 模块所在的路径，SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)，然后可以用INCLUDE命令来调用自己的模块
#EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
#LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置



# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/SDL_LCD
    ${PROJECT_SOURCE_DIR}/ImageSource
    ${PROJECT_SOURCE_DIR}/CONFIG
    ${PROJECT_SOURCE_DIR}/YMCV/**
)
# 添加链接库路径
link_directories(${PROJECT_SOURCE_DIR}/YMCV/CORE)

# 查找当前目录下的所有源文件 ,并将名称保存到 xxx_SRC 变量
# 编译成xxx_lib库
aux_source_directory(./SDL_LCD sdl_SRC)
add_library(sdl_lib ${sdl_SRC})

aux_source_directory(./ImageSource imgsource_SRC)
add_library(img_lib ${imgsource_SRC})

aux_source_directory(./YMCV/CONFIG ymcv_cfg_SRCS)
add_library(ymcv_cfg_lib ${ymcv_cfg_SRCS})

aux_source_directory(./YMCV/Code ymcv_code_SRCS)
add_library(ymcv_code_lib ${ymcv_code_SRCS})

aux_source_directory(./YMCV/DEBUG ymcv_debug_SRCS)
add_library(ymcv_debug_lib ${ymcv_debug_SRCS})

aux_source_directory(./YMCV/IOFILE ymcv_iofile_SRCS)
add_library(ymcv_iofile_lib ${ymcv_iofile_SRCS})

aux_source_directory(./YMCV/OPOBJ ymcv_opobj_SRCS)
add_library(ymcv_opobj_lib ${ymcv_opobj_SRCS})

#aux_source_directory(./YMCV/QRCODE ymcv_qrcode_SRCS)
#add_library(ymcv_qrcode_lib ${ymcv_qrcode_SRCS})

#打包上述的库名为SRC_LIST 注意顺序：依赖其他库的库一定要放到被依赖库的前面
set(SRC_LIST 	sdl_lib 
		img_lib 
		ymcv_iofile_lib 
		ymcv_code_lib 
		ymcv_opobj_lib
		ymcv_debug_lib 
		ymcv_cfg_lib
		)

message( STATUS "编译生成的库文件 ：${SRC_LIST}" )

#打包外部引用的库名为LINK_LIST 其中math库就是m
set(LINK_LIST 	SDL2 
		SDL2_ttf 
		SDL2main 
		pthread 
		m 
		ymcv-ubuntu20
		)	
message( STATUS "引用的外部库文件 ：${LINK_LIST}" )

# 手动添加要编译的Demo源文件名为MAIN_SRC
if(DemoFile)# 判断需要编译的文件
    set(MAIN_SRC ${PROJECT_SOURCE_DIR}/${DemoFile})
    message( STATUS "成功添加要编译的Demo文件 = ${MAIN_SRC}" )
else() #默认选择
    set(MAIN_SRC ${PROJECT_SOURCE_DIR}/Demo/0、SDL_Test.c)
    message( STATUS "未设置Demo ,默认为${MAIN_SRC}" )
endif()


# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)

# 编译添加为可执行文件
add_executable(CMk_YMCV_exe ${MAIN_SRC})

# 链接库  其中math库就是m  
#在链接命令中给出所依赖的库时，需要注意库之间的依赖顺序，依赖其他库的库一定要放到被依赖库的前面，
#这样才能真正避免undefined reference的错误，完成编译链接。
target_link_libraries(CMk_YMCV_exe ${SRC_LIST} ${LINK_LIST})


# 增量编译:

#1、指定了一个自定义的目标，名为 MyProject_IncrementalBuild,
#   它依赖于工程中的所有 C 代码文件，这里使用 file(GLOB_RECURSE ...) 命令来实现
file(GLOB_RECURSE SRC_LIST ${CMAKE_SOURCE_DIR}/*.c)
add_custom_target(MyProject_IncrementalBuild DEPENDS ${SRC_LIST})  
#2、指定了一个自定义的命令，它在每次编译 YMCV-linux-exe 后都会执行，以保证 MyProject_IncrementalBuild 会在每次编译时更新时间戳
#   这样，下次编译时，CMake 就会根据时间戳比较的结果，只编译修改过的代码文件，从而实现增量编译。
set_target_properties(MyProject_IncrementalBuild PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_custom_command(  
    TARGET CMk_YMCV_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_SOURCE_DIR}/CMakeLists.txt
    DEPENDS MyProject_IncrementalBuild
)

