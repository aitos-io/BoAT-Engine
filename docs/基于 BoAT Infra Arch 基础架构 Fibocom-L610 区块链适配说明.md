# 基于 BoAT Infra Arch 基础架构 Fibocom-L610 区块链适配说明


## 一、前言

本文档说明如何在 `Fibocom L610` 开发环境中使用 `BoAT Infra Arch` 基础架构开发模板`BoAT-ProjectTemplate`编译相关BoAT静态库以及生成区块链应用`Demo`固件程序。

本例中使用到 `BoAT Infra Arch` 基础架构的静态库包括：

`BoAT-SupportLayer` ： 为基于 BoAT Infra Arch 基础架构的应用程序和特定功能中间库提供平台操作系统调用接口抽象和驱动抽象应用接口。

`BoAT-Engine`：为基于 BoAT Infra Arch 基础架构的应用程序提供区块链访问应用接口。

`Fibocom L610` 的SDK版本号为 `core_sdk_16000.1000.00.96.64.04`。

### 编译环境
操作系统：
```
ubuntu 16.04
```
软件依赖：
```
gcc
   ubuntu 16.04已安装
make
   ubuntu 16.04已安装
git
   sudo apt install git -y
python3
   sudo apt install python3 -y
curl
   sudo apt install curl -y
```
通常使用虚拟机安装`ubuntu 16.04`操作系统，在其中进行编译。

## 二、构建编译目录

将`Fibocom L610` 平台SDK软件包拷贝到 ubuntu 16.04 系统中。

约定`<L610 Root>`是 `Fibocom L610` 平台SDK的根目录`core_sdk_16000.1000.00.96.64.04`。

1、下载BoAT-ProjectTemplate编译模板

在ubuntu 16.04操作系统中打开终端，进入<L610 Root>目录。

通过`git clone`命令，在`<L610 Root>`目录下下载`BoAT-ProjectTemplate`库，命令参考如下:

  ```
  git clone -b dev git@github.com:aitos-io/BoAT-ProjectTemplate.git
  ```
  或
  ```
  git clone -b dev https://github.com/aitos-io/BoAT-ProjectTemplate.git
  ```
  
2、进入 `<L610 Root>/BoAT-ProjectTemplate/` 目录，修改 `BoATLibs.conf` 文件

将 `BoATLibs.conf` 文件替换为以下内容：
```
BoAT-SupportLayer
BoAT-Engine
```
保存并退出。

3、在 `<L610 Root>/BoAT-ProjectTemplate/` 目录下执行配置脚本
```
python3 config.py
```
根据提示输入选择，配置过程如下：
```  
We will clone the BoAT-SupportLayer repository, which may take several minutes

Input the branch name or null:
``` 
输入‘dev’回车，选择BoAT-SupporLayer仓库dev分支。
``` 
Input the branch name or null:dev
branch name is [ -b dev]

git clone -b dev git@github.com:aitos-io/BoAT-SupportLayer.git

Cloning into 'BoAT-SupportLayer'...
remote: Enumerating objects: 2930, done.
remote: Counting objects: 100% (704/704), done.
remote: Compressing objects: 100% (327/327), done.
remote: Total 2930 (delta 441), reused 589 (delta 362), pack-reused 2226
Receiving objects: 100% (2930/2930), 3.40 MiB | 21.00 KiB/s, done.
Resolving deltas: 100% (1826/1826), done.
git cmd succ


We will clone the BoAT-Engine repository, which may take several minutes

Input the branch name or null:
```
输入‘dev’回车，选择BoAT-Engine仓库dev分支。
```
Input the branch name or null:dev 
branch name is [ -b dev]

git clone -b dev git@github.com:aitos-io/BoAT-Engine.git

Cloning into 'BoAT-Engine'...
remote: Enumerating objects: 900, done.
remote: Counting objects: 100% (39/39), done.
remote: Compressing objects: 100% (27/27), done.
remote: Total 900 (delta 18), reused 22 (delta 12), pack-reused 861
Receiving objects: 100% (900/900), 527.23 KiB | 37.00 KiB/s, done.
Resolving deltas: 100% (567/567), done.
git cmd succ


overwrite the Makefile?(Y/n):
```
输入回车，选择重写Makefile
```
Yes

 Select blockchain list as below:
 [1] ETHEREUM          : 
 [2] PLATON            : 
 [3] PLATONE           : 
 [4] FISCOBCOS         : 
 [5] HLFABRIC          : 
 [6] HWBCS             : 
 [7] CHAINMAKER_V1     : 
 [8] CHAINMAKER_V2     : 
 [9] VENACHAIN         : 
 [a] QUORUM            : 
 [b] CITA              : 
 [0] All block chains
 Example:
  Select blockchain list as below:
  input:1a
  Blockchain selected:
   [1] ETHEREUM
   [a] QUORUM

input:
```
选择使用的区块链，可以选择一个或多个区块链，本例选择三个区块链。

