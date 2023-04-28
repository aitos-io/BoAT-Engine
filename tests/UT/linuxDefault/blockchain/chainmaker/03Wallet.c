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
#include "tcase_chainmaker.h"
#include "boatosal.h"
#ifndef TEST_CHAINMAKER_NODE_URL
#define TEST_CHAINMAKER_NODE_URL "127.0.0.1"
#endif

const BCHAR* chainmaker_client_sign_prikey = 
"-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEICmuEW+1V4A088cAlGG5An7yph/FRU/TPbJ9wcJH2qL1oAoGCCqGSM49\n"
"AwEHoUQDQgAE6n3L2qRu/USN7tiztQ7JeyZX3PurXGi/Mo45vh5qQZ8GWB9ddutC\n"
"UqMck/WTqY73ASuk7JAP1JaM3g8+R5wbiA==\n"
"-----END EC PRIVATE KEY-----\n";

const BCHAR* chainmaker_client_sign_cert = 
"-----BEGIN CERTIFICATE-----\n"
"MIICeDCCAh6gAwIBAgIDDciJMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDQyMTA3MzIzMloXDTI4\n"
"MDQxOTA3MzIzMlowgZExCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MQ8wDQYDVQQLEwZjbGllbnQxLDAqBgNVBAMTI2NsaWVudDEuc2lnbi53eC1vcmcx\n"
"LmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE6n3L2qRu\n"
"/USN7tiztQ7JeyZX3PurXGi/Mo45vh5qQZ8GWB9ddutCUqMck/WTqY73ASuk7JAP\n"
"1JaM3g8+R5wbiKNqMGgwDgYDVR0PAQH/BAQDAgbAMCkGA1UdDgQiBCD5hfF9PVeL\n"
"40q2gVSXCJNmti9voBc/8K7pexVdxoGPGzArBgNVHSMEJDAigCBgwlwgFiGRqD8N\n"
"ZPtAo85N+d8tcOJVAP0GjzvBWPTKBDAKBggqhkjOPQQDAgNIADBFAiEAmnQGKnnk\n"
"xr2EwYVzODTs+oz8pEd7rke5EwvakynpU+0CIHWQ25BEL60k4o/u3e3PlHgVaxvM\n"
"XfFywPY5StdPe23H\n"
"-----END CERTIFICATE-----\n";


#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    const BCHAR* chainmaker_ca_tls_cert = 
"-----BEGIN CERTIFICATE-----\n"
"MIICnjCCAkSgAwIBAgIDBgpCMAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDQyMTA3MzIzMloXDTMz\n"
"MDQxODA3MzIzMlowgYoxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MRIwEAYDVQQLEwlyb290LWNlcnQxIjAgBgNVBAMTGWNhLnd4LW9yZzEuY2hhaW5t\n"
"YWtlci5vcmcwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT3G2EwezkxRBwzIbLo\n"
"+Uzkg8QR/rq13J8CgaOS1uxXsZnS+hJjsV3818m7HTWdTskK5C4R2L2YUume6yFG\n"
"+U6Zo4GWMIGTMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\n"
"DgQiBCBgwlwgFiGRqD8NZPtAo85N+d8tcOJVAP0GjzvBWPTKBDBFBgNVHREEPjA8\n"
"gg5jaGFpbm1ha2VyLm9yZ4IJbG9jYWxob3N0ghljYS53eC1vcmcxLmNoYWlubWFr\n"
"ZXIub3JnhwR/AAABMAoGCCqGSM49BAMCA0gAMEUCIDzSgjJ/JKlKg7E9JpmUFhbm\n"
"sPYw0FDIuMo9QPNFGXOKAiEAx1T2+dA7v8d8e2LkLhpdgNk8qoyzlvVcwQae7i/x\n"
"UoY=\n"
"-----END CERTIFICATE-----\n";


    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        const BCHAR* chainmaker_clinet_tls_prikey = 
"-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIBCUjAeupw07QEyQR0BuZCD1IB3BsrMhJIuVME1epT+zoAoGCCqGSM49\n"
"AwEHoUQDQgAEhzkLrAiFGh9isDrNyFCydTBwZs58FRC6s8aOjqYwFnr2dNXizUhZ\n"
"qmmfaRNzCNlTVh1mvIqp3n0kX0Hrvp54rw==\n"
"-----END EC PRIVATE KEY-----\n";


            const BCHAR* chainmaker_client_tls_cert = 
