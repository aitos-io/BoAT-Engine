/**
****************************************************************************************
* @FilePath: boatEngine.h
* @Author: aitos
* @Date: 2023-02-27 20:58:14
* @LastEditors:
* @LastEditTime: 2023-02-27 20:58:14
* @Descripttion:
****************************************************************************************
*/
#ifndef __BOAT_ENGINE_H__
#define __BOAT_ENGINE_H__

#define BOAT_MAX_NETWORK_NUM 5

#include "persiststore.h"

//!@brief Blockchain Protocol types
typedef enum
{
    BOAT_PROTOCOL_UNKNOWN = 0, //!< Placeholder for unknown protocol
    BOAT_PROTOCOL_ETHEREUM,    //!< Ethereum
    BOAT_PROTOCOL_HLFABRIC,    //!< HyperLedger Fabric
    BOAT_PROTOCOL_PLATON,      //!< PlatON
    BOAT_PROTOCOL_PLATONE,     //!< PlatONE Enterprise consortium chain
    BOAT_PROTOCOL_FISCOBCOS,   //!< FISCOBCOS Enterprise consortium chain
    BOAT_PROTOCOL_HWBCS,       // huawei chain
    BOAT_PROTOCOL_CHAINMAKER,  // chainmaker
    BOAT_PROTOCOL_VENACHAIN,   // Venachain Enterprise consortium chain
    BOAT_PROTOCOL_QUORUM,      // quorum
    BOAT_PROTOCOL_CITA         // cita
} BoatProtocolType;

#if PROTOCOL_USE_ETHEREUM == 1
#include "network/network_ethereum.h"
#include "protocolapi/api_ethereum.h"
#endif

#if PROTOCOL_USE_PLATON == 1
#include "network/network_platon.h"
#include "protocolapi/api_platon.h"
#endif

#if PROTOCOL_USE_HLFABRIC == 1
#include "network/network_hlfabric.h"
#include "protocolapi/api_hlfabric.h"
#endif

#if PROTOCOL_USE_PLATONE == 1
#include "network/network_platone.h"
#include "protocolapi/api_platone.h"
#endif

#if PROTOCOL_USE_FISCOBCOS == 1
#include "network/network_fiscobcos.h"
#include "protocolapi/api_fiscobcos.h"
#endif

#if PROTOCOL_USE_HWBCS == 1
#include "network/network_hwbcs.h"
#include "protocolapi/api_hw_bcs.h"
#endif

#if ((PROTOCOL_USE_CHAINMAKER_V1 == 1) || (PROTOCOL_USE_CHAINMAKER_V2 == 1))
#include "network/network_chainmaker.h"
#include "protocolapi/api_chainmaker.h"
#endif

#if PROTOCOL_USE_VENACHAIN == 1
#include "network/network_venachain.h"
#include "protocolapi/api_venachain.h"
#endif

#if PROTOCOL_USE_QUORUM == 1
#include "network/network_quorum.h"
#include "protocolapi/api_quorum.h"
#endif

#if PROTOCOL_USE_CITA == 1
#include "network/network_cita.h"
#include "protocolapi/api_cita.h"
#endif

#endif