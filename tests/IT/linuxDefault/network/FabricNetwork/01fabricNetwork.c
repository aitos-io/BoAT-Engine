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
#include "tcase_network.h"
#include "boatiotsdk.h"
#include "boatlog.h"
#include "boatosal.h"

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

const BCHAR *fabric_demo_order1_url = "127.0.0.0:7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = "127.0.0.0:7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = "127.0.0.0:8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = "127.0.0.0:9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = "127.0.0.0:10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";

const BCHAR *fabric2_demo_order1_url = "127.0.0.1:7050";
const BCHAR *fabric2_demo_order1_hostName = "orderer.example1.com";

const BCHAR *fabric2_demo_endorser_peer0Org1_url = "127.0.0.1:7051";
const BCHAR *fabric2_demo_endorser_peer0Org1_hostName = "peer0.org1.example1.com";
const BCHAR *fabric2_demo_endorser_peer1Org1_url = "127.0.0.1:8051";
const BCHAR *fabric2_demo_endorser_peer1Org1_hostName = "peer1.org1.example1.com";

const BCHAR *fabric2_demo_endorser_peer0Org2_url = "127.0.0.1:9051";
const BCHAR *fabric2_demo_endorser_peer0Org2_hostName = "peer0.org2.example1.com";
const BCHAR *fabric2_demo_endorser_peer1Org2_url = "127.0.0.1:10051";
const BCHAR *fabric2_demo_endorser_peer1Org2_hostName = "peer1.org2.example1.com";

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric_demo_endorser_peer0Org1_hostName);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric_demo_endorser_peer1Org1_hostName);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org2_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric_demo_endorser_peer0Org2_hostName);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric_demo_endorser_peer1Org2_hostName);

    // set orderer info
    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url) + 1);
    memset(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric_demo_order1_url) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, fabric_demo_order1_url);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    networkConfig->accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
    strcpy((BCHAR *)networkConfig->accountClientTlsPrikey.value, fabric_client_tls_prikey);
    networkConfig->accountClientTlsCert.length = strlen(fabric_client_tls_cert);
    strcpy((BCHAR *)networkConfig->accountClientTlsCert.content, fabric_client_tls_cert);
#endif
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg2(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric2_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric2_demo_endorser_peer0Org1_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric2_demo_endorser_peer1Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric2_demo_endorser_peer1Org1_hostName) + 1);
    memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric2_demo_endorser_peer0Org1_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric2_demo_endorser_peer0Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric2_demo_endorser_peer0Org1_hostName);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric2_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric2_demo_endorser_peer1Org1_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric2_demo_endorser_peer1Org1_hostName);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric2_demo_endorser_peer0Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric2_demo_endorser_peer0Org2_hostName) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric2_demo_endorser_peer1Org2_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric2_demo_endorser_peer1Org2_hostName) + 1);
    memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric2_demo_endorser_peer0Org2_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric2_demo_endorser_peer0Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric2_demo_endorser_peer0Org2_hostName);
    memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, 0, strlen(fabric2_demo_endorser_peer1Org2_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric2_demo_endorser_peer1Org2_url);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric2_demo_endorser_peer1Org2_hostName);

    // set orderer info
    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric2_demo_order1_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric2_demo_order1_url) + 1);
    memset(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric2_demo_order1_url) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, fabric2_demo_order1_url);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, fabric2_demo_order1_hostName);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
    networkConfig->accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
    strcpy((BCHAR *)networkConfig->accountClientTlsPrikey.value, fabric_client_tls_prikey);
    networkConfig->accountClientTlsCert.length = strlen(fabric_client_tls_cert);
    strcpy((BCHAR *)networkConfig->accountClientTlsCert.content, fabric_client_tls_cert);
