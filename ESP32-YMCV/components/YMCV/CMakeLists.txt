#版本判断
cmake_minimum_required(VERSION 3.5)

#查找当前目录下的所有源文件 保存到 DIR_LIB_SRCS 变量
file(GLOB_RECURSE DIR_LIB_SRCS
		 "./Code/*.c"
		 "./CONFIG/*.c"
		 "./IOFILE/*.c"
		 "./DEBUG/*.c"
		 "./OPOBJ/*.c")

#添加源文件列表
set(COMPONENT_SRCS ${DIR_LIB_SRCS})

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
		"./")


#设置依赖项 （其他组件）
set(COMPONENT_REQUIRES heap)


#组建注册(源文件，头文件，依赖项)
#idf_component_register(SRCS "LED.c"
#INCLUDE_DIRS "."
#REQUIRES driver
#)


# Link the YMCV_CORE.a library
set(COMPONENT_EMBED_FILES "${CMAKE_CURRENT_LIST_DIR}/CORE/libYMCV-esp32.a")


#注册组件
register_component()



#链接对应的 YMCV.a 库文件 
# PUBLIC 之后的库和目标链接到链接接口，并成为链接接口的一部分。
# PRIVATE 之后的库和目标被链接到，但不是链接接口的一部分。
# INTERFACE 之后的库被附加到链接接口，不用于链接<target>.
#target_link_libraries(${COMPONENT_LIB} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/CORE/libYMCV-esp32.a" )

set(LIBS YMCV-esp32)
add_library(${LIBS} STATIC IMPORTED)
set_property(TARGET ${LIBS} PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/CORE/lib${LIBS}.a)
target_link_libraries(${COMPONENT_LIB} INTERFACE ${LIBS})
set_property(TARGET ${LIBS} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${COMPONENT_LIB})



