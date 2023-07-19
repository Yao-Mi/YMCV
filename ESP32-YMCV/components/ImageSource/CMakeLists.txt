
# 查找当前目录下的所有源文件
file(GLOB_RECURSE image_SRC "./*.c")

#添加源文件列表
set(COMPONENT_SRCS ${image_SRC})

#添加头文件路径
set(COMPONENT_ADD_INCLUDEDIRS
  ./
  )

#注册组件
register_component()


