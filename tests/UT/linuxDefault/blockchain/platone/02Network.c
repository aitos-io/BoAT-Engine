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
#include "tcase_platone.h"

#define TEST_EIP155_COMPATIBILITY   BOAT_FALSE
#define TEST_PLATONE_CHAIN_ID      1
#ifndef TEST_PLATONE_NODE_URL
#define TEST_PLATONE_NODE_URL "127.0.0.1"
#endif
BoatPlatoneNetworkConfig get_platone_network_settings()
{
    g_platone_network_config.chain_id             = TEST_PLATONE_CHAIN_ID;
    g_platone_network_config.eip155_compatibility = TEST_EIP155_COMPATIBILITY;
    memset(g_platone_network_config.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(g_platone_network_config.node_url_str, TEST_PLATONE_NODE_URL, BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    return g_platone_network_config;
}

BOAT_RESULT check_platone_networkData(BoatPlatoneNetworkData *networkData,BUINT8 networkIndex,BoatPlatoneNetworkConfig *networkConfig)
{
    BOAT_RESULT ret = BOAT_ERROR;

    if((networkData->networkIndex != networkIndex) || \
        (networkData->chain_id != networkConfig->chain_id) || \
        (networkData->eip155_compatibility != networkConfig->eip155_compatibility) || \
        (0 != strncmp(networkData->node_url_str,networkConfig->node_url_str,strlen(networkData->node_url_str))) || \
        (strlen(networkData->node_url_str) != strlen(networkConfig->node_url_str)))
    {
        return ret;
    }

    return BOAT_SUCCESS;

}

BOAT_RESULT check_platone_network_in_networkList(BoatPlatoneNetworkConfig *networkConfig,BUINT8 networkIndex,BoatPlatoneNetworkContext *networkList)
{
    BOAT_RESULT ret = BOAT_ERROR;

    BUINT8 loop = networkList->networkNum;

    if(loop <= 0)
    {
        return ret;
    }

    for(BUINT8 i = 0;i < loop;i++)
    {
        if((networkIndex == networkList->networks[i].networkIndex) && \
            (networkConfig->chain_id == networkList->networks[i].chain_id) && \
                (networkConfig->eip155_compatibility == networkList->networks[i].eip155_compatibility) && \
                    (0 == strncmp(networkConfig->node_url_str,networkList->networks[i].node_url_str,strlen(networkConfig->node_url_str))) && \
                        (strlen(networkConfig->node_url_str) == strlen(networkList->networks[i].node_url_str)))
        {
            return BOAT_SUCCESS;
        }
    }

    return ret;
}

START_TEST(test_002Network_0001CreateOnetimeNetworkSuccess) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex;
    BoatPlatoneNetworkConfig networkConfig;

    BoatLog(BOAT_LOG_VERBOSE, "03 is start");

    BoatIotSdkInit();
    /* 1. Create network */
    networkConfig = get_platone_network_settings();
    networkIndex = BoatPlatoneNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 2.Verify network */
    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3.Check network list */
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_002Network_0002CreateOnetimeNetworkFailureNullConfig) 
{
    BOAT_RESULT networkIndex;

    BoatIotSdkInit();
    /* Create network */
    networkIndex = BoatPlatoneNetworkCreate(NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0003CreateOnetimeNetworkSuccessTwice) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex1;
    BOAT_RESULT networkIndex2;
    BoatPlatoneNetworkConfig networkConfig1;
    BoatPlatoneNetworkConfig networkConfig2;

    BoatIotSdkInit();
    /* 1.Create network once*/
    networkConfig1.chain_id = 111;
    networkConfig1.eip155_compatibility = true;
    memset(networkConfig1.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(networkConfig1.node_url_str, "nodeUrl1", BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    networkIndex1 = BoatPlatoneNetworkCreate(&networkConfig1,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex1, 0);

    /* 1.Create network twice*/
    networkConfig2.chain_id = 222;
    networkConfig2.eip155_compatibility = false;
    memset(networkConfig2.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(networkConfig2.node_url_str, "nodeUrl2", BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    networkIndex2 = BoatPlatoneNetworkCreate(&networkConfig2,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex2, 0);

    /* 3.Check network*/
    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,0);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData,0,&networkConfig1);
    ck_assert_int_eq(ret, BOAT_ERROR);

    ret = check_platone_networkData(&networkData,0,&networkConfig2);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.check network list*/
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig1,0,&networkList);
    ck_assert_int_eq(ret, BOAT_ERROR);

    ret = check_platone_network_in_networkList(&networkConfig2,0,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_002Network_0004CreatePersistNetworkSuccess) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex;
    BoatPlatoneNetworkConfig networkConfig;

    BoatIotSdkInit();
    /* 1. Create network */
    networkConfig = get_platone_network_settings();
    networkIndex = BoatPlatoneNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 2.Verify network */
    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3.Check network list */
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);
    
    /* 4.Delete persist network*/
    ret = BoATPlatoneNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0005Create2PersistNetworkSuccess) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex1;
    BOAT_RESULT networkIndex2;
    BoatPlatoneNetworkConfig networkConfig1;
    BoatPlatoneNetworkConfig networkConfig2;

    BoatIotSdkInit();
    /* 1. Create network1 */
    networkConfig1.chain_id = 111;
    networkConfig1.eip155_compatibility = true;
    memset(networkConfig1.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(networkConfig1.node_url_str, "nodeUrl1", BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    networkIndex1 = BoatPlatoneNetworkCreate(&networkConfig1,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex1,5);
    ck_assert_int_gt(networkIndex1,0);

    /* 2. Create network2*/
    networkConfig2.chain_id = 222;
    networkConfig2.eip155_compatibility = false;
    memset(networkConfig2.node_url_str,0U,BOAT_PLATONE_NODE_URL_MAX_LEN);
    strncpy(networkConfig2.node_url_str, "nodeUrl2", BOAT_PLATONE_NODE_URL_MAX_LEN - 1);

    networkIndex2 = BoatPlatoneNetworkCreate(&networkConfig2,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex2,5);
    ck_assert_int_gt(networkIndex2,0);

    /* 3.Verify network1 */
    BoatPlatoneNetworkData networkData1;
    ret = BoATPlatone_GetNetworkByIndex(&networkData1,networkIndex1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData1,networkIndex1,&networkConfig1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Verify network2*/
    BoatPlatoneNetworkData networkData2;
    ret = BoATPlatone_GetNetworkByIndex(&networkData2,networkIndex2);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData2,networkIndex2,&networkConfig2);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 5.Check network list */
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig1,networkIndex1,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig2,networkIndex2,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 6.Delete persist networks */
    ret = BoATPlatoneNetworkDelete(networkIndex1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATPlatoneNetworkDelete(networkIndex2);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0006CreatePersistNetworkFailureNullConfig) 
{
    BOAT_RESULT networkIndex;

    BoatIotSdkInit();
    /* Create network */
    networkIndex = BoatPlatoneNetworkCreate(NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_eq(networkIndex, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
    
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0007DeleteOnetimeNetworkSuccess) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex;
    BoatPlatoneNetworkConfig networkConfig;

    BoatIotSdkInit();
    /* 1. Execute 003_0001 */
    networkConfig = get_platone_network_settings();
    networkIndex = BoatPlatoneNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    /* 2. Delete network */
    ret = BoATPlatoneNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3. Check network */
    BoatPlatoneNetworkData networkData1;
    ret = BoATPlatone_GetNetworkByIndex(&networkData1,networkIndex);
    ck_assert_int_eq(ret, BOAT_ERROR_NETWORK_INEXISTENCE);

     /* 3. Check network list */
    BoatPlatoneNetworkContext networkList1;
    ret = BoATPlatone_GetNetworkList(&networkList1);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList1);
    ck_assert_int_eq(ret, BOAT_ERROR);
    
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0008DeleteOnetimeNetworkFailureNonExistentIndex) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex;
    BoatPlatoneNetworkConfig networkConfig;

    BoatIotSdkInit();
    /* 1. Execute 003_0001 */
    networkConfig = get_platone_network_settings();
    networkIndex = BoatPlatoneNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_networkData(&networkData,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    /* 2. Delete network */
    ret = BoATPlatoneNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 2. Delete network twice*/
    ret = BoATPlatoneNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_ERROR_NETWORK_INEXISTENCE);
    
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0009DeletePersistNetworkSuccess) 
{
    BOAT_RESULT ret;
    BOAT_RESULT networkIndex;
    BoatPlatoneNetworkConfig networkConfig;

    BoatIotSdkInit();
    /* 1. Create persist network */
    networkConfig = get_platone_network_settings();
    networkIndex = BoatPlatoneNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 2. Delete network */
    ret = BoATPlatoneNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 3. Check network */

    BoatPlatoneNetworkData networkData;
    ret = BoATPlatone_GetNetworkByIndex(&networkData,networkIndex);
    ck_assert_int_eq(ret, BOAT_ERROR_NETWORK_INEXISTENCE);

    /* 4. Check network list */
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = check_platone_network_in_networkList(&networkConfig,networkIndex,&networkList);
    ck_assert_int_eq(ret, BOAT_ERROR);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_002Network_0010DeletePersistNetworkFailureNonExistentIndex) 
{
    BOAT_RESULT ret;
    BUINT8 i,j,pickNum;

    BoatIotSdkInit();
    /* 1. Pick a NonExistent index */
    BoatPlatoneNetworkContext networkList;
    ret = BoATPlatone_GetNetworkList(&networkList);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BUINT8 networkNum = networkList.networkNum;
    for(i=1;i<5;i++)
    {
        for(j=0;j<networkNum;j++)
        {
            if(i == networkList.networks[j].networkIndex)
            {
                break;
            }
        }

        if(j >= networkNum)
        {
            pickNum = i;
            break;
        }

    }

    ck_assert_int_lt(pickNum,5);
    ck_assert_int_gt(pickNum,0);

    /* 2. Delete network */
    ret = BoATPlatoneNetworkDelete(pickNum);
    ck_assert_int_eq(ret, BOAT_ERROR_NETWORK_INEXISTENCE);

    BoatIotSdkDeInit();
}
END_TEST

Suite *make_network_suite(void) 
{
    /* Create Suite */
    Suite *s_network = suite_create("network");

    /* Create test cases */
    TCase *tc_network_api = tcase_create("network_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_network, tc_network_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_network_api, test_002Network_0001CreateOnetimeNetworkSuccess);  
    tcase_add_test(tc_network_api, test_002Network_0002CreateOnetimeNetworkFailureNullConfig);
    tcase_add_test(tc_network_api, test_002Network_0003CreateOnetimeNetworkSuccessTwice);
    tcase_add_test(tc_network_api, test_002Network_0004CreatePersistNetworkSuccess);
    tcase_add_test(tc_network_api, test_002Network_0005Create2PersistNetworkSuccess);
    tcase_add_test(tc_network_api, test_002Network_0006CreatePersistNetworkFailureNullConfig);
    tcase_add_test(tc_network_api, test_002Network_0007DeleteOnetimeNetworkSuccess);
    tcase_add_test(tc_network_api, test_002Network_0008DeleteOnetimeNetworkFailureNonExistentIndex);
    tcase_add_test(tc_network_api, test_002Network_0009DeletePersistNetworkSuccess);
    tcase_add_test(tc_network_api, test_002Network_0010DeletePersistNetworkFailureNonExistentIndex);

    return s_network;
}