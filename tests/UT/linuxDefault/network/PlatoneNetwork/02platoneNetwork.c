/**
****************************************************************************************
* @FilePath: PlatoneNetwork_test.c
* @Author: aitos
* @Date: 2022-10-29 14:57:56
* @LastEditors:
* @LastEditTime: 2022-10-29 14:57:56
* @Descripttion:
****************************************************************************************
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
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"

/**
 * test node url
 */
const BCHAR *demoUrl[6] = {"http://127.0.0.0:7545",
                           "http://127.0.0.1:7545",
                           "http://127.0.0.2:7545",
                           "http://127.0.0.3:7545",
                           "http://127.0.0.4:7545",
                           "http://127.0.0.5:7545"};

const BUINT32 chainID[6] = {1, 2, 3, 4, 5, 6};

/**
****************************************************************************************
* @brief:
*  This function get the configuration to create platone network;
* @param[in/out] *config
* @param[in] networkIndex
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
static BOAT_RESULT getPlatoneNetworkConfig(BoatPlatoneNetworkConfig *config, BUINT8 networkIndex)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check param*/
    if ((NULL == config) || networkIndex > 5)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    if ((strlen(demoUrl[networkIndex]) + 1) > sizeof(config->node_url_str))
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    config->chain_id = chainID[networkIndex];
    config->eip155_compatibility = BOAT_FALSE;
    strcpy(config->node_url_str, demoUrl[networkIndex]);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime platone network success
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
    /* check eip155 flag */
    ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    /* delete the onetime networl */
    BoATPlatoneNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime platone network success;
*  Create a onetime network and again , check the result.
*  The second onetime network will cover the first one.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0002CreateMoreOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* create onetime network twice*/
    for (size_t i = 0; i < 2; i++)
    {
        /* get configuration */
        rtnVal = getPlatoneNetworkConfig(&network_config, i);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* create onetime network , store type must be RAM */
        rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
        /* check index of onetime network , the index must equal 0 */
        ck_assert_int_eq(rtnVal, 0);
    }
    /* get network data by index , index = 0 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the second configuration */
    ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
    /* check eip155 flag */
    ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the second configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    /* delete the onetime networl */
    BoATPlatoneNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist platone network success;
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0003CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* create persist network */
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network ;
        index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network, the index must equal 1  */
    ck_assert_int_eq(rtnVal, 1);
    /* get network data by index , index = 1 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, 1);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 1 */
    ck_assert_int_eq(networkdata.networkIndex, 1);
    /* check chainId in the struct , must be same to the  configuration */
    ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
    /* check eip155 flag */
    ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the  configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist platone network success;
