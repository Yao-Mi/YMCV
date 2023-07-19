

#添加源文件列表
set(COMPONENT_SRCS
  LED.c
  )

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
  ./
  )


#设置依赖项 （其他组件）
#set(COMPONENT_PRIV_INCLUDEDIRS
#  driver/private_include
#  )
#设置依赖项 （其他组件）
set(COMPONENT_REQUIRES driver)
#set(COMPONENT_PRIV_REQUIRES freertos nvs_flash)

#注册组件
register_component()


#组建注册(源文件，头文件，依赖项)
#idf_component_register(SRCS "LED.c"
#                    INCLUDE_DIRS "."
#                    REQUIRES driver
#                    )
