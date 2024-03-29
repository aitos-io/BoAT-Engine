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

/*!@brief Header file for performing RAW transaction

@file
boathlfabric.h is header file for RAW transaction construction and performing.
*/

#ifndef __BOATHLFABRIC_H__
#define __BOATHLFABRIC_H__

#include "boatconfig.h"
#include "boatinternal.h"
#include "boatEngine.h"

#if PROTOCOL_USE_HLFABRIC == 1

/*! @defgroup fabricp protocol-fabric
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!****************************************************************************
 * @brief
 *   proposal or transaction top level packed
 *
 * @param tx_ptr
 *   fabric transaction structure pointer
 *
 * @return
 *   Return \c BOAT_SUCCESS if packed successed, otherwise return a failed code.
 ******************************************************************************/
BOAT_RESULT hlfabricProposalTransactionPacked(BoatHlfabricTx *tx_ptr);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of PROTOCOL_USE_HLFABRIC */
#endif
