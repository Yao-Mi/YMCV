

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
            ./)

#设置依赖项 （其他组件）
set(COMPONENT_REQUIRES driver)
#set(COMPONENT_PRIV_REQUIRES freertos nvs_flash)

#注册组件
register_component()