*   have creted a persist network in above case and will create all the other 4 persist networks;
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0004CreateMorePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* create persist network */
    for (Index = 2; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get configuration */
        rtnVal = getPlatoneNetworkConfig(&network_config, Index);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* create persist network , store type must be FLASH */
        rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
        /* check index of persist network ;
            index of persist networks is form 1 to BOAT_MAX_NETWORK_NUM*/
        ck_assert_int_eq(rtnVal, Index);
        /* get network data by index  */
        rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the  configuration */
        ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
        /* check eip155 flag */
        ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
        /* check url in the struct , must be same to the  configuration */
        ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist platone network ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks, can't create another persist network,
*  this case will return error.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0005Create6PersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatPlatoneNetworkConfig network_config;
    /* create persist network */
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check result of creating persist network  */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get platone network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0006GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BoatPlatoneNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATPlatone_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
        num of network in the list must be BOAT_MAX_NETWORK_NUM*/
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM);
    /* check data of every network in the list */
    for (size_t i = 0; i < BOAT_MAX_NETWORK_NUM; i++)
    {
        /* check index of network , index of persist network begin with 1 */
        ck_assert_int_eq(networkList.networks[i].networkIndex, i + 1);
        /* check chainID of network , chainID[0] used by onetime network,
            chainID of persist network begin with chainID[1]*/
        ck_assert_int_eq(networkList.networks[i].chain_id, chainID[i + 1]);
        /* check eip155 flag of network */
        ck_assert(networkList.networks[i].eip155_compatibility == BOAT_FALSE);
        /* check url of network, demoUrl[0] used by onetime network,
            persist network begin with demoUrl[1] */
        ck_assert_int_eq(strcmp(networkList.networks[i].node_url_str, demoUrl[i + 1]), 0);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete platone network which index equal 3 ;
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0007DeletePersistNetworkSuccess_index3)
{
    BSINT32 rtnVal;
    /* delete network*/
    rtnVal = BoATPlatoneNetworkDelete(3);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get platone network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*  there are BOAT_MAX_NETWORK_NUM-1 networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0008GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatPlatoneNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATPlatone_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
        and deleted one which index equal 3 , num of network in the list must be BOAT_MAX_NETWORK_NUM - 1*/
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM - 1);
    /* check data of every network in the list */
    for (size_t i = 0; i < BOAT_MAX_NETWORK_NUM - 1; i++)
    {
        index = i + 1;
        if (index > 2)
        {
            index++;
        }
        /* check index of network , index of persist network begin with 1 */
        ck_assert_int_eq(networkList.networks[i].networkIndex, index);
        /* check chainID of network , chainID[0] used by onetime network,
            chainID of persist network begin with chainID[1]*/
        ck_assert_int_eq(networkList.networks[i].chain_id, chainID[index]);
        /* check eip155 flag of network */
        ck_assert(networkList.networks[i].eip155_compatibility == BOAT_FALSE);
        /* check url of network, demoUrl[0] used by onetime network,
            persist network begin with demoUrl[1] */
        ck_assert_int_eq(strcmp(networkList.networks[i].node_url_str, demoUrl[index]), 0);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist platone network success;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*   if create new persist network, index of the new persist network must be 3.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0009CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 3;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* create persist network */
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create persist network , store type must be FLASH */
    rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
    /* check index of persist network , index must be 3 */
    ck_assert_int_eq(rtnVal, 3);
    /* get network data by index , index = 3 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, 3);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 3 */
    ck_assert_int_eq(networkdata.networkIndex, 3);
    /* check chainId in the struct , must be same to the  configuration */
    ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
    /* check eip155 flag */
    ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the  configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create onetime platone network success
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0010CreateOnetimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 0;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* get configuration */
    rtnVal = getPlatoneNetworkConfig(&network_config, Index);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* create onetime network , store type must be RAM */
    rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
    /* check index of onetime network , the index must equal 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network data by index , index = 0 */
    rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, 0);
    /* check the result of getting network data */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(networkdata.networkIndex, 0);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
    /* check eip155 flag */
    ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
    /* check url in the struct , must be same to the configuration */
    ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    /* delete the onetime networl */
    BoATPlatoneNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist platone networks by index;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   get network data by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0011GetNetworkByIndex)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatPlatoneNetworkData networkdata;
    /* create persist network */
    for (Index = 1; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get network data by index  */
        rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the  configuration */
        ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
        /* check eip155 flag */
        ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
        /* check url in the struct , must be same to the  configuration */
        ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete all the persist networks ;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   delete network  by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0012DeleteAllNetworkSuccess)
{
    BSINT32 rtnVal;
    for (size_t i = 1; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete network*/
        rtnVal = BoATPlatoneNetworkDelete(i);
        /* check the result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get platone network list ;
*  deleted all the netsorks ,have no networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0013GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatPlatoneNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATPlatone_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have no networks,
        num of network in the list must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create all the platone networks success;
*  create a onetime network and BOAT_MAX_NETWORK_NUM persist networks.
* @return
****************************************************************************************
*/
START_TEST(test_002platoneNetwork_0014CreateAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 Index = 1;
    BoatPlatoneNetworkConfig network_config;
    BoatPlatoneNetworkData networkdata;
    /* create all networks */
    for (Index = 0; Index <= BOAT_MAX_NETWORK_NUM; Index++)
    {
        /* get configuration */
        rtnVal = getPlatoneNetworkConfig(&network_config, Index);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        if (Index == 0)
        {
            /* create onetime network , store type must be RAM */
            rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
        }
        else
        {
            /* create persist network , store type must be FLASH */
            rtnVal = BoatPlatoneNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
        }
        /* check index */
        ck_assert_int_eq(rtnVal, Index);
        /* get network data by index  */
        rtnVal = BoATPlatone_GetNetworkByIndex(&networkdata, Index);
        /* check the result of getting network data */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check index in the struct ,must be Index */
        ck_assert_int_eq(networkdata.networkIndex, Index);
        /* check chainId in the struct , must be same to the  configuration */
        ck_assert_int_eq(networkdata.chain_id, chainID[Index]);
        /* check eip155 flag */
        ck_assert(networkdata.eip155_compatibility == BOAT_FALSE);
        /* check url in the struct , must be same to the  configuration */
        ck_assert_int_eq(strcmp(networkdata.node_url_str, demoUrl[Index]), 0);
    }
}
END_TEST

Suite *make_platoneNetworkUtTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0002CreateMoreOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0003CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0004CreateMorePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0005Create6PersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0006GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0007DeletePersistNetworkSuccess_index3);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0008GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0009CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0010CreateOnetimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0011GetNetworkByIndex);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0012DeleteAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0013GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0014CreateAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002platoneNetwork_0012DeleteAllNetworkSuccess);

    return s_networkcreate;
}