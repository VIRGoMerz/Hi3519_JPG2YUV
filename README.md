# 简介

> 使用海思平台完成 JPG 格式图像转换 YUV420SP（NV21）格式图像的操作
>
> 方便大家进行 IVE 或 NNIE 的测试

# 环境

- 仅支持在使用 arm-himix200-linux 编译器的海思平台上运行
- 可能需要修改 CMakeLists.txt 里的编译器路径
- 需要复制  <kbd>lib/ libopencv_world.so</kbd> 到海思平台上



# 修改内容

1. 使得 `IVE` 模块独立出来
2. 使用 `cmake` 进行编译
3. 支持 `opencv`

# 编译命令

`cd build`

`cmake ..`

`make`

# 使用方法

1. 编译项目
2. 在 <kbd>data/images/JPG</kbd> 目录下放你需要转换的图片
3. 执行程序
4. 转换好的 YUV 图片在 <kbd>data/images/YUV </kbd> 目录下



