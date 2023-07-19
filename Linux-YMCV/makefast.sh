#!/bin/sh


# 获取命令行参数
FILE_NAME=$1

# 判断是否存在参数
if [ -z ${FILE_NAME} ]; then
    echo "这样使用: ./makefast.sh 要编译的Demo文件名"
    exit 1
fi
# 判断文件是否存在
if [ ! -f "${FILE_NAME}" ]; then
    echo "没有找到文件名: ${FILE_NAME}"
    exit 1
fi

# 创建 build 目录
if [ ! -d "build" ]; then
    mkdir build
fi
# 进入 build 目录
cd build

# 执行 cmake 命令 (由于cmake进入了build，所以位置要先回退)   添加文件要以-D开头
cmake ../  -DDemoFile:STRING=${FILE_NAME}

#编译可执行文件
#使用make指令或者下面指令 
cmake --build .


#运行Demo
echo "开始运行 out/CMk_YMCV_exe:\n"
../out/CMk_YMCV_exe





        
