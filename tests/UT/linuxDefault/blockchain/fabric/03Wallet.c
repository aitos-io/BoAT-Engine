/*
 * @Description:
 * @Author: aitos
 * @Date: 2022-09-04 11:28:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-09-13 14:37:50
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
#include "tcase_fabric.h"
#include "boatosal.h"

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
 #ifndef TEST_FABRIC_NODE_URL
 #define TEST_FABRIC_NODE_URL "127.0.0.1"
 #endif
static BoatHlfabricWallet *g_fabric_wallet_ptr;
const BCHAR *fabric_client_demokey = "-----BEGIN PRIVATE KEY-----\r\n"
									"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgQelGsrrI1bSHfhn7\r\n"
									"cE8eZhW0vcN/zxwQSSvGGjC//XWhRANCAAQF5s7NQ68grY1w8QqWst8nAYhGKIZm\r\n"
									"lWAuUJEex+bm1kZmxfWjGTRTTxBxiW0nEHOoKh3180brsVtF4O/SqYRm\r\n"
									"-----END PRIVATE KEY-----\r\n";

const BCHAR *fabric_client_democert = "-----BEGIN CERTIFICATE-----\r\n"
									"MIICKzCCAdGgAwIBAgIRALK+tyjVhClDWyt/ggWKgEQwCgYIKoZIzj0EAwIwczEL\r\n"
									"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\r\n"
									"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\r\n"
									"Lm9yZzEuZXhhbXBsZS5jb20wHhcNMjMwMjAyMDIzMjAwWhcNMzMwMTMwMDIzMjAw\r\n"
									"WjBsMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\r\n"
									"U2FuIEZyYW5jaXNjbzEPMA0GA1UECxMGY2xpZW50MR8wHQYDVQQDDBZVc2VyMUBv\r\n"
									"cmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEBebOzUOv\r\n"
									"IK2NcPEKlrLfJwGIRiiGZpVgLlCRHsfm5tZGZsX1oxk0U08QcYltJxBzqCod9fNG\r\n"
									"67FbReDv0qmEZqNNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYD\r\n"
									"VR0jBCQwIoAgLlMBAU/4icXPJc4Nj7zn/7wWiuLPlUHmBVZ/xvPochEwCgYIKoZI\r\n"
									"zj0EAwIDSAAwRQIhAJy4GMtVyuLUcEBHNXIJUT5p4VCd8qDHBOuT24oi3/k7AiB2\r\n"
									"Us4Uu9EgwXe5ZtogeUvY2t+T1clQkgD6O9QqmcYEVA==\r\n"
									"-----END CERTIFICATE-----\r\n";

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)

const BCHAR *fabric_client_tls_prikey = "-----BEGIN PRIVATE KEY-----\r\n"
										"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgJi3O2cMWUMaMqT1P\r\n"
										"PnZT+QW3o2UMsoZSRC4kRlpprvOhRANCAAQDc5l684LtyVJgyTshzXGASAVhHOLq\r\n"
										"HGQmvm/kuJhEB7drImUunhSr1or8+7KdANPoHF9k+yzYNBCb5rYc/rh2\r\n"
										"-----END PRIVATE KEY-----\r\n";

const BCHAR *fabric_client_tls_cert = "-----BEGIN CERTIFICATE-----\r\n"
									"MIICOzCCAeGgAwIBAgIQDO1oaMNMWjFlNY0PJ1RyQTAKBggqhkjOPQQDAjB2MQsw\r\n"
									"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\r\n"
									"YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\r\n"
									"Y2Eub3JnMS5leGFtcGxlLmNvbTAeFw0yMzAyMDIwMjMyMDBaFw0zMzAxMzAwMjMy\r\n"
									"MDBaMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\r\n"
									"Ew1TYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZVc2VyMUBvcmcxLmV4YW1wbGUuY29t\r\n"
									"MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEA3OZevOC7clSYMk7Ic1xgEgFYRzi\r\n"
									"6hxkJr5v5LiYRAe3ayJlLp4Uq9aK/PuynQDT6BxfZPss2DQQm+a2HP64dqNsMGow\r\n"
									"DgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAM\r\n"
									"BgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIDL1YKxMZjB/zgNqXKzD/lQvL2B1VkJJ\r\n"
									"fLTWbAniSmArMAoGCCqGSM49BAMCA0gAMEUCIQCf16/NCH0BX9Ka0VmUII8aObS/\r\n"
									"AOUE1+5zN8O7UIE7PgIgBsx3KOhRlg05ThrbtuESEeukgMekruWo2XdfTsI4DVM=\r\n"
									"-----END CERTIFICATE-----\r\n";

#endif

const BCHAR *fabric_org1_tlsCert = "-----BEGIN CERTIFICATE-----\r\n"
								"MIICWDCCAf6gAwIBAgIRAIq8VU7QVaMbCmO4XtlPcGwwCgYIKoZIzj0EAwIwdjEL\r\n"
								"MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\r\n"
								"cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHzAdBgNVBAMTFnRs\r\n"
								"c2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMjMwMjAyMDIzMjAwWhcNMzMwMTMwMDIz\r\n"
								"MjAwWjB2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UE\r\n"
								"BxMNU2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEfMB0G\r\n"
								"A1UEAxMWdGxzY2Eub3JnMS5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49\r\n"
								"AwEHA0IABBsBufHeiF3lhn73kMNSb4OgPtAvkOhF6Km46DGjJiJhkeFpB7Lz0/AO\r\n"
								"1eBX4vVXyPXmGfz/r/1jW+t9kVPsPdKjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNV\r\n"
								"HSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNV\r\n"
								"HQ4EIgQgMvVgrExmMH/OA2pcrMP+VC8vYHVWQkl8tNZsCeJKYCswCgYIKoZIzj0E\r\n"
								"AwIDSAAwRQIhAPRrhZgY8AZwXzLXifFhw6DfLB1kYTPykKJCZDAkl8EsAiBrZLND\r\n"
								"i/FJpYxm3NdeHxmpe98bTccpksZovAd9WqWypA==\r\n"
								"-----END CERTIFICATE-----\r\n";

const BCHAR *fabric_org2_tlsCert = "-----BEGIN CERTIFICATE-----\r\n"
								"MIICVzCCAf2gAwIBAgIQD7y+U4BnohOSCBonGS+jOTAKBggqhkjOPQQDAjB2MQsw\r\n"
								"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\r\n"
								"YW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEfMB0GA1UEAxMWdGxz\r\n"
								"Y2Eub3JnMi5leGFtcGxlLmNvbTAeFw0yMzAyMDIwMjMyMDBaFw0zMzAxMzAwMjMy\r\n"
								"MDBaMHYxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQH\r\n"
								"Ew1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcyLmV4YW1wbGUuY29tMR8wHQYD\r\n"
								"VQQDExZ0bHNjYS5vcmcyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\r\n"
								"AQcDQgAEtG6QUB0YbZuDSJHkAa+KSnVtAVvO3WZBExabZShKxcA2yyn0zuXI+9uQ\r\n"
								"1SZdFMEZw9mCL2+MDyagvPOWeCTnlKNtMGswDgYDVR0PAQH/BAQDAgGmMB0GA1Ud\r\n"
								"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAPBgNVHRMBAf8EBTADAQH/MCkGA1Ud\r\n"
								"DgQiBCBnrIPU8XwE+SVbbV/wTZolaX1PBv5vXAtonEJi6wHYEjAKBggqhkjOPQQD\r\n"
								"AgNIADBFAiEAxxDBxlSt6fHzZhx0Xj/LW2UrGNY17IPBNxd27W8qPrcCIA7LBgC+\r\n"
								"6Kkh21m2a1TTnQ8FoSgaTv6D8/WHcopSL3CG\r\n"
								"-----END CERTIFICATE-----\r\n";

const BCHAR *fabric_order_tlsCert = "-----BEGIN CERTIFICATE-----\r\n"
									"MIICQzCCAemgAwIBAgIQeqDTsZGBmA/T0auvq8uZ4zAKBggqhkjOPQQDAjBsMQsw\r\n"
									"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\r\n"
									"YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xGjAYBgNVBAMTEXRsc2NhLmV4\r\n"
									"YW1wbGUuY29tMB4XDTIzMDIwMjAyMzIwMFoXDTMzMDEzMDAyMzIwMFowbDELMAkG\r\n"
									"A1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFu\r\n"
									"Y2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRowGAYDVQQDExF0bHNjYS5leGFt\r\n"
									"cGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABMiaV0Nzr2/N83pHiPdH\r\n"
									"6jOp1HftD+vGPfSx9fXzjH4ik3Wl0uRzBK+nRM6zbblFe9Bp6Ndr1MoOIPjzEgFh\r\n"
									"YCCjbTBrMA4GA1UdDwEB/wQEAwIBpjAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYB\r\n"
									"BQUHAwEwDwYDVR0TAQH/BAUwAwEB/zApBgNVHQ4EIgQgwrK+n/KzIvMIvlMbDzKV\r\n"
									"sB2eOXdJ/Y5Qqu/cEiZbY9UwCgYIKoZIzj0EAwIDSAAwRQIhAL9+6AuCgHwXC1f9\r\n"
									"Um9uM4yvBleBzHKmxswP0CivSbdpAiBQkm/iP66IYMElwJN8UIacoQY/yAuTTiBW\r\n"
									"8M0cQoS49Q==\r\n"
									"-----END CERTIFICATE-----\r\n";

/**
 * native demo key
 */
