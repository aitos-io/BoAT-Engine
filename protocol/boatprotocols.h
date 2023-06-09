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

/*!@brief Header file wrapper for protocols
@file
boatprotocols.h is a wrapper header file for protocols.
*/

#ifndef __BOATPROTOCOLS_H__
#define __BOATPROTOCOLS_H__

#if PROTOCOL_USE_ETHEREUM
#include "boatethereum/boatethereum.h"
#endif

#if PROTOCOL_USE_HLFABRIC
#include "boathlfabric/boathlfabric.h"
#endif

#if PROTOCOL_USE_PLATON
#include "boatplaton/boatplaton.h"
#endif

#if PROTOCOL_USE_PLATONE
#include "boatplatone/boatplatone.h"
#endif

#if PROTOCOL_USE_FISCOBCOS
#include "boatfiscobcos/boatfiscobcos.h"
#endif

#if PROTOCOL_USE_HWBCS
#include "boathwbcs/boathwbcs.h"
#endif

#if PROTOCOL_USE_CHAINMAKER_V1
#include "boatchainmaker_v1/boatchainmaker.h"
#endif

#if PROTOCOL_USE_CHAINMAKER_V2
#include "boatchainmaker_v2/boatchainmaker.h"
#endif

#if PROTOCOL_USE_VENACHAIN
#include "boatvenachain/boatvenachain.h"
#endif

#if PROTOCOL_USE_QUORUM
#include "boatquorum/boatquorum.h"
#endif

#if PROTOCOL_USE_CITA
#include "boatcita/boatcita.h"
#endif

#endif
