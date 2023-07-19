

#添加源文件列表
#set(COMPONENT_SRCS
#  read_esp32_mac.c
#  )

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
  ./
  )


#设置依赖项 （其他组件）
#set(COMPONENT_REQUIRES efuse esp_rom)
#set(COMPONENT_PRIV_REQUIRES freertos nvs_flash)

#注册组件
register_component()

#链接对应的 libCONFIG.a 库文件
target_link_libraries(${COMPONENT_LIB} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/libCONFIG.a")