const BCHAR *native_demoKey = "0xf1395a1fc3f74f0c867b61292e28e0f6cc98a095535fd6bf04e4169ebc047e61";
const BUINT8 pubkey256R1[] = {0xEC, 0xBB, 0x65, 0xC5, 0xAF, 0x1E, 0x6E, 0xD9, 0x80, 0x16, 0x32, 0xFE, 0x30, 0x18, 0x7D, 0xEA, 0x2B, 0x31, 0x83, 0x6B, 0xF4, 0x2E, 0xB2, 0xD2, 0xF4, 0x0B, 0x78, 0x95, 0xC1, 0x97, 0xBB, 0xDC, 0x0C, 0xA0, 0xD7, 0x31, 0x63, 0x80, 0xAB, 0x59, 0xD4, 0x6A, 0xEE, 0xDD, 0x55, 0xD9, 0xEB, 0xEA, 0x2C, 0x44, 0xB0, 0xFC, 0xE8, 0x50, 0xF7, 0xF8, 0xC6, 0x14, 0xCF, 0x8E, 0xAF, 0x09, 0x37, 0xAF};
BUINT8 binFormatKey[32] = {0};

/**
****************************************************************************************
* @brief:
*  This function check nodes configuration, the first nodes configuration must be same to another one;
* @param[in] nodesCfg1
*  the first nodes configuration
* @param[in] nodesCfg2
*  the second nodes configuration
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_checkNetworkNodesConfg(BoatHlfabricNodesCfg nodesCfg1, BoatHlfabricNodesCfg nodesCfg2)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* check two nodes configuration's endorserLayoutNum*/
    if (nodesCfg1.endorserLayoutNum != nodesCfg2.endorserLayoutNum)
    {
        BoatLog(BOAT_LOG_NORMAL, " endorserLayoutNum ERR ");
        return BOAT_ERROR;
    }
    /****************** check the content ***************************/
    /* check every layout */
    for (size_t i = 0; i < nodesCfg1.endorserLayoutNum; i++)
    {
        /* check layoutcfg is NULL or not */
        if ((NULL == nodesCfg1.layoutCfg) || (NULL == nodesCfg2.layoutCfg))
        {
            BoatLog(BOAT_LOG_NORMAL, " layoutCfg NULL ");
            return BOAT_ERROR;
        }
        /* check every endorserGroupNum*/
        if (nodesCfg1.layoutCfg[i].endorserGroupNum != nodesCfg2.layoutCfg[i].endorserGroupNum)
        {
            BoatLog(BOAT_LOG_NORMAL, " endorserGroupNum err ");
            return BOAT_ERROR;
        }
        for (size_t j = 0; j < nodesCfg1.layoutCfg[i].endorserGroupNum; j++)
        {
            /* check every groupCfg is NULL or not */
            if ((NULL == nodesCfg1.layoutCfg[i].groupCfg) || (NULL == nodesCfg2.layoutCfg[i].groupCfg))
            {
                BoatLog(BOAT_LOG_NORMAL, " groupCfg NULL ");
                return BOAT_ERROR;
            }
            /* check every endorserNumber */
            if (nodesCfg1.layoutCfg[i].groupCfg[j].endorserNumber != nodesCfg2.layoutCfg[i].groupCfg[j].endorserNumber)
            {
                BoatLog(BOAT_LOG_NORMAL, " endorserNumber err ");
                return BOAT_ERROR;
            }
            /* check every endorser is NULL or not */
            if ((NULL == nodesCfg1.layoutCfg[i].groupCfg[j].endorser) || (NULL == nodesCfg2.layoutCfg[i].groupCfg[j].endorser))
            {
                BoatLog(BOAT_LOG_NORMAL, " endorser NULL ");
                return BOAT_ERROR;
            }
            /* check every quantities is same or not */
            if (nodesCfg1.layoutCfg[i].groupCfg[j].quantities != nodesCfg2.layoutCfg[i].groupCfg[j].quantities)
            {
                BoatLog(BOAT_LOG_NORMAL, " quantities err ");
                return BOAT_ERROR;
            }
            /* check every tlsOrgCertContent*/
            if (!strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodesCfg2.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content) == 0)
            {
                BoatLog(BOAT_LOG_NORMAL, " tlsOrgCertContent content err ");
                return BOAT_ERROR;
            }
            /* check endorser content */
            for (size_t k = 0; k < nodesCfg1.layoutCfg[i].groupCfg[j].endorserNumber; k++)
            {
                /* check every hostname */
                if (strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].endorser[k].hostName, nodesCfg2.layoutCfg[i].groupCfg[j].endorser[k].hostName) != 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, " hostName err ");
                    return BOAT_ERROR;
                }
                /* check every url */
                if (strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl, nodesCfg2.layoutCfg[i].groupCfg[j].endorser[k].nodeUrl) != 0)
                {
                    BoatLog(BOAT_LOG_NORMAL, " nodeurl err ");
                    return BOAT_ERROR;
                }
            }
        }
    }
    /****************** check every order **********************/
    /* check endorserNumber of orderCfg */
    if (nodesCfg1.orderCfg.endorserNumber != nodesCfg2.orderCfg.endorserNumber)
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg endorserNumber err ");
        return BOAT_ERROR;
    }
    /* check endorser of orderCfg is NULL or not */
    if ((NULL == nodesCfg1.orderCfg.endorser) || (NULL == nodesCfg2.orderCfg.endorser))
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg endorser NULL ");
        return BOAT_ERROR;
    }
    /* check tlsOrgCertContent of orderCfg */
    if (strcmp(nodesCfg1.orderCfg.tlsOrgCertContent.content, nodesCfg2.orderCfg.tlsOrgCertContent.content) != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, " ordercfg tlsOrgCertContent content err ");
        return BOAT_ERROR;
    }

    for (size_t i = 0; i < nodesCfg1.orderCfg.endorserNumber; i++)
    {
        /* check every hostname of orderCfg*/
        if (strcmp(nodesCfg1.orderCfg.endorser[i].hostName, nodesCfg2.orderCfg.endorser[i].hostName) != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " ordercfg hostname err ");
            return BOAT_ERROR;
        }
        /* check every url of orderCfg */
        if (strcmp(nodesCfg1.orderCfg.endorser[i].nodeUrl, nodesCfg2.orderCfg.endorser[i].nodeUrl) != 0)
        {
            BoatLog(BOAT_LOG_NORMAL, " ordercfg nodeurl err ");
            return BOAT_ERROR;
        }
    }

    return result;
}
/**
****************************************************************************************
* @brief:
*  This function check network data reading by index or list.
* @param[in] networkData
*  network data read from memory
* @param[in] networkCfg
*  network configuration to creat the network data
* @param[in] expectedIndex
*  index of created network
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT fabric_createNetwork(BBOOL is_onetime, BUINT8* networkIndex, BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result;
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = (BoatHlfabricNodeLayoutCfg *)BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = (BoatHlfabricNodeGroupCfg *)BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER0_PORT)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = (BCHAR *)BoatMalloc(strlen(ORG1_PEER0_HOSTNAME)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = (BCHAR *)BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG1_PEER1_PORT)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = (BCHAR *)BoatMalloc(strlen(ORG1_PEER1_HOSTNAME)+1);

    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, TEST_FABRIC_NODE_URL);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG1_PEER0_PORT);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, ORG1_PEER0_HOSTNAME);

    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, TEST_FABRIC_NODE_URL);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG1_PEER1_PORT);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, ORG1_PEER1_HOSTNAME);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER0_PORT)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = (BCHAR *)BoatMalloc(strlen(ORG2_PEER0_HOSTNAME)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = (BCHAR *)BoatMalloc(strlen(TEST_FABRIC_NODE_URL) + strlen(ORG2_PEER1_PORT)+1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = (BCHAR *)BoatMalloc(strlen(ORG2_PEER1_HOSTNAME)+1);
  
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, TEST_FABRIC_NODE_URL);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG2_PEER0_PORT);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, ORG2_PEER0_HOSTNAME);

    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, TEST_FABRIC_NODE_URL);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORG2_PEER1_PORT);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, ORG2_PEER1_HOSTNAME);

    // set orderer info
    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = (BCHAR *)BoatMalloc(strlen(ORDER_HOSTNAME)+1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(TEST_FABRIC_NODE_URL)+strlen(ORDER_PORT)+1);

    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, TEST_FABRIC_NODE_URL);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl+strlen(TEST_FABRIC_NODE_URL), ORDER_PORT);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, ORDER_HOSTNAME);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    networkConfig->accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
    strcpy((BCHAR *)networkConfig->accountClientTlsPrikey.value, fabric_client_tls_prikey);
    networkConfig->accountClientTlsCert.length = strlen(fabric_client_tls_cert);
    strcpy((BCHAR *)networkConfig->accountClientTlsCert.content, fabric_client_tls_cert);
#endif
    /* create ciat wallet */
    if (is_onetime)
    {
        result = BoatHlfabricNetworkCreate(networkConfig, BOAT_STORE_TYPE_RAM);
    }
    else
    {
        result = BoatHlfabricNetworkCreate(networkConfig, BOAT_STORE_TYPE_FLASH);
    }

    if (result < 0)
    {
        BoatLog(BOAT_LOG_CRITICAL, "create one-time wallet failed.");
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *networkIndex = result;
    return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT
fabric_checkNetworkData(BoatHlfabricNetworkData networkData, BoatHlfabricNetworkConfig networkCfg, BUINT8 expectedIndex)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    /* index of networkData must be same to expectedIndex */
    if (networkData.index != expectedIndex)
    {
        BoatLog(BOAT_LOG_NORMAL, " index err");
        return BOAT_ERROR;
    }
    /******************* check account cert ***************************/
    /* check account cert content */
    if (strcmp(networkData.accountCertContent.content, networkCfg.accountCertContent.content) != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, " accountCertContent err");
        return BOAT_ERROR;
    }
    /* check nodesCfg */
    result = fabric_checkNetworkNodesConfg(networkData.nodesCfg, networkCfg.nodesCfg);
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get keypair config ;
* @param[in/out] *keypair_config
* @return
*   This function has no return value;
****************************************************************************************
*/

