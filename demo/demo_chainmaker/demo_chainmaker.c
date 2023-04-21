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
#include "boatkeypair.h"
#include "boatEngine.h"
#include "network_chainmaker.h"
#include "protocolapi/api_chainmaker.h"
#include "boatosal.h"


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

#ifndef CHAINMAKER_DEMO_IP
#define CHAINMAKER_DEMO_IP "127.0.0.1"
#endif

BCHAR *chainmaker_node_url   = CHAINMAKER_DEMO_IP ":12301";
BCHAR *chainmaker_host_name  = "chainmaker.org";
BCHAR *chainmaker_chain_id   = "chain1";
BCHAR *chainmaker_org_id     = "wx-org1.chainmaker.org";

BoatChainmakerWallet *g_chaninmaker_wallet_ptr;
BoatChainmakerWallet  wallet_config = {0};
BUINT8 keypair_index = 0;
BUINT8 network_index = 0;

__BOATSTATIC BOAT_RESULT chainmaker_create_keypair()
{
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BOAT_RESULT result                         = BOAT_SUCCESS;
    BUINT8 binFormatKey[32]                    = {0};

    (void)binFormatKey; //avoid warning

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)chainmaker_client_sign_prikey;
    keypair_config.prikey_content.field_len = strlen(chainmaker_client_sign_prikey) + 1; //length contain terminator
    
    /* create ethereum keypair */
#if defined(USE_ONETIME_WALLET)
    result = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, "create keypair persist");
    result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
#else
    result = BOAT_ERROR;
#endif

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    keypair_index = result;
    
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT chainmaker_create_network(void)
{
    BOAT_RESULT index;
    BoatChainmakerNetworkData networkConfig = {0};

    //set user cert context
    networkConfig.client_member_type              = BOAT_MEMBER_TYPE_CERT;
    networkConfig.client_sign_cert_content.length = strlen(chainmaker_client_sign_cert);
    if (networkConfig.client_sign_cert_content.length  > BOAT_CHAINMAKER_CERT_MAX_LEN)
    {
        return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
    }
    strcpy(networkConfig.client_sign_cert_content.content, chainmaker_client_sign_cert);

    networkConfig.node_url  = BoatMalloc(strlen(chainmaker_node_url) + 1);
    networkConfig.host_name = BoatMalloc(strlen(chainmaker_host_name) + 1);
    networkConfig.chain_id  = BoatMalloc(strlen(chainmaker_chain_id) + 1);
    networkConfig.org_id    = BoatMalloc(strlen(chainmaker_org_id) + 1);

    strcpy(networkConfig.node_url,  chainmaker_node_url);
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

#if defined(USE_ONETIME_WALLET)
    index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);

#elif defined(USE_CREATE_PERSIST_WALLET)
    BoatLog(BOAT_LOG_NORMAL, "startting create persist wallet");
    index = BoATChainmakerNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);

#else
    return BOAT_ERROR;
#endif
    if (index < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    network_index = index;

    if (networkConfig.node_url != NULL)
    {
        BoatFree(networkConfig.node_url);
    }

    if (networkConfig.host_name != NULL)
    {
        BoatFree(networkConfig.host_name);
    }

    if (networkConfig.chain_id != NULL)
    {
        BoatFree(networkConfig.chain_id);
    }

    if (networkConfig.org_id != NULL)
    {
        BoatFree(networkConfig.org_id);
    }

    return index;
}

int main(int argc, char *argv[])
{
    BOAT_RESULT           result  = BOAT_SUCCESS;
    BoatChainmakerTx      tx_ptr;
    BoatResponseData      response_data;
    boat_try_declare;

    /* step-1: Boat SDK initialization */
    BoatIotSdkInit();

    /* step-2: prepare keypair and network*/
    result = chainmaker_create_keypair();
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create keypair failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }

    result = chainmaker_create_network();
    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "chainmaker create network failed.");
        boat_throw(result, chainmaker_demo_catch);;
    }
    BoatLog(BOAT_LOG_CRITICAL, "wallet init keypair_index =%d, network_index = %d",keypair_index, network_index);

    g_chaninmaker_wallet_ptr = BoatChainmakerWalletInit(keypair_index, network_index);
    if (g_chaninmaker_wallet_ptr == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerWalletInit failed.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, chainmaker_demo_catch);
    }

    /* step-3: Chainmaker transaction structure initialization */
    result = BoatChainmakerTxInit(g_chaninmaker_wallet_ptr, &tx_ptr, 0);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerTxInit failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-4: add invoke parameters */
    result = BoatChainmakerAddTxParam(&tx_ptr, 6, "time", "1669200547", "file_hash", "a3f485ea14d946168d2a448adea8d47123", "file_name", "file_name", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-5: execute invoke command */
    result = BoatChainmakerContractInvoke(&tx_ptr, "save","fact", true, &response_data); 
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractInvoke() failed.");
        BoatChainmakerResponseFree(&response_data);
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d, message = %s", response_data.code, response_data.message);
    BoatChainmakerResponseFree(&response_data);

    // /* step-6: add query parameters*/
    BoatSleep(2);
    result = BoatChainmakerAddTxParam(&tx_ptr, 2, "file_hash", "a3f485ea14d946168d2a448adea8d47123", NULL);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerAddTxParam() failed.");
        boat_throw(result, chainmaker_demo_catch);
    }

    /* step-7: execute query command*/
    result = BoatChainmakerContractQuery(&tx_ptr, "find_by_file_hash","fact", &response_data);
    if (result != BOAT_SUCCESS)
    {
        BoatLog(BOAT_LOG_CRITICAL, "BoatChainmakerContractQuery() failed.");
        BoatChainmakerResponseFree(&response_data);
        boat_throw(result, chainmaker_demo_catch);
    }
    BoatLog(BOAT_LOG_CRITICAL, "response code = %d,  message = %s,  contract_result = %s, tx_id = %s", 
            response_data.code, response_data.message, response_data.contract_result.payload.field_ptr, response_data.tx_id);
     BoatChainmakerResponseFree(&response_data);

    boat_catch(chainmaker_demo_catch)
    {
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
    }   

    if (g_chaninmaker_wallet_ptr != NULL)
    {
        BoatChainmakerWalletDeInit(g_chaninmaker_wallet_ptr);
    }

    /* step-8: Boat SDK Deinitialization */
    BoatIotSdkDeInit();

    return 0;
}

