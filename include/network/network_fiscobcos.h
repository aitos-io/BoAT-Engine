/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-06 14:49:41
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-22 13:41:47
 */
/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Boatwallet SDK header file

@file
boatwallet.h is the SDK header file.
*/

#ifndef __NETWORK_FISCOBCOS_H__
#define __NETWORK_FISCOBCOS_H__

/*! @defgroup wallet boat wallet API
 * @{
 */

#include "boattypes.h"

#define BOAT_FISCOBCOS_NODE_URL_MAX_LEN 127 //!< Maxmum length for node's URL

//!@brief Fiscobcos network configuration

//! Fiscobcos network configuration is used in network creation.
typedef struct TBoatFiscobcosNetworkConfig
{
    BUINT32 chain_id;                                    //!< Chain ID (in host endian) of the blockchain network if the network is EIP-155 compatible
    BBOOL eip155_compatibility;                          //!< Network EIP-155 compatibility. See BoatFiscobcosNetworkInfo
    BCHAR node_url_str[BOAT_FISCOBCOS_NODE_URL_MAX_LEN]; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
} BoatFiscobcosNetworkConfig;

//! Fiscobcos network  is used in network creation.
typedef struct TBoatFiscobcosNetworkData
{
    BUINT8 networkIndex;                                 // index of network
    BUINT32 chain_id;                                    //!< Chain ID (in host endian) of the blockchain network if the network is EIP-155 compatible
    BBOOL eip155_compatibility;                          //!< Network EIP-155 compatibility. See BoatFiscobcosNetworkInfo
    BCHAR node_url_str[BOAT_FISCOBCOS_NODE_URL_MAX_LEN]; //!< URL of the blockchain node, e.g. "http://a.b.com:8545"
} BoatFiscobcosNetworkData;

//!@brief BoAT IoT SDK Context
typedef struct TBoatFiscobcosNetworkContext
{
    BUINT8 networkNum;
    // Protocol specific properties are defined in protocol specific WalletInfo structure
    BoatFiscobcosNetworkData networks[BOAT_MAX_NETWORK_NUM + 1]; //!< Wallet Info List
} BoatFiscobcosNetworkContext;

/**
 * @description:
 *  This function get network list ,include persistent networks and onetime network
 * @param[out] {BoatFiscobcosNetworkContext} *networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATFiscobcos_GetNetworkList(BoatFiscobcosNetworkContext *networkList);

/**
 * @description:
 *  This function use to free BoatFiscobcosNetworkContext param.
 * @param[in] {BoatFiscobcosNetworkContext} networkList
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATFiscobcos_FreeNetworkContext(BoatFiscobcosNetworkContext networkList);

/**
 * @description:
 * This function create platon network
 * @param[in] {BoatFiscobcosNetworkConfig} networkConfig
 * @param[in] {BoatStoreType} storeType
 *  For onetime network or test , select store in ram .
 *  For persistent network ,slect store in flash.
 * @return
 *   This function returns network index if create network successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATFiscobcosNetworkCreate(BoatFiscobcosNetworkConfig *networkConfig, BoatStoreType storeType);

/**
 * @description:
 *  This function delete the network which networkIndex equals index.
 *  This function will delete the network data in Nvram.
 * @param {BUINT8} index
 *  the index want to delete
 * @return {*}
 *   This function returns BOAT_SUCCESS if delete successfully.\n
 *   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *   for details.
 * @author: aitos
 */
BOAT_RESULT BoATFiscobcosNetworkDelete(BUINT8 index);

/**
 * @description:
 *  This function read network data from Nvram by index.
 * @param {BoatFiscobcosNetworkData} *networkData
 * @param {BUINT8} index
 *  the network index want to read
 * @return {*}
 *  This function returns BOAT_SUCCESS if successfully executed.
 *  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
 *  for details.
 * @author: aitos
 */
BOAT_RESULT BoATFiscobcos_GetNetworkByIndex(BoatFiscobcosNetworkData *networkData, BUINT8 index);

#endif
