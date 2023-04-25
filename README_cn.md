## BoAT-Engine
BoAT-Engine是在BoAT Infra Arch基础架构下实现的区块链应用接口集合，是基于BoAT Infra Arch基础架构BoAT-SupportLayer库现实的区块链应用实例，是具有多种区块链上链、链上数据查询、合约交互等应用功能的静态库。
应用程序通过引用BoAT-Engine静态库调用其API接口实现区块链操作功能。

BoAT-Engine保留改进BoAT-X-Platform （参考:https://github.com/aitos-io/BoAT-X-Framework/blob/master/README_cn.md） SDK区块链相关功能API接口，将BoAT-X-Platform中实现区块链的密钥管理、数据通信等辅助功能剥离，剥离的功能分离到BoAT-SupportLayer的BoAT Common Components组件中，如keypair、keystore、common、third-party等。

BaAT Engine，提供chainmaker、cita、ethereum、fiscobcos、hlfabric、hwbcs、platon、platone、quorum、venachain  11种区块链的区块链网络配置管理network、区块链协议访问protocol，钱包wallet访问等功能。
应用程序通过 BoAT-Engine提供的接口，参考demo示例程序能迅速结合应用需求完成区块链应用实现。

## 目录说明
### BoAT-Engine
```
<BoAT-Engine>
|
+---demo                  | Demo application
|   \---demo_chainmaker   |     network api of chainmaker
|   \---demo_cita         |     network api of cita
|   \---demo_ethereum     |     network api of ethereum
|   \---demo_fiscobcos    |     network api of fiscobcos
|   \---demo_hlfabric     |     network api of hlfabric
|   \---demo_hwbcs        |     network api of hwbcs
|   \---demo_platon       |     network api of platon
|   \---demo_platone      |     network api of platone
|   \---demo_quorum       |     network api of quorum
|   \---demo_venachain    |     network api of venachain
+---include               | Header files for application to include
+---network               | Configuration and management of blockchain networks
|   \---chainmaker        |     network api of chainmaker
|   \---cita              |     network api of cita
|   \---ethereum          |     network api of ethereum
|   \---fiscobcos         |     network api of fiscobcos
|   \---hlfabric          |     network api of hlfabric
|   \---hwbcs             |     network api of hwbcs
|   \---platon            |     network api of platon
|   \---platone           |     network api of platone
|   \---quorum            |     network api of quorum
|   \---venachain         |     network api of venachain
+---protocol              | Blockchain client protocol implementation
|   \---boatchainmaker_v1 |     protocol api of chainmaker
|   \---boatchainmaker_v2 |     protocol api of chainmaker
|   \---boatcita          |     protocol api of cita
|   \---boatethereum      |     protocol api of ethereum
|   \---boatfiscobcos     |     protocol api of fiscobcos
|   \---boathlfabric      |     protocol api of hlfabric
|   \---boathwbcs         |     protocol api of hwbcs
|   \---boatplaton        |     protocol api of platon
|   \---boatplatone       |     protocol api of platone
|   \---boatquorum        |     protocol api of quorum
|   \---boatvenachain     |     protocol api of venachain
|   +---common            |     Universal soft algorithms implementation for blockchains
|   |   \---web3intf      |     web3 api implementation
+---tests                 | Test cases
\---tools                 | Tools for generating C interface from contract ABI
\---wallet                | wallet API implementation for each blockchain
```

## BoAT-Engine的编译和使用
***BoAT-Engine的编译需要通过BoAT-ProjectTemplate模板构建编译目录来实现，操作步骤如下：***
1. 前提条件
    1. 操作系统 linux
    2. 在 Linux 操作系统中完成 git 安装,能正常拉取 GitHub 上的远程仓库
    3. 在 Linux 操作系统中安装 python3, version >= 3.5
    4. 在 Linux 操作系统中安装 gcc 及相关编译库
    
2. 在 Linux 操作系统下打开Terminal终端，可以在任意目录执行 git clone 命令克隆 BoAT-ProjectTemplate 模板：
    ``` 
    $ git clone -b dev https://github.com/aitos-io/BoAT-ProjectTemplate.git 
    Cloning into 'BoAT-ProjectTemplate'...
    remote: Enumerating objects: 16, done.
    remote: Counting objects: 100% (16/16), done.
    remote: Compressing objects: 100% (9/9), done.
    remote: Total 16 (delta 4), reused 11 (delta 2), pack-reused 0
    Unpacking objects: 100% (16/16), done.
    Checking connectivity... done.
    ```
    将BoAT-ProjectTemplate 开发模板 clone 到 BoAT-ProjectTemplate/目录

3. clone 成功后，进入 BoAT-ProjectTemplate/目录，目录下包含如下文件：
    ```
    |
    +---BoAT-ProjectTemplate/
    |---BoATLibs.conf
    |---config.py
    ```

4. 修改 BoATLibs.conf'，在BoATLibs.conf文件中写入：
        BoAT-SupportLayer
        BoAT-Engine
    
    以上内容表示在BoAT-ProjectTemplate编译模板中会使用以上两个git仓库的代码