输入“9”回车，选择VENACHAIN区块链。
```
input:9
Blockchain selected:
 [9] VENACHAIN

Select the platform list as below:
[1] linux-default             : Default linux platform
[2] Fibocom-L610              : Fibocom's LTE Cat.1 module
[3] create a new platform
```
输入‘2’回车，选择“Fibocom-L610”平台
```
2
 
platform is : Fibocom-L610

include BoAT-SupportLayer.conf

include BoAT-Engine.conf


./BoAT-SupportLayer/demo/ False
./BoAT-Engine/demo/ True
Configuration completed
```
配置完成。

通过指令查询目录下的文件:
```
ls
BoAT-Engine  BoATLibs.conf  BoAT-SupportLayer  config.py  Makefile  README.md  README_en.md

``` 

完成上述操作后`<L610 Root>`目录下的部分目录和文件结构如下：
```
<L610 Root>
|
`-- BoAT-ProjectTemplate
      |-- BoAT-SupportLayer
      |-- BoAT-Engine
      |-- BoATLibs.conf
      |-- config.py
      |-- Makfile
      |-- README.md
      |-- README_en.md
`-- cmake
      |-- toolchain-gcc.cmake
|-- CMakeLists.txt
|-- hello_world.c
```

## 三、文件修改

### 1、修改BoAT-SupportLayer中的平台external.env

打开`<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/external.env`文件
  
配置本地实际的交叉编译器，例如：
```
CC := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
AR := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-ar
```

### 2、修改 httpclient.c 禁能 SSL
打开`<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal/http/httpclient.c`文件

屏蔽或删除SSL使能宏定义：
```
//#define BOAT_HTTPCLIENT_SSL_ENABLE
```
保存并退出

### 3、更改BoAT-Engine的demo程序

`BoAT-Engine`的`demo`程序不能作为单独的可执行程序生成，需要将程序中的`main`函数修改为任务函数供`hello_world.c`中的入口函数创建的任务。

将`<L610 Root>/BoAT-ProjectTemplate/BoAT-Engine/demo/demo_venachain_mycontract.c`修改如下。

在文件最后一个include 下增加宏定义：
```
#include "boatosal.h"

#define USE_ONETIME_WALLET
```

将：
```
#ifndef VENACHAIN_DEMO_IP
#define VENACHAIN_DEMO_IP "127.0.0.1"
#endif
```
替换为：
```
#ifndef VENACHAIN_DEMO_IP
#define VENACHAIN_DEMO_IP "x.x.x.x"
#endif
```
`"x.x.x.x"`，是venachain节点的ip地址，按照实际部署venachain区块链节点的地址填写。

将`main`函数名替换为api接口函数名
```
//int main(int argc, char *argv[])
int boatdemovenachainmain(void *param)
{
```

	
### 4、添加demo的源文件路径、头文件路径以及BoAT库的链接路径

1、打开`<L610 Root>/CMakeLists.txt`文件，

在`include_directories`中添加demo源文件路径和头文件路径，例如：
```
include_directories(components/include 
                    components/newlib/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/include        
                    BoAT-ProjectTemplate/BoAT-SupportLayer/tests
                    BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/log
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/osal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/third-party/cJSON
                    BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
                    BoAT-ProjectTemplate/BoAT-Engine/include
                    BoAT-ProjectTemplate/
                   )
```
在`if(CONFIG_APPIMG_LOAD_FLASH)`条件编译中添加 onenetdemo 需要编译的相关源文件路径和连接所需外部库名称，例如：
```
      add_appimg(${target} ${flash_ldscript}
        BoAT-ProjectTemplate/mycontract.cpp.abi.c
        BoAT-ProjectTemplate/BoAT-Engine/demo/demo_venachain/demo_venachain_mycontract.c
        hello_world.c)
      target_link_libraries(${target} PRIVATE ${libboatengine_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name} )
```
  如上 PRIVATE 后包含的连接库名称如下：