BOAT_RESULT fabric_createKeypair(BCHAR * keypairName, BBOOL is_onetime, BUINT8* keypairIndex_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    //BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> keypair format: external injection[native].");
    // keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    // keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    // keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    // UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    // keypair_config.prikey_content.field_ptr = binFormatKey;
    // keypair_config.prikey_content.field_len = 32;

    /* keypair_config value assignment */
    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
    keypair_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_demokey;
    keypair_config.prikey_content.field_len = strlen(fabric_client_demokey) + 1; // length contain terminator

    /* create cita keypair */
    if (is_onetime)
    {
        result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
    }
    else
    {
        result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
    }

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypairIndex_ptr = result;
    
    return BOAT_SUCCESS;
}

BOAT_RESULT fabric_createKeypair_native(BCHAR * keypairName, BBOOL is_onetime, BUINT8* keypairIndex_ptr)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};
    BUINT8 binFormatKey[32]           = {0};

    (void)binFormatKey; //avoid warning

    keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
    keypair_config.prikey_format  = BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE;
    keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
    UtilityHexToBin(binFormatKey, 32, native_demoKey, TRIMBIN_TRIM_NO, BOAT_FALSE);
    keypair_config.prikey_content.field_ptr = binFormatKey;
    keypair_config.prikey_content.field_len = 32;

    /* create cita keypair */
    if (is_onetime)
    {
        result = BoatKeypairCreate(&keypair_config, keypairName, BOAT_STORE_TYPE_RAM);
    }
    else
    {
        result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
    }

    if (result < 0)
    {
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    *keypairIndex_ptr = result;
    
    return BOAT_SUCCESS;
}


