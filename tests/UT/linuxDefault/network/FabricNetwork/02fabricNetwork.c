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

const BCHAR *fabric_demo_order1_url[6] = {"127.0.0.0:7050",
                                          "127.0.0.1:7050",
                                          "127.0.0.2:7050",
                                          "127.0.0.3:7050",
                                          "127.0.0.4:7050",
                                          "127.0.0.5:7050"};
const BCHAR *fabric_demo_order1_hostName[6] = {"orderer.example0.com",
                                               "orderer.example1.com",
                                               "orderer.example2.com",
                                               "orderer.example3.com",
                                               "orderer.example4.com",
                                               "orderer.example5.com"};
const BCHAR *fabric_demo_endorser_peer0Org1_url[6] = {
    "127.0.0.0:7051",
    "127.0.0.1:7051",
    "127.0.0.2:7051",
    "127.0.0.3:7051",
    "127.0.0.4:7051",
    "127.0.0.5:7051"};

const BCHAR *fabric_demo_endorser_peer0Org1_hostName[6] = {
    "peer0.org1.example0.com",
    "peer0.org1.example1.com",
    "peer0.org1.example2.com",
    "peer0.org1.example3.com",
    "peer0.org1.example4.com",
    "peer0.org1.example5.com"};
const BCHAR *fabric_demo_endorser_peer1Org1_url[6] = {
    "127.0.0.0:8051",
    "127.0.0.1:8051",
    "127.0.0.2:8051",
    "127.0.0.3:8051",
    "127.0.0.4:8051",
    "127.0.0.5:8051"};
const BCHAR *fabric_demo_endorser_peer1Org1_hostName[6] = {
    "peer1.org1.example0.com",
    "peer1.org1.example1.com",
    "peer1.org1.example2.com",
    "peer1.org1.example3.com",
    "peer1.org1.example4.com",
    "peer1.org1.example5.com"};
const BCHAR *fabric_demo_endorser_peer0Org2_url[6] = {
    "127.0.0.0:9051",
    "127.0.0.1:9051",
    "127.0.0.2:9051",
    "127.0.0.3:9051",
    "127.0.0.4:9051",
    "127.0.0.5:9051"};
const BCHAR *fabric_demo_endorser_peer0Org2_hostName[6] = {
    "peer0.org2.example0.com",
    "peer0.org2.example1.com",
    "peer0.org2.example2.com",
    "peer0.org2.example3.com",
    "peer0.org2.example4.com",
    "peer0.org2.example5.com"};
const BCHAR *fabric_demo_endorser_peer1Org2_url[6] = {
    "127.0.0.0:10051",
    "127.0.0.1:10051",
    "127.0.0.2:10051",
    "127.0.0.3:10051",
    "127.0.0.4:10051",
    "127.0.0.5:10051"};
const BCHAR *fabric_demo_endorser_peer1Org2_hostName[6] = {
    "peer1.org2.example0.com",
    "peer1.org2.example1.com",
    "peer1.org2.example2.com",
    "peer1.org2.example3.com",
    "peer1.org2.example4.com",
    "peer1.org2.example5.com"};

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

/**
****************************************************************************************
* @brief:
* @param[in] *networkConfig
* @param[in] index
* @return
****************************************************************************************
*/
__BOATSTATIC BOAT_RESULT fabric_getNetworkCfg(BoatHlfabricNetworkConfig *networkConfig, BUINT8 index)
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
    networkConfig->nodesCfg.layoutCfg = (BoatHlfabricNodeLayoutCfg *)BoatMalloc(networkConfig->nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
    networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg = (BoatHlfabricNodeGroupCfg *)BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser = (BoatHlfabricNodeInfoCfg *)BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName[index]) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org1_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric_demo_endorser_peer0Org1_hostName[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric_demo_endorser_peer1Org1_hostName[index]);

    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser = (BoatHlfabricNodeInfoCfg *)BoatMalloc(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url[index]) + 1);
    networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = (BCHAR *)BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName[index]) + 1);
    // memset(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org2_url) + 1);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org2_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric_demo_endorser_peer0Org2_hostName[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url[index]);
    strcpy(networkConfig->nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric_demo_endorser_peer1Org2_hostName[index]);

    // set orderer info
    networkConfig->nodesCfg.orderCfg.endorserNumber = 1;
    networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
    strcpy(networkConfig->nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
    networkConfig->nodesCfg.orderCfg.endorser = (BoatHlfabricNodeInfoCfg *)BoatMalloc(networkConfig->nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
    networkConfig->nodesCfg.orderCfg.endorser[0].hostName =(BCHAR *) BoatMalloc(strlen(fabric_demo_order1_hostName[index]) + 1);
    networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl = (BCHAR *)BoatMalloc(strlen(fabric_demo_order1_url[index]) + 1);
    memset(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric_demo_order1_url[index]) + 1);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].nodeUrl, fabric_demo_order1_url[index]);
    strcpy(networkConfig->nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName[index]);

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
            if (0 != strcmp((char *)nodesCfg1.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content, (char *)nodesCfg2.layoutCfg[i].groupCfg[j].tlsOrgCertContent.content))
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
START_TEST(test_002fabricNetwork_0001CreateOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg, 0);
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
*  This function test case: creat two onetime network success
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0002CreateMoreOneTimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg[2];
    BoatHlfabricNetworkData networkData;
    /************************** create the first onetime network *******************/
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg[index], index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /************************* create the second onetime network ******************/
    index = 1;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg[index], index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
    /* check result of create network , index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);

    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, 0);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg[index], 0);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg[0]);
    fabricWalletConfigFree(networkCfg[1]);
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
START_TEST(test_002fabricNetwork_0003CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM ;
        this is the first persist network, index muse be 1*/
    ck_assert_int_eq(rtnVal, index);
    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat more persist network success;