#endif
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric;
*  set layoutCfg to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg_layoutCfg_NULL(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric;
*  set group to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg_group_NULL(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric;
*  set endorser to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg_endorser_NULL(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert, strlen(fabric_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric;
*  set nodeurl to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg_nodeurl_NULL(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert, strlen(fabric_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = NULL;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName) + 1);
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

/**
****************************************************************************************
* @brief:
*  This function get the network configuration of fabric;
*  set hostname to NULL;
* @param[in/out] *networkConfig
* @return
*  This function returns BOAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg_hostname_NULL(BoatHlfabricNetworkConfig *networkConfig)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    if (NULL == networkConfig)
    {
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    // set cert context
    networkConfig->accountCertContent.length = strlen(fabric_client_democert) + 1;
    strcpy(networkConfig->accountCertContent.content, fabric_client_democert);

    networkConfig->nodesCfg.endorserLayoutNum = 1;
    networkConfig->nodesCfg.layoutCfg = BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    memcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert, strlen(fabric_org1_tlsCert));

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = NULL;
    networkConfig->nodesCfg.orderCfg.endorserNumber = 0;
    networkConfig->nodesCfg.orderCfg.endorser = NULL;
    return result;
}

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
            if (strcmp(nodesCfg1.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, nodesCfg2.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content) != 0)
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
*  This function test case: creat onetime network success
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg, 0);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHlfabricNetworkDelete(0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network success
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0002CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM,
        this is the first persist network ,index must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 1);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg, 1);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHlfabricNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserLayoutNum in the configuration is 0 ;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0003CreatetNetworkFail_NodeCfg_endorserLayoutNum_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change the endorserLayoutNum to 0*/
    networkCfg.nodesCfg.endorserLayoutNum = 0;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  layoutCfg in the configuration is NULL ;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0004CreatetNetworkFail_NodeCfg_layoutCfg_NULL)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg_layoutCfg_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserGroupNum in the configuration is 0, this param must not be 0 ;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0005CreatetNetworkFail_NodeCfg_endorserGroupNum_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change endorserGroupNum to 0*/
    networkCfg.nodesCfg.layoutCfg[0].endorserGroupNum = 0;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  group in the configuration is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0006CreatetNetworkFail_NodeCfg_groupCfg_NULL)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg_group_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorserNumber = 0
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0007CreatetNetworkFail_NodeCfg_endorserNumber_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* change endorserNumber to 0 */
    networkCfg.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 0;
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  endorser is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0008CreatetNetworkFail_NodeCfg_endorser_NULL)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg_endorser_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  nodeUrl is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0009CreatetNetworkFail_NodeCfg_nodeUrl_NULL)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg_nodeurl_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  hostname is NULL
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0010CreatetNetworkFail_NodeCfg_hostname_NULL)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg_hostname_NULL(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat persist network fail;
*  quantities is bigger than endorserNumber;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0011CreatetNetworkFail_NodeCfg_quantities_ERR)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* change quantities to 3 ,is bigger than the endorserNumber in the same group*/
    networkCfg.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 3;
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_NODECFG_ERR);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete onetime network;
*  create a onetime network ,then delete it ;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0012DeleteOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* delete the onetime network*/
    rtnVal = BoATHlfabricNetworkDelete(0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get network data by index , index = 0 */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 0);
    /* check result; There are no network , so return BOAT_ERROR_NETWORK_INEXISTENCE */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete persist network;
*  create a persist network ,then delete it ;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0013DeletePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network, index of persist network is from 1 to BOAT_MAX_NETWORK_NUM;
        this is the first persist network, index must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* delete the persist network*/
    rtnVal = BoATHlfabricNetworkDelete(1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* get network data by index , index = 1 */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 1);
    /* check result; There are no network , so return BOAT_ERROR_NETWORK_INEXISTENCE */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete network using index from 0 to BOAT_MAX_NETWORK_NUM;