/**
****************************************************************************************
* @brief:
*  This function test case : create onetime keypair success
* @return
****************************************************************************************
*/
START_TEST(test_003CreateWallet_0001CreateOneTimeWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatHlfabricNetworkConfig networkConfig;
    BoatIotSdkInit();

    rtnVal = fabric_createKeypair_native("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);

    rtnVal = fabric_createNetwork(BOAT_TRUE, &networkIndex, &networkConfig);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_fabric_wallet_ptr, NULL);

    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.keypair_index, 0);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_fabric_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_fabric_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey256R1, sizeof(pubkey256R1)), 0);

    rtnVal = fabric_checkNetworkData(g_fabric_wallet_ptr->network_info, networkConfig, 0);

    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);
 
    /* free configuration struct */
    fabricWalletConfigFree(networkConfig);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();
    BoatHlfabricNetworkConfig networkConfig;

    rtnVal = fabric_createNetwork(BOAT_TRUE, &networkIndex, &networkConfig);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 0);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_fabric_wallet_ptr, NULL);

    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);

    /* free configuration struct */
    fabricWalletConfigFree(networkConfig);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = fabric_createKeypair_native("keypairOnetime", BOAT_TRUE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 0);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_fabric_wallet_ptr, NULL);

    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);

    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0004CreatePersistWalletSuccess) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatHlfabricNetworkConfig networkConfig;
    BoatIotSdkInit();

    rtnVal = fabric_createKeypair_native("keypairOnetime", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);

    rtnVal = fabric_createNetwork(BOAT_FALSE, &networkIndex, &networkConfig);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_ne(g_fabric_wallet_ptr, NULL);

    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.keypair_index, 1);
    /* check keypair name in the struct */
    ck_assert_int_eq(strcmp(g_fabric_wallet_ptr->account_info.prikeyCtx.keypair_name, "keypairOnetime"), 0);
    /* check keypair format in the struct ,must be the same as in the config */
    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.prikey_format, BOAT_KEYPAIR_PRIKEY_FORMAT_NATIVE);
    /* check keypair type in the struct ,must be the same as in the config*/
    ck_assert_int_eq(g_fabric_wallet_ptr->account_info.prikeyCtx.prikey_type, BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1);
    // /* check public key in the struct */
    ck_assert_int_eq(memcmp(g_fabric_wallet_ptr->account_info.prikeyCtx.pubkey_content, pubkey256R1, sizeof(pubkey256R1)), 0);

    rtnVal = fabric_checkNetworkData(g_fabric_wallet_ptr->network_info, networkConfig, 1);
    /* free configuration struct */
    fabricWalletConfigFree(networkConfig);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);

    /* delete this network*/
    BoATHlfabricNetworkDelete(1);
    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0005CreatePersistWalletFailureWithoutKeypair) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatHlfabricNetworkConfig networkConfig;
    BoatIotSdkInit();

    rtnVal = fabric_createNetwork(BOAT_FALSE, &networkIndex, &networkConfig);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(networkIndex, 1);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_fabric_wallet_ptr, NULL);

    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);

    /* free configuration struct */
    fabricWalletConfigFree(networkConfig);
    BoATHlfabricNetworkDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

