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
#include "tcase_venachain.h"
#ifndef TEST_VENACHAIN_NODE_URL
#define TEST_VENACHAIN_NODE_URL "127.0.0.1"
#endif

BOAT_RESULT check_venachain_wallet(BoatVenachainWallet *wallet,BUINT8 keypairIndex,BCHAR *keypairName,BoatKeypairPriKeyCtx_config *keypairConfig,
                                    BUINT8 networkIndex,BoatVenachainNetworkConfig *networkConfig)
{
    BOAT_RESULT ret = BOAT_ERROR;
    if(wallet == NULL)
    {
        return ret;
    }

    BCHAR temBuf[15]={0};
    BCHAR *temP = keypairName;
    if(keypairName == NULL)
    {
        memcpy(temBuf,"keypair",strlen("keypair"));
        temBuf[strlen("keypair")] = 0x30 + keypairIndex;
        temP = temBuf;
    }

    if((wallet->account_info.prikeyCtx.keypair_index != keypairIndex) || \
        (0 != strncmp(wallet->account_info.prikeyCtx.keypair_name,temP,strlen(temP))) || \
        (strlen(wallet->account_info.prikeyCtx.keypair_name) != strlen(temP)) || \
        (wallet->account_info.prikeyCtx.prikey_format != keypairConfig->prikey_format) || \
        (wallet->account_info.prikeyCtx.prikey_type != keypairConfig->prikey_type) || \
        (wallet->network_info.networkIndex != networkIndex) || \
        (wallet->network_info.chain_id != networkConfig->chain_id) || \
        (wallet->network_info.eip155_compatibility != networkConfig->eip155_compatibility) || \
        (0 != strncmp(wallet->network_info.node_url_str,networkConfig->node_url_str,strlen(networkConfig->node_url_str))) || \
        (strlen(wallet->network_info.node_url_str) != strlen(networkConfig->node_url_str)))
    {
        return ret;
    }

    return BOAT_SUCCESS;
}



START_TEST(test_003Wallet_0001InitWalletSuccessOneTimeKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0002InitWalletSuccessOneTimeKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Delete persist network*/
    ret = BoATVenachainNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0003InitWalletSuccessPersistKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair_index,5);
    ck_assert_int_gt(keypair_index,0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Delete persist keypair*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0004InitWalletSuccessPersistKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair_index,5);
    ck_assert_int_gt(keypair_index,0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Delete persist keypair and network*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATVenachainNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0005InitWalletFailureWrongKeypair) 
{
    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;
    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();

    /* 1. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 2.Init wallet */
    wallet_p = BoatVenachainWalletInit(5,networkIndex);
    ck_assert(wallet_p == NULL);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0006InitWalletFailureWrongNetwork) 
{
    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);


    /* 3.Init wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,5);
    ck_assert(wallet_p == NULL);

    BoatIotSdkDeInit();
}
END_TEST


START_TEST(test_003Wallet_0001DeInitWalletSuccessOneTimeKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);


    /* 4.DeInit wallet*/
    BoatVenachainWalletDeInit(wallet_p);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0002DeInitWalletSuccessOneTimeKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create onetime keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(keypair_index, 0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Deinit wallet*/
    BoatVenachainWalletDeInit(wallet_p);

    /* 5.Delete persist network*/
    ret = BoATVenachainNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0003DeInitWalletSuccessPersistKeypairOneTimeNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair_index,5);
    ck_assert_int_gt(keypair_index,0);

    /* 2. Create onetime network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_RAM);
    ck_assert_int_eq(networkIndex, 0);

    /* 3.Init onetime wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Deinit wallet*/
    BoatVenachainWalletDeInit(wallet_p);

    /* 5.Delete persist keypair*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0004DeInitWalletSuccessPersistKeypairPersistNetwork) 
{
    BOAT_RESULT ret;

    BOAT_RESULT keypair_index;
    BoatKeypairPriKeyCtx_config keypair_config;

    BOAT_RESULT networkIndex;
    BoatVenachainNetworkConfig networkConfig;

    BoatVenachainWallet *wallet_p = NULL;

    BoatIotSdkInit();
    /* 1. Create persist keypair */
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    UtilityHexToBin(g_binFormatKey, 32, g_venachain_private_key_buf, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = g_binFormatKey;
    keypair_config.prikey_content.field_len = 32;
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type	  = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;

    keypair_index = BoatKeypairCreate(&keypair_config,NULL,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(keypair_index,5);
    ck_assert_int_gt(keypair_index,0);

    /* 2. Create persist network */
    networkConfig.chain_id             = 300;
    networkConfig.eip155_compatibility = BOAT_FALSE;
    memset(networkConfig.node_url_str,0U,BOAT_VENACHAIN_NODE_URL_MAX_LEN);
    strncpy(networkConfig.node_url_str, TEST_VENACHAIN_NODE_URL, BOAT_VENACHAIN_NODE_URL_MAX_LEN - 1);

    networkIndex = BoatVenachainNetworkCreate(&networkConfig,BOAT_STORE_TYPE_FLASH);
    ck_assert_int_lt(networkIndex,5);
    ck_assert_int_gt(networkIndex,0);

    /* 3.Init wallet */
    wallet_p = BoatVenachainWalletInit(keypair_index,networkIndex);
    ret = check_venachain_wallet(wallet_p,keypair_index,NULL,&keypair_config,networkIndex,&networkConfig);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    /* 4.Deinit wallet*/
    BoatVenachainWalletDeInit(wallet_p);

    /* 5.Delete persist keypair and network*/
    ret = BoATIotKeypairDelete(keypair_index);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    ret = BoATVenachainNetworkDelete(networkIndex);
    ck_assert_int_eq(ret, BOAT_SUCCESS);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003Wallet_0011BoatVenachainParseRpcResponseResult_fail) 
{
	BOAT_RESULT ret = 0;
	ret = BoatVenachainParseRpcResponseResult(NULL,NULL,NULL);
    ck_assert_int_eq(ret, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_003Wallet_0001InitWalletSuccessOneTimeKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0002InitWalletSuccessOneTimeKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0003InitWalletSuccessPersistKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0004InitWalletSuccessPersistKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0005InitWalletFailureWrongKeypair);
    tcase_add_test(tc_wallet_api, test_003Wallet_0006InitWalletFailureWrongNetwork);

    tcase_add_test(tc_wallet_api, test_003Wallet_0001DeInitWalletSuccessOneTimeKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0002DeInitWalletSuccessOneTimeKeypairPersistNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0003DeInitWalletSuccessPersistKeypairOneTimeNetwork);
    tcase_add_test(tc_wallet_api, test_003Wallet_0004DeInitWalletSuccessPersistKeypairPersistNetwork);
	
    tcase_add_test(tc_wallet_api, test_003Wallet_0011BoatVenachainParseRpcResponseResult_fail);
	

    return s_wallet;
}