5. 运行 config.py 脚本，Linux 操作系统下运行 python 脚本指令如下：
    ```  
    $ python3 config.py 
    We will clone the BoAT-SupportLayer repository, which may take several minutes
    Input the branch name or null:
    ```
    1.仓库分支输入交互：
        脚本在这里等待输入 clone BoAT-SupportLayer 仓库的分支名称或 tag 名称，
        将 BoAT-SupportLayer 仓库 clone 到 BoAT-ProjectTemplate/BoAT-SupportLayer/ 目录
        目前我们选用 dev 分支进行开发，输入:dev回车,

        Input the branch name or null:dev
        branch name is [ -b dev]
        
        git clone -b dev https://github.com/aitos-io/BoAT-SupportLayer.git
        
        Cloning into 'BoAT-SupportLayer'...
        remote: Enumerating objects: 614, done.
        remote: Counting objects: 100% (614/614), done.
        remote: Compressing objects: 100% (394/394), done.
        remote: Total 614 (delta 229), reused 581 (delta 199), pack-reused 0
        Receiving objects: 100% (614/614), 992.55 KiB | 987.00 KiB/s, done.
        Resolving deltas: 100% (229/229), done.
        Checking connectivity... done.
        git cmd succ

        We will clone the BoAT-Engine repository, which may take several minutes

        Input the branch name or null:

    2.仓库分支输入交互：
        脚本在这里等待输入clone BoAT-Engine 仓库的分支名称或tag名称，
        将 smApp 源码仓库 clone 到 BoAT-ProjectTemplate/BoAT-Engine/ 目录.
        假设我们选用 master 分支进行开发，输入:回车,

        Input the branch name or null:
        branch name is [ -b dev]
        
        git clone -b dev https://github.com/aitos-io/BoAT-Engine.git
        
        Cloning into 'BoAT-Engine'...
        remote: Enumerating objects: 335, done.
        remote: Counting objects: 100% (335/335), done.
        remote: Compressing objects: 100% (173/173), done.
        remote: Total 335 (delta 201), reused 290 (delta 159), pack-reused 0
        Receiving objects: 100% (335/335), 302.66 KiB | 578.00 KiB/s, done.
        Resolving deltas: 100% (201/201), done.
        git cmd succ
        
        overwrite the Makefile?(Y/n):
    3.重写 Makefile 交互：
        完成全部仓库 clone 后，脚本提示是否重写 Makefile，在第一次运行脚本时必须选择 Y 生成    Makefile,否则无法执行编译
        后续再次执行脚本可根据需求选择 Y 或 N
        直接输入回车，默认选择 Y
        这里直接输入:回车

        重写Makefile 将会把 BoAT-SupportLayer 和 BoAT-Engine 相关编译信息添加到 Makefile 中
        执行完 config.py 脚本后，可直接编译
        
        overwrite the Makefile?(Y/n):
        Yes

        Select blockchain list as BoAT-Enginelow:
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
        Select blockchain list as BoAT-Enginelow:
        input:1a
        Blockchain selected:
        [1] ETHEREUM
        [a] QUORUM
        
        input:
        
    4.选择 blockchain 交互：
        交互选项中提供了当前 BoAT-Engine 库支持的 blockchain 。
        示例中选择 [5] HLFABRIC
        
        input:5 
        Blockchain selected:
        [5] HLFABRIC
                
        Choose the platform list as BoAT-Enginelow:
        [1] linux-default             : Default linux platform
        [2] Fibocom-L610              : Fibocom's LTE Cat.1 module
        [3] create a new platform
    
    5.选择 platform 交互：
        交互中提供了当前 BoAT-SupportLayer 库支持的 platform 进行选择。
        [3] create a new platform 功能只做提示没有实现，后续完善该功能。
        在这里输入:1，选择 linux-default                       
    
        Choose the platform list as BoAT-Enginelow:
        [1] linux-default             : Default linux platform
        [2] Fibocom-L610              : Fibocom's LTE Cat.1 module
        [3] create a new platform
        1
        platform is : linux-default
        
        include BoAT-SupportLayer.conf
       
        include BoAT-Engine.conf
        
        Configuration completed
    
    自此脚本运行结束，完成 BoAT-SupportLayer 和 BoAT-Engine  源码拉取，并重写 Makefile，
    其中'include BoAT-SupportLayer.conf'和'nclude BoAT-Engine.conf' 为 BoAT-SupportLayer 
    和 BoAT-Engine 添加必要的头文件包含路径
    
    脚本执行完后的目录结构如下：
    ```
    +---BoAT-ProjectTemplate/
    |
    +---BoAT-Engine/
    |    ...
    |    |---Makefile
    |
    +---BoAT-SupportLayer/
    |    ...
    |    |---Makefile
    |
    |---BoATLibs.conf
    |---config.py
    |---Makefile
    ```
    
    在 BoAT-ProjectTemplate/目录下执行 make demo 
    编译成功后执行:
    ./build/BoAT-Engine/demo/demo_fabric//demo_fabric_abac_onetime  
    可演示hlfabric区块链操作过程。
## BoAT-Engine 应用方法
参考
https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README_cn.md
