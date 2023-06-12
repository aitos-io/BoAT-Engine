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

## BoAT-Engine的编译和使用方法
参考 https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README_cn.md