"-----BEGIN CERTIFICATE-----\n"
"MIICkTCCAjagAwIBAgIDBE/5MAoGCCqGSM49BAMCMIGKMQswCQYDVQQGEwJDTjEQ\n"
"MA4GA1UECBMHQmVpamluZzEQMA4GA1UEBxMHQmVpamluZzEfMB0GA1UEChMWd3gt\n"
"b3JnMS5jaGFpbm1ha2VyLm9yZzESMBAGA1UECxMJcm9vdC1jZXJ0MSIwIAYDVQQD\n"
"ExljYS53eC1vcmcxLmNoYWlubWFrZXIub3JnMB4XDTIzMDQyMTA3MzIzMloXDTI4\n"
"MDQxOTA3MzIzMlowgZQxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdCZWlqaW5nMRAw\n"
"DgYDVQQHEwdCZWlqaW5nMR8wHQYDVQQKExZ3eC1vcmcxLmNoYWlubWFrZXIub3Jn\n"
"MRMwEQYDVQQLEwpDaGFpbk1ha2VyMSswKQYDVQQDEyJjbGllbnQxLnRscy53eC1v\n"
"cmcxLmNoYWlubWFrZXIub3JnMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEhzkL\n"
"rAiFGh9isDrNyFCydTBwZs58FRC6s8aOjqYwFnr2dNXizUhZqmmfaRNzCNlTVh1m\n"
"vIqp3n0kX0Hrvp54r6N/MH0wDgYDVR0PAQH/BAQDAgP4MBMGA1UdJQQMMAoGCCsG\n"
"AQUFBwMCMCkGA1UdDgQiBCClZJ0IflrlBk2H3HPNzhBT8x90c6oIcgkDoU23aAMk\n"
"gzArBgNVHSMEJDAigCBgwlwgFiGRqD8NZPtAo85N+d8tcOJVAP0GjzvBWPTKBDAK\n"
"BggqhkjOPQQDAgNJADBGAiEA7sEf3xMMrmscqvs20uxIvtUZX3kytRIf1v7QBBiv\n"
"8K4CIQC1MmrKQbyP+ZhB33VyxUDnZqc0ZT7dFovKZ476HVqV+A==\n"
"-----END CERTIFICATE-----\n";

    #endif
#endif

BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";
//const BUINT8 pubkey_256R1[] = {0x4B, 0x06, 0x0A, 0x1D, 0xE7, 0xE0, 0x01, 0x52, 0x62, 0x9F, 0x51, 0x3C, 0x99, 0x2A, 0xC1, 0x6E, 0xC3, 0x29, 0x34, 0xA6, 0x72, 0x6E, 0x7B, 0x11, 0x03, 0x91, 0xE6, 0x4C, 0x6E, 0x95, 0x40, 0x3A, 0x79, 0xAC, 0x28, 0x46, 0x06, 0xFC, 0xA5, 0xE4, 0x6A, 0x27, 0xE3, 0x7C, 0xF5, 0x7D, 0xFD, 0xDC, 0x1D, 0xA6, 0xB5, 0x3F, 0x8D, 0xAD, 0x99, 0xC9, 0xE3, 0xDA, 0xC4, 0xB8, 0x97, 0x20, 0xB3, 0xBF};
const BUINT8 pubkey_256R1[] = {0xea, 0x7d, 0xcb, 0xda, 0xa4, 0x6e, 0xfd, 0x44, 0x8d, 0xee, 0xd8, 0xb3, 0xb5, 0x0e, 0xc9, 0x7b, 0x26, 0x57, 0xdc, 0xfb, 0xab, 0x5c, 0x68, 0xbf, 0x32, 0x8e, 0x39, 0xbe, 0x1e, 0x6a, 0x41, 0x9f, 0x06, 0x58, 0x1f, 0x5d, 0x76, 0xeb, 0x42, 0x52, 0xa3, 0x1c, 0x93, 0xf5, 0x93, 0xa9, 0x8e, 0xf7, 0x01, 0x2b, 0xa4, 0xec, 0x90, 0x0f, 0xd4, 0x96, 0x8c, 0xde, 0x0f, 0x3e, 0x47, 0x9c, 0x1b, 0x88};

BUINT8 binFormatKey[32] = {0};

BOAT_RESULT test_chainmaker_create_keypair(BCHAR *keypairName, BBOOL is_onetime, BUINT8 *keypair_index_ptr)
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]                    = {0};
    BSINT32 index;

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator

    /* create ethereum keypair */
    if (is_onetime)
    {
        index = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
    }

    else
    {
        index = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
    }

    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypair_index_ptr = index;
    return BOAT_SUCCESS;
}