START_TEST(test_003CreateWallet_0006CreatePersistWalletFailureWithoutNetwork) 
{
    BSINT32 rtnVal;
    BUINT8 keypairIndex = 0;
    BUINT8 networkIndex = 0;
    BoatIotSdkInit();

    rtnVal = fabric_createKeypair_native("keypairOnetime", BOAT_FALSE, &keypairIndex);
    ck_assert_int_eq(rtnVal, 0);
    ck_assert_int_eq(keypairIndex, 1);
   
    g_fabric_wallet_ptr = BoatHlfabricWalletInit(keypairIndex, networkIndex);
    ck_assert_ptr_eq(g_fabric_wallet_ptr, NULL);
    BoatHlfabricWalletDeInit(g_fabric_wallet_ptr);

    BoATIotKeypairDelete(1);
    BoatIotSdkDeInit();
}
END_TEST

Suite *make_fabricWalletTest_suite(void)
{
    /* Create Suite */
    Suite *s_walletInit = suite_create("walletInit");

    /* Create test cases */
    TCase *tc_walletInit_api = tcase_create("walletInit_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_walletInit, tc_walletInit_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0001CreateOneTimeWalletSuccess);  
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0002CreateOneTimeWalletFailureWithoutKeypair); 
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0003CreateOneTimeWalletFailureWithoutNetwork);

    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0004CreatePersistWalletSuccess);
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0005CreatePersistWalletFailureWithoutKeypair); 
    tcase_add_test(tc_walletInit_api, test_003CreateWallet_0006CreatePersistWalletFailureWithoutNetwork);
    return s_walletInit;
}