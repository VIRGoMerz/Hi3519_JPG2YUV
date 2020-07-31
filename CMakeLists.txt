#CMake工具最低版本要求,使用命令“cmake --version”查看
cmake_minimum_required(VERSION 2.8)

#项目名称
project(demo)

MESSAGE(STATUS "Project: demo")

#设置交叉编译器
option(ARM_CROSS_COMPILE "Arm cross compiling option" ON)
if(ARM_CROSS_COMPILE)
    MESSAGE(STATUS "Arm cross compiling...")
    # 设置目标系统
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    # 设置工具链目录
    set(TOOL_CHAIN_DIR /opt/hisi-linux/x86-arm/arm-himix200-linux)
    set(TOOL_CHAIN_INCLUDE 
        ${TOOL_CHAIN_DIR}/include
        )
    set(TOOL_CHAIN_LIB 
        ${TOOL_CHAIN_DIR}/lib
        )

    # 设置编译器
    set(CMAKE_C_COMPILER "arm-himix200-linux-gcc")
    set(CMAKE_CXX_COMPILER "arm-himix200-linux-g++")

    MESSAGE(STATUS "CMAKE_C_COMPILER:" ${CMAKE_C_COMPILER})
    MESSAGE(STATUS "CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
    # 设置Cmake查找主路径
    set(CMAKE_FIND_ROOT_PATH ${TOOL_CHAIN_DIR})

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # 只在指定目录下查找库文件
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    # 只在指定目录下查找头文件
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    # 只在指定目录下查找依赖包
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    include_directories(
        ${TOOL_CHAIN_DIR}/include
        )

    set(CMAKE_INCLUDE_PATH 
        ${TOOL_CHAIN_INCLUDE}
        )

    set(CMAKE_LIBRARY_PATH 
        ${TOOL_CHAIN_LIB}
        )
endif(ARM_CROSS_COMPILE)
#添加头文件的路径 
include_directories( 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/include
)

#添加动态连接库的路径
link_directories(${CMAKE_SOURCE_DIR}/lib) 

#添加编译选项
add_definitions(-Wall -O2 -g -Wpsabi -fno-aggressive-loop-optimizations)
add_definitions(-Dhi3519av100 -DHI_XXXX -DISP_V2)
add_definitions(-mcpu=cortex-a53 -mfloat-abi=softfp -mfpu=neon-vfpv4)
add_definitions(-ffunction-sections -fdata-sections -fstack-protector -fPIC)
add_definitions(-DHI_RELEASE -Wno-error=implicit-function-declaration)
add_definitions(-DVER_X=1 -DVER_Y=0 -DVER_Z=0 -DVER_P=0 -DVER_B=10)
add_definitions(-DUSER_BIT_32 -DKERNEL_BIT_32 -Wno-date-time)
add_definitions(-DSENSOR0_TYPE=SONY_IMX334_MIPI_8M_30FPS_12BIT)
add_definitions(-DSENSOR1_TYPE=SONY_IMX290_SLAVE_MIPI_2M_60FPS_10BIT)
add_definitions(-DSENSOR2_TYPE=SONY_IMX290_SLAVE_MIPI_2M_60FPS_10BIT)
add_definitions(-DSENSOR3_TYPE=SONY_IMX290_SLAVE_MIPI_2M_60FPS_10BIT)
add_definitions(-DSENSOR4_TYPE=SONY_IMX334_MIPI_8M_30FPS_12BIT)
add_definitions(-DHI_ACODEC_TYPE_INNER -DHI_ACODEC_TYPE_HDMI -DSMP)

#添加源文件路径
aux_source_directory(${CMAKE_SOURCE_DIR}/src/. SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/common/. SRC)

#add_executable(想要生成的程序名 源文件)
add_executable(${PROJECT_NAME} ${SRC}) 

#添加编译时的依赖库
target_link_libraries(${PROJECT_NAME} -lopencv_world -lpthread -lrt -ldl -lm)
target_link_libraries(${PROJECT_NAME} libive.a libmpi.a libmd.a libhdmi.a libsecurec.a libVoiceEngine.a libupvqe.a libdnvqe.a)