BOAT_RESULT test_chainmaker_create_network(BBOOL is_onetime, BUINT8 *network_index_ptr)
{
    BSINT32 index;
    BoatChainmakerNetworkData networkConfig = {0};

    //set user cert context
    networkConfig.client_member_type              = BOAT_MEMBER_TYPE_CERT;
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert);
    if (networkConfig.client_sign_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert);
    networkConfig.node_url  = (char *)BoatMalloc(strlen(TEST_CHAINMAKER_NODE_URL) + 1);
    networkConfig.host_name = (char *)BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig.chain_id  = (char *)BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig.org_id    = (char *)BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig.node_url,  TEST_CHAINMAKER_NODE_URL);
    strcpy(networkConfig.host_name, chainmaker_host_name);
    strcpy(networkConfig.chain_id,  chainmaker_chain_id);
    strcpy(networkConfig.org_id,    chainmaker_org_id);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1) 
     networkConfig.ca_tls_cert_content.length = strlen(chainmaker_ca_tls_cert);
    if (networkConfig.ca_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        //set tls cert context
        networkConfig.client_tls_cert_content.length = strlen(chainmaker_client_tls_cert);
        if (networkConfig.client_tls_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy(networkConfig.client_tls_cert_content.content, chainmaker_client_tls_cert);

        networkConfig.client_tls_privkey_data.value_len = strlen(chainmaker_clinet_tls_prikey);
        if (networkConfig.client_tls_privkey_data.value_len  > BOAT_CHAINMAKER_PRIKEY_MAX_LEN)
        {
            return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
        }
        strcpy((BCHAR *)networkConfig.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
    #endif
#endif 
    /* create ethereum keypair */
    if (is_onetime)
    {
        index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
    }

    else
    {
         index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
    }

    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *network_index_ptr = index;

    return BOAT_SUCCESS;
}

START_TEST(test_003CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypair_index;
    BUINT8 network_index;
    BoatIotSdkInit();

    /* 1. execute unit test */
    rtnVal = test_chainmaker_create_keypair("keypairOnetime", BOAT_TRUE, &keypair_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypair_index, 0);
 
    rtnVal = test_chainmaker_create_network(BOAT_TRUE, &network_index);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(network_index, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey_256R1, sizeof(pubkey_256R1)), 0);
    /* check the result of getting network data */
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.networkIndex, 0);
     /* check client_member_type in the struct , must be same to the configuration */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.client_member_type, BOAT_MEMBER_TYPE_CERT);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.chain_id, chainmaker_chain_id);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.host_name, chainmaker_host_name);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.org_id, chainmaker_org_id);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, TEST_CHAINMAKER_NODE_URL);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        ck_assert_str_eq((char *)g_chaninmaker_wallet_ptr->network_info.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_cert_content.content, chainmaker_client_tls_cert);  
    #endif
#endif

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_network(BOAT_TRUE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    BoatIotSdkInit();

    rtnVal = test_chainmaker_create_keypair("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0004CreatePersistWalletSuccess) 
{
   
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = test_chainmaker_create_keypair("keypairPersist", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    rtnVal = test_chainmaker_create_network(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_chaninmaker_wallet_ptr, NULL);

    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_index, 1);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairPersist"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_chaninmaker_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey_256R1, sizeof(pubkey_256R1)), 0);
  
    /* check the result of getting network data */
    /* check index in the struct ,must be 0 */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.networkIndex, 1);
     /* check client_member_type in the struct , must be same to the configuration */
    ck_assert_int_eq(g_chaninmaker_wallet_ptr->network_info.client_member_type, BOAT_MEMBER_TYPE_CERT);
    /* check chainId in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.chain_id, chainmaker_chain_id);
    /* check hostname in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.host_name, chainmaker_host_name);
    /* check orgID in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.org_id, chainmaker_org_id);
    /* check url in the struct , must be same to the configuration */
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.node_url, TEST_CHAINMAKER_NODE_URL);

#if (BOAT_CHAINMAKER_TLS_SUPPORT == 1)
    ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.ca_tls_cert_content.content, chainmaker_ca_tls_cert);

    #if (BOAT_CHAINMAKER_TLS_IDENTIFY_CLIENT == 1)
        ck_assert_str_eq((char *)g_chaninmaker_wallet_ptr->network_info.client_tls_privkey_data.value, chainmaker_clinet_tls_prikey);
        ck_assert_str_eq(g_chaninmaker_wallet_ptr->network_info.client_tls_cert_content.content, chainmaker_client_tls_cert);  
    #endif
#endif

    BoATIotKeypairDelete(1);
    BoATChainmakerNetworkDelete(1);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreatePersistWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_network(BOAT_FALSE, &networkIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);

    BoATChainmakerNetworkDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreatePersistWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BoatIotSdkInit();

    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;

    rtnVal = test_chainmaker_create_keypair("keypairPersist", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    BoatChainmakerWallet *g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_chaninmaker_wallet_ptr, NULL);

    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST


Suite *make_wallet_suite(void) 
{
    /* Create Suite */
    Suite *s_wallet = suite_create("wallet");

    /* Create test cases */
    TCase *tc_wallet_api = tcase_create("wallet_api");
    tcase_set_timeout(tc_wallet_api, 5);
    /* Add a test case to the Suite */
    suite_add_tcase(s_wallet, tc_wallet_api);       
    /* Test cases are added to the test set */
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair); 
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork);

    tcase_add_test(tc_wallet_api, test_003CreateWallet_0004CreatePersistWalletSuccess);
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0002CreatePersistWalletFailureWithoutKeypair); 
    tcase_add_test(tc_wallet_api, test_003CreateWallet_0003CreatePersistWalletFailureWithoutNetwork);
    return s_wallet;
}

