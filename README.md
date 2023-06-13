## BoAT-Engine

***BoAT-Engine*** is a blockchain application component implemented in the ***Composable BoAT Core*** layer of the ***BoAT Infra Arch*** architecture.

The ***BoAT Infra Arch*** is a cross-platform application framework that provides a unified cross-platform application abstraction interface for BoAT applications by abstracting different application APIs of diverse platforms. BoAT applications can quickly realize portability between different platforms by calling these abstract interfaces.

The ***BoAT Infra Arch*** consists of four layers:

1. The ***OS*** platform layer provides basic operating system support and system call API interfaces of each ***OS*** platform for the upper layer.

2. The ***BoAT Support Layer*** is the core layer of the ***BoAT Infra Arch*** architecture which provides a unified cross-platform application abstraction interface to the upper layer. It implements instances of cross-platform application abstraction interfaces according to system call API interfaces provided by different platforms. The cross-platform abstraction interface is compiled and selected through conditional compilation and provided for upper layer calls.

3. The ***Composable BoAT Core*** is the core component of BoAT's functionality which is implemented using the cross-platform application abstraction interface provided by the ***BoAT Support Layer***. It supports various blockchain operations, such as on-chain transactions and queries, etc.

4. The Application layer can implement application functionality directly using the cross-platform application abstraction interface provided by the ***BoAT Support Layer***. It can also use the blockchain operation interface provided by the ***Composable BoAT Core***, which is called the ***BoAT Engine***, to interact with various blockchains.

BoAT-Engine is a collection of blockchain application interfaces implemented under the BoAT Infra Arch architecture. It is a static library that realizes various blockchain applications, including on-chain transactions, on-chain data queries, and contract interactions, based on the BoAT-SupportLayer library of the BoAT Infra Arch architecture.

Applications call the API interfaces of BoAT-Engine by referencing the BoAT-Engine static library to implement blockchain operation functions.

BoAT-Engine retains and improves the blockchain-related function API interfaces of BoAT-X-Platform SDK (refer to: https://github.com/aitos-io/BoAT-X-Framework/blob/master/README_cn.md), and removes the auxiliary functions for implementing blockchain, such as key management and data communication in BoAT-X-Platform. The removed functions are separated into the BoAT Common Components component of BoAT-SupportLayer, such as keypair, keystore, common, third-party, etc.

BoAT Engine provides various blockchain network configuration management, protocol access, wallet access, etc. for different blockchains such as chainmaker, ethereum, fiscobcos, hlfabric, platon, platone, venachain. By using the interfaces provided by BoAT-Engine, application programs can quickly complete the implementation of blockchain applications according to their needs by referring to demo example programs.

## Directory Description
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

Here is the translation:

## Compiling and Using BoAT-Engine

Reference https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README.md
