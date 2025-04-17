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
#include "erc20Transfer.h"
#include "boatlog.h"

/**
 * macro used to select wallet type:
 * - USE_ONETIME_WALLET          : create a one-time wallet
 * - USE_CREATE_PERSIST_WALLET   : create a persist wallet
 * - USE_LOAD_PERSIST_WALLET     : load a persist wallet
 * 
 * macro used to select private key format:
 * - USE_PRIKEY_FORMAT_INTERNAL_GENERATION       : key is internal generated
 * - USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS   : key is pkcs format via external injection
 * - USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE : key is native format via external injection
 */

/**
 * PKCS format demo key. The original private key of 'pkcs_demoKey' is 
 * "fcf6d76706e66250dbacc9827bc427321edb9542d58a74a67624b253960465ca"
 */
const BCHAR *pkcs_demoKey =  "-----BEGIN EC PRIVATE KEY-----\n"
                             "MHQCAQEEIPz212cG5mJQ26zJgnvEJzIe25VC1Yp0pnYkslOWBGXKoAcGBSuBBAAK\n"
                             "oUQDQgAEMU/3IAjKpQc8XdURIGQZZJQRHZhPDkp80ahiRAM7KKV9Gmn699pei5fL\n"
                             "qZlYLvlxdQJsoh2IPyObgGr87gBT7w==\n"
                             "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
//const BCHAR *native_demoKey = "0x6cbed15c793ce57650b9877cf6fa156fbef513c4e6134f022a85b1ffdd59b2a1";
const BCHAR *native_demoKey = "0x0cfd728e2fcf8de0de28465cf134842a7d1cdc0cbe3fd8d5f5f2ae059e93bbc2";

/**
 * test node url
 */
#ifndef ETHEREUM_DEMO_URL
//#define ETHEREUM_DEMO_URL "https://sepolia.base.org"
//#define ETHEREUM_DEMO_URL "https://data-seed-prebsc-1-s2.binance.org:8545/"
//#define ETHEREUM_DEMO_URL "https://alfajores-forno.celo-testnet.org/"
#define ETHEREUM_DEMO_URL "https://wss-async.agung.peaq.network"

#endif

const BCHAR * demoUrl = ETHEREUM_DEMO_URL;

/**
 * transfer recipient address
 */
//const BCHAR *demoRecipientAddress = "0xe78a0f7e598cc8b0bb87894b0f60dd2a88d6a8ab";
//const BCHAR *demoRecipientAddress = "0x036cbd53842c5426634e7929541ec2318f3dcf7e"; // base usdc
//const BCHAR *demoRecipientAddress = "0x93eFC409Ff44788E8b1DAF395F46965046cAe84B"; // BNB SMART CHAIN TESTNET
//const BCHAR *demoRecipientAddress = "0x2F25deB3848C207fc8E0c34035B3Ba7fC157602B";// CELO TEST USDC
const BCHAR *demoRecipientAddress = "0xAef7a31b064031C474F5FE85d9c2f854308a1e59";// agung peaq LTK
BoatAddress toAddress = {0x9b, 0xfE, 0xd7, 0x5d, 0x18, 0x09, 0xb5, 0xF0, 0x62, 0xF7, 0x24, 0x1e, 0x59, 0x8F, 0xD3, 0x88, 0x43, 0x4A, 0xbd, 0xFE};


BoatEthWallet *g_ethereum_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;


