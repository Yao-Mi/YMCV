#设置组建依赖
#set(COMPONENT_REQUIRES
#    LED
#    nvs_flash
#    )


# Link the YMCV_CORE.a library
set(COMPONENT_EMBED_FILES "${CMAKE_CURRENT_LIST_DIR}/../components/YMCV/CORE/libYMCV-esp32.a")

#组建注册
idf_component_register(SRCS "ymcv_main.c" "app_camera.c"
                    INCLUDE_DIRS "./" "./include")
            

# 添加链接库路径
#link_directories(${PROJECT_SOURCE_DIR}/YMCV/CORE)

#链接对应的 YMCV.a 库文件 
# PUBLIC 之后的库和目标链接到链接接口，并成为链接接口的一部分。
# PRIVATE 之后的库和目标被链接到，但不是链接接口的一部分。
# INTERFACE 之后的库被附加到链接接口，不用于链接<target>.
#target_link_libraries(${COMPONENT_LIB} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../components/YMCV/CORE/libYMCV-esp32.a" )