```  
  ${libboatengine_file_name} 
  ${libboatvendor_file_name} 
  ${libc_file_name} 
  ${libm_file_name} 
  ${libgcc_file_name}
```
以上各个链接库名称在 `<L610 Root>/cmake/toolchain-gcc.cmake` 文件中定义。
  
`<L610 Root>/CMakeLists.txt`文件配置结束。
  
2、打开`<L610 Root>//cmake/toolchain-gcc.cmake`文件，

在`if(CONFIG_CPU_ARM_CA5)`的条件下面添加BoAT相关静态库的路径：
```
if(CONFIG_CPU_ARM_CA5)
    set(abi_options -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4
        -mfloat-abi=hard -mno-unaligned-access)
    set(partial_link_options)
    if(L610_CN_02_90_ALIEVK)
    message(STATUS "BUILD L610_CN_02_90_ALIEVK")
    set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/alievk/libc.a)
    else()
    set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libc.a)
    endif()
    set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libm.a)
    set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a)
    set(libboatengine_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a)
endif()
```
`<L610 Root>//cmake/toolchain-gcc.cmake` 文件配置结束。

### 5、修改`<L610 Root>/hello_world.c`

添加头文件引用：
```
#include "boatlog.h"
```

将`hello_world.c`中 `appimg_enter`函数完整替换为以下内容:
```
void boatdemovenachainmain (void *param);
void demoapp(void *param)
{
    reg_info_t reg_info;
    int test = 1;
    while (test)
    {
        fibo_getRegInfo(&reg_info, 0);
        fibo_taskSleep(1000);
        if (reg_info.nStatus == 1)
        {
            test = 0;
            fibo_PDPActive(1, NULL, NULL, NULL, 0, 0, NULL);
            fibo_taskSleep(1000);
        }
    }
    boatdemovenachainmain(NULL);
    fibo_thread_delete();
}
void *appimg_enter(void *param)
{
    OSI_LOGI(0, "application image enter, param 0x%x", param);

    fibo_thread_create(demoapp, "mythread", 1024 * 36, NULL, OSI_PRIORITY_NORMAL);
    return (void *)&user_callback;
}
```

注意，需要清除hello_world.c中其他无用的代码，避免编译告警或错误。

## 四、BoAT静态库的编译和使用

### 1、编译BoAT静态库文件
   
   
打开终端并进入`<L610 Root>/BoAT-ProjectTemplate/`目录编译BoAT静态库：
```
make clean
make all
```

编译成功后，在`<L610 Root>/BoAT-ProjectTemplate/lib`下会生成两个静态库：
```
libboatvendor.a
libboatengine.a
```
### 2、创建Venachain合约api函数c文件和头文件

在`<L610 Root>/BoAT-ProjectTemplate/`目录下执行脚本创建Venachain合约api函数和头文件

```
python BoAT-Engine/tools/venachainSolidity2c.py BoAT-Engine/demo/demo_venachain/demo_contract/Solidity/mycontract.cpp.abi.json 
```

脚本执行后，在`<L610 Root>/BoAT-ProjectTemplate/`目录下会产生两个新文件：

```
ls mycontract*
```
查询结果：
```
mycontract.cpp.abi.h
mycontract.cpp.abi.c
```

不同的区块链使用不同的脚本创建区块链合约api函数c文件和头文件，这些脚本存放在`BoAT-Engine/tools/`目录中,可根据选择的区块链选择对应的脚本和合约json文件。

`BoAT-Engine/tools/`包含的脚本如下：
```
cita2c.py
eth2c.py
fiscobcos2c.py
platon2c.py     
platoneSolidity2c.py  
platoneWASM2c.py      
quorum2c.py             
venachainSolidity2c.py
venachainWASM2c.py
```

### 3、编译 L610 固件
   
打开终端并进入`<L610 Root>/`目录，执行以下指令完成固件编译
```
. tools/core_launch.sh
cout
cmake ../.. -G Ninja
ninja
```

编译成功后会在`<L610 Root>//out/appimage_debug/hex`下生成`hello_flash.pac`文件。
  
下载`hello_flash.pac`文件到模组可进行`onenetdemo`应用演示。模组代码下载参考L610平台操作说明。

## 五 `demo` 演示

将`Fibocom L610`开发板串口接入PC机，通过`Fibocom L610`调试软件接收模组发出的LOG信息，可监视区块链`demo`上链过程和结果。