__BOATSTATIC BOAT_RESULT ethereum_createKeypair(BCHAR * keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

	/* keypair_config value assignment */
    #if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: internal generated.");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[pkcs].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
        keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; //length contain terminator
    #elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
        //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
        keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
        UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
        keypair_config.prikey_content.field_ptr = binFormatKey;
        keypair_config.prikey_content.field_len = 32;
    #else  
        /* default is internal generation */  
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    #endif


	/* create ethereum keypair */
#if defined(USE_ONETIME_WALLET) 
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate( &keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id             = 9990;//agung peaq:9990,CELO:44787,BNB:97,BASE:84532;
    network_config.eip155_compatibility = BOAT_TRUE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);

	/* create ethereum wallet */
#if defined(USE_ONETIME_WALLET) 
    result = BoATEthNetworkCreate( &network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATEthNetworkCreate( &network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < 0)
	{
        //BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    networkIndex = result;
    
    return BOAT_SUCCESS;
}

BOAT_RESULT ethereum_call_usdc_transfer(BoatEthWallet *wallet_ptr)
{
    BCHAR *result_str;
    BUINT32 list_len;
    BUINT32 index_u256_big[32/4];
    BUINT32 index;
    BoatEthTx tx_ctx;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};
    
    /* Set Contract Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
						   "0x333333",
						   (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
	{
        //BoatLog(BOAT_LOG_NORMAL, "BoatEthTxInit fails.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }
    BUINT256 value = {0}; // 100000 = 0.1 usdc 
    value[2] = 0x01;
    value[1] = 0x86;
    value[0] = 0xa0;
    UtilityChangeEndian(toAddress, sizeof(BoatAddress));
    result_str = usdcTransfer_transfer(&tx_ctx, toAddress, value);
    
    if (result_str != NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "usdc transfer returns: %s", result_str);
/*        result = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
        if (result == BOAT_SUCCESS && result_str != NULL)
        {
            //BoatLog(BOAT_LOG_NORMAL, "readListLength returns: %s", result_str);
            
            UtilityHexToBin((BUINT8*)index_u256_big, sizeof(index_u256_big),
							(BCHAR*)parse_result.field_ptr, TRIMBIN_TRIM_NO, BOAT_FALSE);
            UtilityChangeEndian(index_u256_big, sizeof(index_u256_big));
            list_len = index_u256_big[0];      
            for (index = 0; index < list_len; index++)
            {
                result_str = StoreRead_readListByIndex(&tx_ctx, index);
                result     = BoatEthParseRpcResponseStringResult(result_str, &parse_result);
                if (result == BOAT_SUCCESS && result_str != NULL)
                {
                    //BoatLog(BOAT_LOG_NORMAL, "readListByIndex returns: %s", parse_result.field_ptr);
                }
                else 
                {
                    return BOAT_ERROR;
                }
            }
        }*/
    }

    return BOAT_SUCCESS;
}

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    boat_try_declare;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();
    
    /* step-2: create ethereum wallet */
    BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create one-time wallet.");
    result = ethereum_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed: %d.", result);
		//return -1;
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    BoatLog(BOAT_LOG_NORMAL,"    execute creat network");
    result = createNetwork();
    if (result != BOAT_SUCCESS)
	{
		 //BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed: %d.", result);
		//return -1;
        boat_throw(result, ethereum_storeread_demo_catch);
	}
    BoatLog(BOAT_LOG_NORMAL,"    execute wallet init");
    g_ethereum_wallet_ptr = BoatEthWalletInit(keypairIndex,networkIndex);
    if(g_ethereum_wallet_ptr == NULL){
        // BoatLog(BOAT_LOG_NORMAL,"BoatEthWalletInit fail");
        boat_throw(BOAT_ERROR, ethereum_storeread_demo_catch);
    }
    /* step-3: execute 'usdc transfer' */
    BoatLog(BOAT_LOG_NORMAL,"    execute usdc transfer");
    result = ethereum_call_usdc_transfer(g_ethereum_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        //BoatLog(BOAT_LOG_NORMAL, "ethereum readStore access Failed: %d.", result);
    }
    else
    {
        //BoatLog(BOAT_LOG_NORMAL, "ethereum readStore access Passed.");
    }
    boat_catch(ethereum_storeread_demo_catch)
    {
    }
    BoatEthWalletDeInit(g_ethereum_wallet_ptr);
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    return result;
}