*  have created a persist network in above case, go ahead and create BOAT_MAX_NETWORK_NUM-1 persistent networks
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0004CreateMorePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHlfabricNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHlfabricNetworkData networkData;
    for (index = 2; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get network configuration to creat fabric network */
        rtnVal = fabric_getNetworkCfg(&networkCfg[index - 1], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* creat persist network , store type must be FLASH */
        rtnVal = BoatHlfabricNetworkCreate(&networkCfg[index - 1], BOAT_STORE_TYPE_FLASH);
        /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM */
        ck_assert_int_eq(rtnVal, index);
        /* get network by index */
        rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
        /* check result of getting network by index */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = fabric_checkNetworkData(networkData, networkCfg[index - 1], index);
        /* free configuration struct */
        fabricWalletConfigFree(networkCfg[index - 1]);
        /* free network data*/
        BoATHlfabric_FreeNetworkData(networkData);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: creat more than BOAT_MAX_NETWORK_NUM persist networks fail;
*  have created BOAT_MAX_NETWORK_NUM persist networks, couldn't create another one;
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0005Create6PersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, BOAT_ERROR_NETWORK_INDEX_EXCEED);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case: get network list ;
*  have created BOAT_MAX_NETWORK_NUM persist networks,
*  there will be BOAT_MAX_NETWORK_NUM network in the list ;
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0006GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHlfabricNetworkContext networkList;

    /* get network list */
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list , have created BOAT_MAX_NETWORK_NUM persist networks,
     num of network in the list must be BOAT_MAX_NETWORK_NUM */
    ck_assert_int_eq(networkList.networkNum, BOAT_MAX_NETWORK_NUM);
    /****************check content of list **************************/
    for (index = 0; index < BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get all the configurations of persist networks*/
        rtnVal = fabric_getNetworkCfg(&networkCfg[index], index + 1);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = fabric_checkNetworkData(networkList.networks[index], networkCfg[index], index + 1);
        /* free configuration struct */
        fabricWalletConfigFree(networkCfg[index]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
    /* free memory of list*/
    BoATHlfabric_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete fabric network which index equal 3 ;
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0007DeletePersistNetworkSuccess_index3)
{
    BSINT32 rtnVal;
    /* delete network*/
    rtnVal = BoATHlfabricNetworkDelete(3);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get fabric network list ;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*  there are BOAT_MAX_NETWORK_NUM-1 networks in the list.
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0008GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHlfabricNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
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
        /* get all the configurations of persist networks*/
        rtnVal = fabric_getNetworkCfg(&networkCfg[i], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = fabric_checkNetworkData(networkList.networks[i], networkCfg[i], index);
        /* free configuration struct */
        fabricWalletConfigFree(networkCfg[i]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
    /* free memory of list*/
    BoATHlfabric_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create persist fabric network success;
*  created BOAT_MAX_NETWORK_NUM persist netsorks and deleted the network which index equal 3,
*   if create new persist network, index of the new persist network must be 3.
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0009CreatePersistNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 3;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat persist network , store type must be FLASH */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_FLASH);
    /* check result of create network */
    ck_assert_int_eq(rtnVal, index);
    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case :  create onetime fabric network success
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0010CreateOnetimeNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg;
    BoatHlfabricNetworkData networkData;
    /* get network configuration to creat fabric network */
    rtnVal = fabric_getNetworkCfg(&networkCfg, index);
    /* check result of getting configuration*/
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* creat onetime network , store type must be RAM */
    rtnVal = BoatHlfabricNetworkCreate(&networkCfg, BOAT_STORE_TYPE_RAM);
    /* check result of create network, index of onetime network must be 0 */
    ck_assert_int_eq(rtnVal, 0);
    /* get network by index */
    rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
    /* check result of getting network by index */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check network data with network configuration */
    rtnVal = fabric_checkNetworkData(networkData, networkCfg, index);
    /* free configuration struct */
    fabricWalletConfigFree(networkCfg);
    /* free network data*/
    BoATHlfabric_FreeNetworkData(networkData);
    /* check result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get persist networks by index;
*   have creted BOAT_MAX_NETWORK_NUM persist networks in above case and
*   get network data by index from 1 to BOAT_MAX_NETWORK_NUM;
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0011GetNetworkByIndex)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    BoatHlfabricNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM];
    BoatHlfabricNetworkData networkData;
    /****************check network data  **************************/
    for (index = 1; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /*get network by index */
        rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
        /* check result*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* get all the configurations of persist networks*/
        rtnVal = fabric_getNetworkCfg(&networkCfg[index - 1], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = fabric_checkNetworkData(networkData, networkCfg[index - 1], index);
        /* free configuration struct */
        fabricWalletConfigFree(networkCfg[index - 1]);
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* free network data*/
        BoATHlfabric_FreeNetworkData(networkData);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : delete all the network;
*   index from 1 to BOAT_MAX_NETWORK_NUM
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0012DeleteAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 0;
    /****************check network data  **************************/
    for (index = 1; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* delete network*/
        rtnVal = BoATHlfabricNetworkDelete(index);
        /* check the result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : get fabric network list ;
*  there have no network ,so networkNum in the list must be 0
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0013GetNetworkListSuccess)
{
    BSINT32 rtnVal;
    BoatHlfabricNetworkContext networkList;
    /* get the network list */
    rtnVal = BoATHlfabric_GetNetworkList(&networkList);
    /* check the result */
    ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    /* check num of network in the list */
    ck_assert_int_eq(networkList.networkNum, 0);
    /* free memory of list*/
    BoATHlfabric_FreeNetworkContext(networkList);
}
END_TEST

/**
****************************************************************************************
* @brief:
*  This function test case : create all the networks ;
*  first, create a onetime network,then creat BOAT_MAX_NETWORK_NUM persist networks;
* @return
****************************************************************************************
*/
START_TEST(test_002fabricNetwork_0014CreateAllNetworkSuccess)
{
    BSINT32 rtnVal;
    BUINT8 index = 1;
    BoatHlfabricNetworkConfig networkCfg[BOAT_MAX_NETWORK_NUM + 1];
    BoatHlfabricNetworkData networkData;
    for (index = 0; index <= BOAT_MAX_NETWORK_NUM; index++)
    {
        /* get network configuration to creat fabric network */
        rtnVal = fabric_getNetworkCfg(&networkCfg[index], index);
        /* check result of getting configuration*/
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        if (index == 0)
        {
            /* creat persist network , store type must be RAM */
            rtnVal = BoatHlfabricNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_RAM);
        }
        else
        {
            /* creat persist network , store type must be FLASH */
            rtnVal = BoatHlfabricNetworkCreate(&networkCfg[index], BOAT_STORE_TYPE_FLASH);
        }

        /* check result of create network , index of persist network is from 1 to BOAT_MAX_NETWORK_NUM */
        ck_assert_int_eq(rtnVal, index);
        /* get network by index */
        rtnVal = BoATHlfabric_GetNetworkByIndex(&networkData, index);
        /* check result of getting network by index */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
        /* check network data with network configuration */
        rtnVal = fabric_checkNetworkData(networkData, networkCfg[index], index);
        /* free configuration struct */
        fabricWalletConfigFree(networkCfg[index]);
        /* free network data*/
        BoATHlfabric_FreeNetworkData(networkData);
        /* check result */
        ck_assert_int_eq(rtnVal, BOAT_SUCCESS);
    }
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
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0001CreateOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0002CreateMoreOneTimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0003CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0004CreateMorePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0005Create6PersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0006GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0007DeletePersistNetworkSuccess_index3);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0008GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0009CreatePersistNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0010CreateOnetimeNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0011GetNetworkByIndex);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0012DeleteAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0013GetNetworkListSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0014CreateAllNetworkSuccess);
    tcase_add_test(tc_networkCreat_api, test_002fabricNetwork_0012DeleteAllNetworkSuccess);

    return s_networkcreate;
}