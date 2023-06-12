## BoAT-Engine

BoAT-Engine is a collection of blockchain application interfaces implemented under the BoAT Infra Arch infrastructure. It is a static library of blockchain applications based on the BoAT Infra Arch infrastructure BoAT-SupportLayer library. It provides various application functions on multiple blockchains, such as on-chain, on-chain data query, contract interaction, etc.

Applications call the API interfaces of BoAT-Engine by referencing the BoAT-Engine static library to implement blockchain operation functions.

BoAT-Engine retains and improves the blockchain-related function API interfaces of BoAT-X-Platform SDK (refer to: https://github.com/aitos-io/BoAT-X-Framework/blob/master/README_cn.md), and removes the auxiliary functions for implementing blockchain, such as key management and data communication in BoAT-X-Platform. The removed functions are separated into the BoAT Common Components component of BoAT-SupportLayer, such as keypair, keystore, common, third-party, etc.

BoAT Engine provides network configuration management (network), blockchain protocol access (protocol), wallet access, and other functions for 11 blockchains, including chainmaker, cita, ethereum, fiscobcos, hlfabric, hwbcs, platon, platone, quorum, and venachain. Applications can quickly complete blockchain application implementation based on their needs by referencing the interfaces provided by BoAT-Engine and referring to demo example programs.

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
