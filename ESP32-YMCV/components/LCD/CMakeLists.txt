

#添加源文件列表
set(COMPONENT_SRCS
  LCD.c
  lcd_cfg.c
  )

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
  ./
  )


#设置依赖项 （本目录下）
#set(COMPONENT_PRIV_INCLUDEDIRS
#  driver/private_include
#  )

#设置依赖项 （其他组件）
set(COMPONENT_REQUIRES gpio_fast)
#set(COMPONENT_PRIV_REQUIRES freertos nvs_flash)

#注册组件
register_component()