*  get network by index return error because there have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0014DeleteNetwork_NoNetwork)
{
    BSINT32 rtnVal;
    for (size_t i = 0; i <= BOAT_MAX_NETWORK_NUM; i++)
    {
        /* delete the  network*/
        rtnVal = BoATHlfabricNetworkDelete(i);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INEXISTENCE);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: delete network using index that is bigger than BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0015DeleteNetwork_IndexExceed)
{
    BSINT32 rtnVal;

    /* delete the network that index is bigger than BOAT_MAX_NETWORK_NUM;
       index of onetime network must be 0 and index of persist network is from
       1 to BOAT_MAX_NETWORK_NUM, so the index must be from 0 to BOAT_MAX_NETWORK_NUM*/
    rtnVal = BoATHlfabricNetworkDelete(BOAT_MAX_NETWORK_NUM + 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get keypair list that param is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0016GetNetworkList_ParamNULL)
{
    BSINT32 rtnVal;
    /* get network list that param is NULL*/
    rtnVal = BoATHlfabric_GetNetworkList(NULL);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get keypair list that have no network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0017GetNetworkList_NONetwork)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkContext networkList;

    /* get network list that param is NULL*/
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check content of list; there is no network, networkNum must be 0*/
    ck_assert_int_eq(networkList.networkNum, 0);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list;
*  create only a onetime network and get the list, check the content of list;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0018GetNetworkList_OnlyOnetimeNetwork)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkContext networkList;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /* get network list */
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /************* check content of network list******************/
    /* check networkNum in the list ,only a onetime network, networkNum must be 1*/
    ck_assert_int_eq(networkList.networkNum, 1);
    /* check index in the list, only a onetime network, index must be 0*/
    ck_assert_int_eq(networkList.networks[0].index, 0);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[0].accountCertContent.content, networkCfg.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = fabric_checkNetworkNodesConfg(networkList.networks[0].nodesCfg, networkCfg.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* delete the persist network*/
    rtnVal = BoATHlfabricNetworkDelete(0);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* free memory of list*/
    BoATHlfabric_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list;
*  create a onetime network and a persist network, then get the list, check the content of list;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0019GetNetworkList_OnetimeAndPersistNetwork)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg1;
    BoatHlfabricNetworkConfig networkCfg2;
    BoatHlfabricNetworkContext networkList;
    /*****************create onetime network *********************/
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0*/
    ck_assert_int_eq(rtnVal, 0);
    /************** create persist network ***********************/
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network, index of persist network is from 1 to BOAT_MAX_NETWORK_NUM,
       this is the first persist network, so index of persist must be 1*/
    ck_assert_int_eq(rtnVal, 1);
    /* get network list */
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /************* check content of network list******************/
    /* there are two network, a onetime network and a persist network;
       read persist network data firstly and then read onetime network*/
    /* check networkNum in the list , networkNum must be 2*/
    ck_assert_int_eq(networkList.networkNum, 2);
    /* check index in the list, networks[0] point to the persist network ,index in this array must be 1*/
    ck_assert_int_eq(networkList.networks[0].index, 1);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[0].accountCertContent.content, networkCfg2.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = fabric_checkNetworkNodesConfg(networkList.networks[0].nodesCfg, networkCfg2.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check index in the list, networks[1] point to the onetime network ,index in this array must be 0*/
    ck_assert_int_eq(networkList.networks[1].index, 0);
    /* check accountCertContent in the list ,must be same to configuration*/
    ck_assert_int_eq(strcmp(networkList.networks[1].accountCertContent.content, networkCfg1.accountCertContent.content), 0);
    /* check nodesCfg in the list,must be same to configuration */
    rtnVal = fabric_checkNetworkNodesConfg(networkList.networks[1].nodesCfg, networkCfg1.nodesCfg);
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);

    /* free configuration struct */
    fabricWalletConfigFree(networkCfg1);
    fabricWalletConfigFree(networkCfg2);
    /* delete the persist network*/
    BoATHlfabricNetworkDelete(0);
    BoATHlfabricNetworkDelete(1);
    /* free memory of list*/
    BoATHlfabric_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network by index that input param is NULL;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0020GetNetworkByIndex_ParamNULL)
{
    BSINT32 rtnVal;
    rtnVal = BoATHlfabric_GetNetworkByIndex(NULL, 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_COMMON_INVALID_ARGUMENT);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network by index that exceed to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0021GetNetworkByIndex_IndexExceed)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkData networkData;
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, BOAT_MAX_NETWORK_NUM + 1);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get onetime network data by index ;
*  create a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0022GetNetworkByIndex_Onetime)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg1;
    BoatHlfabricNetworkConfig networkCfg2;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of onetime network must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get onetime network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg1, 0);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg1);
    fabricWalletConfigFree(networkCfg2);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHlfabricNetworkDelete(0);
    BoATHlfabricNetworkDelete(1);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get persist network data by index ;
*  create a onetime network and a persist network;
* @return
****************************************************************************************
*/
START_TEST(test_001fabricNetwork_0023GetNetworkByIndex_Persist)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg1;
    BoatHlfabricNetworkConfig networkCfg2;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg1);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg1, BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg2(&networkCfg2);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg2, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of onetime network must be 1 */
    ck_assert_int_eq(rtnVal, 1);

    /* get onetime network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 1);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg2, 1);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg1);
    fabricWalletConfigFree(networkCfg2);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* delete this network*/
    BoATHlfabricNetworkDelete(0);
    BoATHlfabricNetworkDelete(1);
}
END_TEST

Suite *make_fabricNetworkIntfTest_suite(void)
{
    /* Create Suite */
    Suite *s_networkcreate = suite_create("networkCreat");

    /* Create test cases */
    TCase *tc_networkCreat_api = tcase_create("networkCreat_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_networkcreate, tc_networkCreat_api);
    /* Test cases are added to the test set */
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0002CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0003CreatetNetworkFail_NodeCfg_endorserLayoutNum_ERR);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0004CreatetNetworkFail_NodeCfg_layoutCfg_NULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0005CreatetNetworkFail_NodeCfg_endorserGroupNum_ERR);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0006CreatetNetworkFail_NodeCfg_groupCfg_NULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0007CreatetNetworkFail_NodeCfg_endorserNumber_ERR);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0008CreatetNetworkFail_NodeCfg_endorser_NULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0009CreatetNetworkFail_NodeCfg_nodeUrl_NULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0010CreatetNetworkFail_NodeCfg_hostname_NULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0011CreatetNetworkFail_NodeCfg_quantities_ERR);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0012DeleteOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0013DeletePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0014DeleteNetwork_NoNetwork);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0015DeleteNetwork_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0016GetNetworkList_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0017GetNetworkList_NONetwork);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0018GetNetworkList_OnlyOnetimeNetwork);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0019GetNetworkList_OnetimeAndPersistNetwork);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0020GetNetworkByIndex_ParamNULL);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0021GetNetworkByIndex_IndexExceed);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0022GetNetworkByIndex_Onetime);
    tcase_add_test(tc_networkCreat_api, test_001fabricNetwork_0023GetNetworkByIndex_Persist);

    return s_networkcreate;
}