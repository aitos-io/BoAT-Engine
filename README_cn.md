## BoAT-Engine

***BoAT-Engine*** 是 ***BOAT EDGE***产品中实现区块链相关应用的组件。是基于[***BoAT Infra Arch***](https://github.com/phengao/hello-world/blob/master/docs/BoAT_Overall_Design_cn.md) 基础架构[***BoAT-SupportLayer***](https://github.com/aitos-io/BoAT-SupportLayer/blob/main/README_cn.md)库现实的区块链应用实例，是具有多种区块链上链、合约交互等应用功能的静态库。

***BoAT Infra Arch*** 基础架构，是一个跨平台应用框架，通过抽象不同平台的应用API，为BoAT应用提供统一的跨平台应用抽象接口，BoAT应用程序可以通过调用这些抽象接口快速实现在不同平台之间移植。

用户应用程序通过***BoAT-Engine***提供的接口，参考demo示例程序能迅速结合应用需求完成区块链应用实现。***BaAT Engine***，支持多种区块链应用接口，包括chainmaker、ethereum、fiscobcos、hlfabric、platon、platone、venachain  等，不同区块链应用API接口采用独立模块设计，能实现在同一个应用中访问多种区块链。

***BoAT-Engine***结合区块链应用的特性，将各个区块链应用接口统一划为三个组成部分，分别是：区块链协议、区块链网络、区块链钱包。

区块链协议***protocol***：  
  实现不同区块链协议的组包解包功能。

区块链网络***network***：  
  实现不同区块链网络信息管理和维护功能。

区块链钱包***wallet***：  
  通过区块链协议对象和区块链网路对象创建区块链钱包。
  实现钱包创建、钱包获取、钱包删除等钱包账号管理。
  实现不同区块链的交易访问功能。包含区块链连接建立、区块链功能调用、合约访问等功能。

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
## BoATEngine代码下载
[代码下载](https://github.com/aitos-io/BoAT-SupportLayer/releases/tag/BoAT-SupportLayer-v3.1.0)

## BoAT-Engine的编译和使用方法
[***BoATEngine***编译和使用方法](https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README_cn.md)
