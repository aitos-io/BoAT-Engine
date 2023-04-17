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
#if defined(__unix__) || defined(__unix) || defined(unix)
/* for timestamp source */
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#endif
#include "boatconfig.h"
#include "boatiotsdk.h"
#include "boatlog.h"
#include "boatkeypair.h"
#include "boatEngine.h"
#include "boatosal.h"
#include <time.h>

//#define USE_ONETIME_WALLET



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

#ifndef FABRIC_DEMO_IP
#define FABRIC_URL_IP "127.0.0.1"
#endif

const BCHAR *fabric_demo_order1_url = FABRIC_DEMO_IP ":7050";
const BCHAR *fabric_demo_order1_hostName = "orderer.example.com";

const BCHAR *fabric_demo_endorser_peer0Org1_url = FABRIC_DEMO_IP ":7051";
const BCHAR *fabric_demo_endorser_peer0Org1_hostName = "peer0.org1.example.com";
const BCHAR *fabric_demo_endorser_peer1Org1_url = FABRIC_DEMO_IP ":8051";
const BCHAR *fabric_demo_endorser_peer1Org1_hostName = "peer1.org1.example.com";

const BCHAR *fabric_demo_endorser_peer0Org2_url = FABRIC_DEMO_IP ":9051";
const BCHAR *fabric_demo_endorser_peer0Org2_hostName = "peer0.org2.example.com";
const BCHAR *fabric_demo_endorser_peer1Org2_url = FABRIC_DEMO_IP ":10051";
const BCHAR *fabric_demo_endorser_peer1Org2_hostName = "peer1.org2.example.com";

BoatHlfabricWallet *g_fabric_wallet_ptr;

BUINT8 g_fabric_keypairIndex = 0;
BUINT8 g_fabric_networkIndex = 0;
// DiscoverRes discoverResult;

__BOATSTATIC BOAT_RESULT fabric_createkeypair()
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatKeypairPriKeyCtx_config keypair_config = {0};
	BUINT8 binFormatKey[32] = {0};

	(void)binFormatKey; // avoid warning

	/* keypair_config value assignment */
	keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_EXTERNAL_INJECTION;
	keypair_config.prikey_type = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256R1;
	keypair_config.prikey_format = BOAT_KEYPAIR_PRIKEY_FORMAT_PKCS;
	keypair_config.prikey_content.field_ptr = (BUINT8 *)fabric_client_demokey;
	keypair_config.prikey_content.field_len = strlen(fabric_client_demokey) + 1; // length contain terminator

	/* create ethereum keypair */
#if defined(USE_ONETIME_WALLET)
	result = BoatKeypairCreate(&keypair_config, "keypairOnetime", BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
	result = BoatKeypairCreate(&keypair_config, "keypairPersist", BOAT_STORE_TYPE_FLASH);
#endif
	if (result < BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "create one-time keypair failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}
	g_fabric_keypairIndex = result;
	return BOAT_SUCCESS;
}

__BOATSTATIC BOAT_RESULT fabric_creatNetwork(void)
{
	BOAT_RESULT index;
	BoatHlfabricNetworkConfig networkConfig;

	// set cert context
	networkConfig.accountCertContent.length = strlen(fabric_client_democert) + 1;
	strcpy(networkConfig.accountCertContent.content, fabric_client_democert);

	networkConfig.nodesCfg.endorserLayoutNum = 1;
	networkConfig.nodesCfg.layoutCfg = BoatMalloc(networkConfig.nodesCfg.endorserLayoutNum * sizeof(BoatHlfabricNodeLayoutCfg));
	networkConfig.nodesCfg.layoutCfg[0].endorserGroupNum = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].endorserGroupNum * sizeof(BoatHlfabricNodeGroupCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].quantities = 1;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.length = strlen(fabric_org1_tlsCert);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].tlsOrgCertContent.content, fabric_org1_tlsCert);

	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org1_hostName) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org1_hostName) + 1);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org1_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org1_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[0].hostName, fabric_demo_endorser_peer0Org1_hostName);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, 0, strlen(fabric_demo_endorser_peer1Org1_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org1_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[0].endorser[1].hostName, fabric_demo_endorser_peer1Org1_hostName);

	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber = 2;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].quantities = 1;
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.length = strlen(fabric_org2_tlsCert);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].tlsOrgCertContent.content, fabric_org2_tlsCert);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser = BoatMalloc(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer0Org2_hostName) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_url) + 1);
	networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName = BoatMalloc(strlen(fabric_demo_endorser_peer1Org2_hostName) + 1);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, 0, strlen(fabric_demo_endorser_peer0Org2_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].nodeUrl, fabric_demo_endorser_peer0Org2_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[0].hostName, fabric_demo_endorser_peer0Org2_hostName);
	// memset(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, 0, strlen(fabric_demo_endorser_peer1Org2_url) + 1);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].nodeUrl, fabric_demo_endorser_peer1Org2_url);
	strcpy(networkConfig.nodesCfg.layoutCfg[0].groupCfg[1].endorser[1].hostName, fabric_demo_endorser_peer1Org2_hostName);

	// set orderer info
	networkConfig.nodesCfg.orderCfg.endorserNumber = 1;
	networkConfig.nodesCfg.orderCfg.tlsOrgCertContent.length = strlen(fabric_order_tlsCert);
	strcpy(networkConfig.nodesCfg.orderCfg.tlsOrgCertContent.content, fabric_order_tlsCert);
	networkConfig.nodesCfg.orderCfg.endorser = BoatMalloc(networkConfig.nodesCfg.orderCfg.endorserNumber * sizeof(BoatHlfabricNodeInfoCfg));
	networkConfig.nodesCfg.orderCfg.endorser[0].hostName = BoatMalloc(strlen(fabric_demo_order1_hostName) + 1);
	networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl = BoatMalloc(strlen(fabric_demo_order1_url) + 1);
	// memset(networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl, 0, strlen(fabric_demo_order1_url) + 1);
	strcpy(networkConfig.nodesCfg.orderCfg.endorser[0].nodeUrl, fabric_demo_order1_url);
	strcpy(networkConfig.nodesCfg.orderCfg.endorser[0].hostName, fabric_demo_order1_hostName);

