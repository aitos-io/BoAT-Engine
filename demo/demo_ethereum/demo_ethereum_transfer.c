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
#include "boatconfig.h"
#include "boatiotsdk.h"
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
const BCHAR *pkcs_demoKey = "-----BEGIN EC PRIVATE KEY-----\n"
                            "MHQCAQEEIJFHEUCiikP/iCuftqsvWt1kN+q3YxUNsP91gK2rSOhVoAcGBSuBBAAKoUQDQgAEFMuZrEghYdcix5oEP005i1qiKayJKe+ag4kQs85aNCx6tGMpqawC6JhdZi8r3FZLMVx1+hk+4rHLY3qS2TCCwg==\n"
                            "-----END EC PRIVATE KEY-----\n";
/**
 * native demo key
 */
const BCHAR *native_demoKey = "0x6cbed15c793ce57650b9877cf6fa156fbef513c4e6134f022a85b1ffdd59b2a1";

/**
 * test node url
 */

const BCHAR *demoUrl = "http://127.0.0.1:8545";

/**
 * transfer recipient address
 */
const BCHAR *demoRecipientAddress = "0x22d491Bde2303f2f43325b2108D26f1eAbA1e32b";

BoatEthWallet *g_ethereum_wallet_ptr;
BUINT8 keypairIndex = 0;
BUINT8 networkIndex = 0;

__BOATSTATIC BOAT_RESULT ethereum_createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32] = {0};

    (void)binFormatKey; // avoid warning

/* keypair_config value assignment */
#if defined(USE_PRIKEY_FORMAT_INTERNAL_GENERATION)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: internal generated.");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[pkcs].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)pkcs_demoKey;
    keypair_config.prikey_content.field_len = strlen(pkcs_demoKey) + 1; // length contain terminator
#elif defined(USE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE)
                        // BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;
#else
    /* default is internal generation */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
#endif

#if defined(USE_ONETIME_WALLET)
    /* create ethereum keypair */
    result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypairIndex = result;
    BoatLog(BOAT_LOG_NORMAL, " create keypair index = %d ", keypairIndex);
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT createNetwork()
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatEthNetworkConfig network_config = {0};

    network_config.chain_id = 1;
    network_config.eip155_compatibility = BOAT_FALSE;
    strncpy(network_config.node_url_str, demoUrl, BOAT_ETH_NODE_URL_MAX_LEN - 1);
#if defined(USE_ONETIME_WALLET)
    /* create ethereum wallet */
    result = BoATEthNetworkCreate(&network_config, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    result = BoATEthNetworkCreate(&network_config, BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif
    if (result < BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    networkIndex = result;
    BoatLog(BOAT_LOG_NORMAL, " create network index = %d ", networkIndex);
    return BOAT_SUCCESS;
}

BOAT_RESULT ethereumGetBalance(BoatEthWallet *wallet_ptr)
{
    // BCHAR *balance_wei;
    BCHAR *cur_balance_wei = NULL;
    BOAT_RESULT result;
    BoatFieldVariable parse_result = {NULL, 0};

    cur_balance_wei = BoatEthWalletGetBalance(wallet_ptr, NULL);
    result = BoatEthParseRpcResponseStringResult(cur_balance_wei, &parse_result);
    // BoatLog(BOAT_LOG_NORMAL, "Balance: %s wei", parse_result.field_ptr);
    if (parse_result.field_ptr != NULL)
    {
        BoatFree(parse_result.field_ptr);
    }
    if (result == BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_NORMAL, "BoatEthWalletGetBalance returns: %s", parse_result.field_ptr);
    }
    else
    {
        return result;
    }

    return BOAT_SUCCESS;
}

BOAT_RESULT ethereumTransfer(BoatEthWallet *wallet_ptr)
{
    BOAT_RESULT result;
    BoatEthTx tx_ctx;

    /* Set Recipient Address */
    result = BoatEthTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, NULL,
                           "0x333333",
                           (BCHAR *)demoRecipientAddress);

    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "BoatEthTxInit failed.");
        return BOAT_ERROR_WALLET_INIT_FAIL;
    }

    /* 0xDE0B6B3A7640000: 1ETH or 1e18 wei, value */
    /* 0x2386F26FC10000: 0.01ETH or 1e16 wei, value */
    result = BoatEthTransfer(&tx_ctx, "0x2386F26FC10000");

    return result;
}

int main(int argc, char *argv[])
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatIotKeypairContext walletList;
    boat_try_declare;
    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: create ethereum wallet */
    result = ethereum_createKeypair("keypair00");
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, ethereum_trans_demo_catch);
    }
    result = createNetwork();
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_CRITICAL, "ethereumWalletPrepare_create failed : %d.", result);
        // return -1;
        boat_throw(result, ethereum_trans_demo_catch);
    }
    result = BoATKeypair_GetKeypairList(&walletList);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "get wallet list failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }

    BoatLog(BOAT_LOG_NORMAL, "eth wallet init begin num = %d", walletList.keypairNum);
    BoatLog(BOAT_LOG_NORMAL, "eth wallet init walletIndex = %d ,networkIndex = %d ", keypairIndex, networkIndex);
    BoATKeypair_FreeKeypairContext(walletList);
    g_ethereum_wallet_ptr = BoatEthWalletInit(keypairIndex, networkIndex);
    if (g_ethereum_wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "BoatEthWalletInit fail");
        boat_throw(BOAT_ERROR, ethereum_trans_demo_catch);
    }
    BoatLog(BOAT_LOG_NORMAL, "eth wallet init OK");
    BoatLog(BOAT_LOG_NORMAL, "eth wallet url = %s ", g_ethereum_wallet_ptr->network_info.node_url_str);

    /* step-3: execute balance transfer */
    result = ethereumGetBalance(g_ethereum_wallet_ptr);
    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_NORMAL, "ethereumGetBalance Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }
    result = ethereumTransfer(g_ethereum_wallet_ptr);

    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_NORMAL, "ethereumTransfer Failed: %d.", result);
        boat_throw(result, ethereum_trans_demo_catch);
    }
    result = ethereumGetBalance(g_ethereum_wallet_ptr);

    if (result != BOAT_SUCCESS)
    {
        // BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Failed: %d.", result);
    }
    else
    {
        // BoatLog(BOAT_LOG_NORMAL, "CaseEthereum Passed.");
    }

    boat_catch(ethereum_trans_demo_catch)
    {
    }
    BoatEthWalletDeInit(g_ethereum_wallet_ptr);
    /* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return result;
}
