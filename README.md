# 项目简介

> 使用海思平台完成 JPG 格式图像转换 YUV420SP（NV21）格式图像的操作
>
> 方便大家进行 IVE 或 NNIE 的测试

# 注意事项

- 支持在使用 arm-himix200-linux 编译器的海思平台上运行
- 如果不是 Hi3519AV100 的话，可能需要修改 CMakeLists.txt 里的编译选项
- 需要复制  <kbd>lib/ libopencv_world.so</kbd> 到海思平台上

* 仅支持 `64x64～1920x1080` 大小的图片进行转换

# 使用方法

1. 编译项目

   `cd build`

   `cmake ..`

   `make`

2. 在 <kbd>data/images/JPG</kbd> 目录下放你需要转换的图片

3. 在 <kbd>build</kbd> 目录下执行程序

4. 转换好的 YUV 图片在 <kbd>data/images/YUV</kbd> 目录下

# 贡献

1. 使得 `IVE` 模块独立出来，抛弃了冗余的模块
2. 使用 `cmake` 进行编译，同 `Makefile` 相比可读性更好
3. 因为 `IVE` 算子有限，所以加入了 `opencv-3.4.3`