#if (BOAT_HLFABRIC_TLS_SUPPORT == 1) && (BOAT_HLFABRIC_TLS_IDENTIFY_CLIENT == 1)
	networkConfig.accountClientTlsPrikey.value_len = strlen(fabric_client_tls_prikey);
	strcpy((BCHAR *)networkConfig.accountClientTlsPrikey.value, fabric_client_tls_prikey);
	networkConfig.accountClientTlsCert.length = strlen(fabric_client_tls_cert);
	strcpy((BCHAR *)networkConfig.accountClientTlsCert.content, fabric_client_tls_cert);
#endif
	/* create fabric wallet */
#if defined(USE_ONETIME_WALLET)
	index = BoatHlfabricNetworkCreate(&networkConfig, BOAT_STORE_TYPE_RAM);
#elif defined(USE_CREATE_PERSIST_WALLET)
	index = BoatHlfabricNetworkCreate(&networkConfig, BOAT_STORE_TYPE_FLASH);
#else
	return BOAT_ERROR;
#endif
	if (index < 0)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "fabricWalletPrepare failed.");
		return BOAT_ERROR_WALLET_CREATE_FAIL;
	}

	g_fabric_networkIndex = index;
	fabricWalletConfigFree(networkConfig);
	return index;
}

int main(void)
{
	BOAT_RESULT result = BOAT_SUCCESS;
	BoatHlfabricTx tx_ptr;
	boat_try_declare;
	/* step-1: Boat SDK initialization */
	BoatIotSdkInit();

	/* step-2: prepare wallet */
	result = fabric_createkeypair();
	if (result < BOAT_ERROR)
	{
		boat_throw(result, fabric_key_network_catch);
	}
	result = fabric_creatNetwork();
	if (result < BOAT_ERROR)
	{
		boat_throw(result, fabric_key_network_catch);
	}
	g_fabric_wallet_ptr = BoatHlfabricWalletInit(g_fabric_keypairIndex, g_fabric_networkIndex);
	if (g_fabric_wallet_ptr == NULL)
	{

		boat_throw(result, fabric_key_network_catch);
	}

	/* step-3: fabric transaction structure initialization */
	result = BoatHlfabricTxInit(&tx_ptr, g_fabric_wallet_ptr, NULL, "mycc", NULL, "mychannel", "Org1MSP");
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxInit failed.");
		boat_throw(result, fabric_key_network_catch);
	}
	/* step-4: set transaction timestamp */
#if defined(__unix__) || defined(__unix) || defined(unix)
	struct timespec txTimestamp;
	clock_gettime(CLOCK_REALTIME, &txTimestamp);
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, txTimestamp.tv_sec, txTimestamp.tv_nsec);
#else
	long int timesec = 0;
	//time(&timesec);
	timesec = BoatGetTimes();
	result = BoatHlfabricTxSetTimestamp(&tx_ptr, timesec, 0);
#endif
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetTimestamp failed.");
		boat_throw(result, fabric_demo_catch);
	}

	/* step-5: set transaction 'invoke' command */
	result += BoatHlfabricTxSetArgs(&tx_ptr, "invoke", "a", "b", "10", NULL);
	result += BoatHlfabricTxSubmit(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	/* step-6: wait seconds and 'query' the tansaction */
	BoatSleep(3);
	result = BoatHlfabricTxSetArgs(&tx_ptr, "query", "a", NULL);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxSetArgs() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	result = BoatHlfabricTxEvaluate(&tx_ptr);
	if (result != BOAT_SUCCESS)
	{
		// BoatLog(BOAT_LOG_CRITICAL, "BoatHlfabricTxEvaluate() failed.");
		boat_throw(result, fabric_demo_catch);
	}
	BoatLog_hexasciidump(BOAT_LOG_NORMAL, "query result",
						 tx_ptr.evaluateRes.http2Res,
						 tx_ptr.evaluateRes.httpResLen);

	boat_catch(fabric_demo_catch)
	{
		BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
	}
	/* step-7: fabric transaction structure Deinitialization */
	BoatHlfabricTxDeInit(&tx_ptr);
	boat_catch(fabric_key_network_catch)
	{
		BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
	}
	/* step-8: Boat SDK Deinitialization */
	BoatIotSdkDeInit();
	// fabricWalletConfigFree(networkConfig);

	return result;
}
