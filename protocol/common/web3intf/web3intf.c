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

/*!@brief Web3 interface

@file web3intf.c contains web3 interface functions for RPC.
*/
#include "boatlog.h"
#include "boatinternal.h"

#include "rpcintf.h"
#include "cJSON.h"
#include "web3intf.h"
#include "boatosal.h"

Web3IntfContext *web3_init(void)
{
    Web3IntfContext *web3intf_context_ptr;
    boat_try_declare;

    web3intf_context_ptr = BoatMalloc(sizeof(Web3IntfContext));

    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 Interface context.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_init_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_json_string_buf.field_len = WEB3_STRING_BUF_STEP_SIZE;
    web3intf_context_ptr->web3_json_string_buf.field_ptr = \
    BoatMalloc(web3intf_context_ptr->web3_json_string_buf.field_len);
    if (web3intf_context_ptr->web3_json_string_buf.field_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 JSON string buffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_init_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_result_string_buf.field_len = WEB3_STRING_BUF_STEP_SIZE;
    web3intf_context_ptr->web3_result_string_buf.field_ptr = \
    BoatMalloc(web3intf_context_ptr->web3_result_string_buf.field_len);
    if (web3intf_context_ptr->web3_result_string_buf.field_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Fail to allocate Web3 result string buffer.");
        boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_init_cleanup);
    }// LCOV_EXCL_STOP

    BoatRandom((BUINT8*)(&web3intf_context_ptr->web3_message_id), 
               sizeof(web3intf_context_ptr->web3_message_id), NULL);
    //web3intf_context_ptr->web3_message_id = random32();

    web3intf_context_ptr->rpc_context_ptr = RpcInit();

    if (web3intf_context_ptr->rpc_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Fail to inintialize RPC.");
        boat_throw(BOAT_ERROR_WEB3_RPC_INIT_FAIL, web3_init_cleanup);
    }// LCOV_EXCL_STOP

    // Clean Up
    boat_catch(web3_init_cleanup)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        if (web3intf_context_ptr != NULL)
        {
            if (web3intf_context_ptr->web3_json_string_buf.field_ptr != NULL)
            {
                BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
            }
            if (web3intf_context_ptr->web3_result_string_buf.field_ptr != NULL)
            {
                BoatFree(web3intf_context_ptr->web3_result_string_buf.field_ptr);
            }
            if (web3intf_context_ptr->rpc_context_ptr != NULL)
            {
                RpcDeinit(web3intf_context_ptr->rpc_context_ptr);
            }
            BoatFree(web3intf_context_ptr);
            web3intf_context_ptr = NULL;
        }
    }// LCOV_EXCL_STOP

    return web3intf_context_ptr;
}

void web3_deinit(Web3IntfContext *web3intf_context_ptr)
{
    if (web3intf_context_ptr == NULL)
    {
		BoatLog(BOAT_LOG_CRITICAL, "web3intf cannot be NULL.");
        return;
    }

    if (web3intf_context_ptr->web3_json_string_buf.field_ptr != NULL)
    {
        BoatFree(web3intf_context_ptr->web3_json_string_buf.field_ptr);
    }

    if (web3intf_context_ptr->web3_result_string_buf.field_ptr != NULL)
    {
        BoatFree(web3intf_context_ptr->web3_result_string_buf.field_ptr);
    }
    
    if (web3intf_context_ptr->rpc_context_ptr != NULL)
    {
        RpcDeinit(web3intf_context_ptr->rpc_context_ptr);
    }

    BoatFree(web3intf_context_ptr);
    
    return;
}


BCHAR *web3_getTransactionCount(Web3IntfContext *web3intf_context_ptr,
                                BCHAR *node_url_str,
                                const Param_web3_getTransactionCount *param_ptr,
                                BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionCount_cleanup);
    }// LCOV_EXCL_STOP
    
    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionCount_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[\"%s\",\"%s\"],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->address_str,
                                        param_ptr->block_num_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
		
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_getTransactionCount_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionCount_cleanup);
    }// LCOV_EXCL_STOP
   
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
							(BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
							expected_string_size,
							(BOAT_OUT BUINT8 **)&rpc_response_str,
							&rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_getTransactionCount_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_getTransactionCount_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }

    return return_value_ptr;
}


BCHAR *web3_gasPrice(Web3IntfContext *web3intf_context_ptr, BCHAR *node_url_str, BCHAR *method_name_str,BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_gasPrice_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_gasPrice_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[],\"id\":%u}",
                                        method_name_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_gasPrice_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_gasPrice_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_gasPrice_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;
	
	// Exceptional Clean Up
    boat_catch(web3_gasPrice_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_getBalance(Web3IntfContext *web3intf_context_ptr,
                       BCHAR *node_url_str,
                       const Param_web3_getBalance *param_ptr,
                       BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getBalance_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getBalance_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[\"%s\",\"%s\"],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->address_str,
                                        param_ptr->block_num_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_getBalance_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);
    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getBalance_cleanup);
    }// LCOV_EXCL_STOP
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_getBalance_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_getBalance_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
                               BCHAR *node_url_str,
                               const Param_web3_sendRawTransaction *param_ptr,
                               BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[\"%s\"],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->signedtx_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_sendRawTransaction_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;
	
    // Exceptional Clean Up
    boat_catch(web3_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}

#ifdef USEGETSTORAGEAT
BCHAR *web3_getStorageAt(Web3IntfContext *web3intf_context_ptr,
						 BCHAR *node_url_str,
						 const Param_web3_getStorageAt *param_ptr,
                         BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getStorageAt_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getStorageAt_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[\"%s\",\"%s\",\"%s\"],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->address_str,
                                        param_ptr->position_str,
                                        param_ptr->block_num_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_getStorageAt_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);
    
    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getStorageAt_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_getStorageAt_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_getStorageAt_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }

    return return_value_ptr;
}
#endif

BCHAR *web3_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
										BCHAR *node_url_str,
										const Param_web3_getTransactionReceipt *param_ptr,
                                        BOAT_RESULT *web3Result)
{
    BCHAR  *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BCHAR  *return_value_ptr = NULL;
    
	BOAT_RESULT result;
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
   
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
				 "[\"%s\"],\"id\":%u}",
                 param_ptr->method_name_str,
				 param_ptr->tx_hash_str,
				 web3intf_context_ptr->web3_message_id
				);
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_getTransactionReceiptStatus_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_getTransactionReceiptStatus_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_call(Web3IntfContext *web3intf_context_ptr,
                 BCHAR *node_url_str,
                 const Param_web3_call *param_ptr,
                 BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
				 "[{\"to\":\"%s\",\"gas\":\"%s\",\"gasPrice\":\"%s\",\"data\":\"%s\"},\"%s\"],\"id\":%u}",
                 param_ptr->method_name_str,
				 param_ptr->to,
				 param_ptr->gas,
				 param_ptr->gasPrice,
				 param_ptr->data,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_call_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_call_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}





#if (PROTOCOL_USE_PLATON == 1)
BCHAR *platon_web3_call(Web3IntfContext *web3intf_context_ptr,
                 BCHAR *node_url_str,
                 const Param_web3_call *param_ptr,
                 BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP
    

    // Construct the REQUEST
	do{
		malloc_size_expand_flag = false;
		expected_string_size = snprintf(
				 (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				 web3intf_context_ptr->web3_json_string_buf.field_len,
				 "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
				 "[{\"from\":\"%s\",\"to\":\"%s\",\"gas\":\"%s\",\"gasPrice\":\"%s\",\"data\":\"%s\"},\"%s\"],\"id\":%u}",
                 param_ptr->method_name_str,
                 param_ptr->from,
				 param_ptr->to,
				 param_ptr->gas,
				 param_ptr->gasPrice,
				 param_ptr->data,
				 param_ptr->block_num_str,
				 web3intf_context_ptr->web3_message_id
				);
		if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_call_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_call_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_call_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


#endif




/***************************************************************************************************
                                  FISCO BCOS JSONRPC INTERFACE
***************************************************************************************************/
#if (PROTOCOL_USE_FISCOBCOS == 1)	

BCHAR *web3_fiscobcos_call(Web3IntfContext *web3intf_context_ptr,
						   BCHAR *node_url_str,
						   const Param_fiscobcos_call *param_ptr,
                           BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_call_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_call_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"call\",\"params\":"
                                        "[%s,{\"from\":\"%s\",\"to\":\"%s\",\"value\":\"%s\",\"data\":\"%s\"}],\"id\":%u}",
                                        param_ptr->groupid,
                                        param_ptr->from,
                                        param_ptr->to,
                                        param_ptr->value,
                                        param_ptr->data,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_fiscobcos_call_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_call_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
							(BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
							expected_string_size,
							(BOAT_OUT BUINT8 **)&rpc_response_str,
							&rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_fiscobcos_call_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_fiscobcos_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}

BCHAR *web3_fiscobcos_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
										 BCHAR *node_url_str,
										 const Param_fiscobcos_sendRawTransaction *param_ptr,
                                         BOAT_RESULT *web3Result)
{
	BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"sendRawTransaction\",\"params\":"
                                        "[%s,\"%s\"],\"id\":%u}",
                                        param_ptr->groupid,
                                        param_ptr->signedtx_str,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_fiscobcos_sendRawTransaction_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
							(BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
							expected_string_size,
							(BOAT_OUT BUINT8 **)&rpc_response_str,
							&rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_fiscobcos_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
	// return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_fiscobcos_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_fiscobcos_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
												  BCHAR *node_url_str,
												  const Param_fiscobcos_getTransactionReceipt *param_ptr,
                                                  BOAT_RESULT *web3Result)
{
    BCHAR  *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
	BBOOL   malloc_size_expand_flag;
    BCHAR  *return_value_ptr = NULL;
    
	BOAT_RESULT result;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
	
    // Construct the REQUEST
    do{
		malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"getTransactionReceipt\",\"params\":"
                                        "[%s,\"%s\"],\"id\":%u}",
                                        param_ptr->groupid,
                                        param_ptr->tx_hash_str,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_fiscobcos_getTransactionReceiptStatus_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
							(BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,  
							expected_string_size,
							(BOAT_OUT BUINT8 **)&rpc_response_str,
							&rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_fiscobcos_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_fiscobcos_getTransactionReceiptStatus_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_fiscobcos_getBlockNumber(Web3IntfContext *web3intf_context_ptr,
									 BCHAR *node_url_str,
									 const Param_fiscobcos_getBlockNumber *param_ptr,
                                     BOAT_RESULT *web3Result)
{
    BCHAR   *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BCHAR   *return_value_ptr = NULL;
	BOAT_RESULT result;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP
    
	
    // Construct the REQUEST
    do{
		malloc_size_expand_flag = false;
        expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
				                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"getBlockNumber\",\"params\":"
                                        "[%s],\"id\":%u}",
                                        param_ptr->groupid,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
		{
			result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
			if (result != BOAT_SUCCESS)
			{// LCOV_EXCL_START
				BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
				boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_fiscobcos_getBlockNumber_cleanup);
			}// LCOV_EXCL_STOP
			malloc_size_expand_flag = true;
		}
	}while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_fiscobcos_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
							(BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
							expected_string_size,
							(BOAT_OUT BUINT8 **)&rpc_response_str,
							&rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_fiscobcos_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
	
    // return entire RESPONSE content	
	return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_fiscobcos_getBlockNumber_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}

#endif /* #if (PROTOCOL_USE_FISCOBCOS == 1) */



/***************************************************************************************************
                                  QUORUM BCOS JSONRPC INTERFACE
***************************************************************************************************/
#if (PROTOCOL_USE_QUORUM == 1)   

BCHAR *web3_fillTransaction(Web3IntfContext *web3intf_context_ptr,
                               BCHAR *node_url_str,
                               const Param_quorun_fillTransaction *param_ptr,
                               BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[{\"to\":\"%s\",\"data\":\"%s\",\"privateFor\":[\"%s\"]}],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->to,
                                        param_ptr->data,
                                        param_ptr->privateFor,
                                        web3intf_context_ptr->web3_message_id
                                        );
        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_sendRawTransaction_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;
    
    // Exceptional Clean Up
    boat_catch(web3_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_sendRawPrivateTransaction(Web3IntfContext *web3intf_context_ptr,
                               BCHAR *node_url_str,
                               const Param_quorum_sendRawPrivateTransaction *param_ptr,
                               BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":"
                                        "[\"%s\", {\"privateFor\":[\"%s\"]}],\"id\":%u}",
                                        param_ptr->method_name_str,
                                        param_ptr->signedtx_str,
                                        param_ptr->privatefor_str,
                                        web3intf_context_ptr->web3_message_id
                                        );
        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_sendRawTransaction_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                    (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                    expected_string_size,
                    (BOAT_OUT BUINT8 **)&rpc_response_str,
                    &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;
    
    // Exceptional Clean Up
    boat_catch(web3_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


#endif /* #if (PROTOCOL_USE_QUORUM == 1) */

/***************************************************************************************************
                                  CITA JSONRPC INTERFACE
***************************************************************************************************/

#if (PROTOCOL_USE_CITA == 1)   
BCHAR *web3_cita_call(Web3IntfContext *web3intf_context_ptr,
                           BCHAR *node_url_str,
                           const Param_cita_call *param_ptr,
                           BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_call_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_call_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"call\",\"params\":"
                                        "[{\"from\":\"%s\",\"to\":\"%s\",\"data\":\"%s\"}, \"latest\"],\"id\":%u}",
                                        param_ptr->from,
                                        param_ptr->to,
                                        param_ptr->data,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_cita_call_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_call_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_cita_call_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_cita_call_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}

BCHAR *web3_cita_sendRawTransaction(Web3IntfContext *web3intf_context_ptr,
                                         BCHAR *node_url_str,
                                         const Param_cita_sendRawTransaction *param_ptr,
                                         BOAT_RESULT *web3Result)
{
    BCHAR *rpc_response_str;
    BUINT32 rpc_response_len;

    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BOAT_RESULT result;
    BCHAR *return_value_ptr = NULL;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"sendRawTransaction\",\"params\":"
                                        "[\"%s\"],\"id\":%u}",
                                        param_ptr->signedtx_str,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_cita_sendRawTransaction_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr, 
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_cita_sendRawTransaction_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_cita_sendRawTransaction_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_cita_getTransactionReceiptStatus(Web3IntfContext *web3intf_context_ptr,
                                                  BCHAR *node_url_str,
                                                  const Param_cita_getTransactionReceipt *param_ptr,
                                                  BOAT_RESULT *web3Result)
{
    BCHAR  *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BCHAR  *return_value_ptr = NULL;
    
    BOAT_RESULT result;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str == NULL || param_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf((BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"getTransactionReceipt\",\"params\":"
                                        "[\"%s\"],\"id\":%u}",
                                        param_ptr->tx_hash_str,
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_cita_getTransactionReceiptStatus_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,  
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_cita_getTransactionReceiptStatus_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);

    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_cita_getTransactionReceiptStatus_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}


BCHAR *web3_cita_getBlockNumber(Web3IntfContext *web3intf_context_ptr,
                                     BCHAR *node_url_str,
                                     BOAT_RESULT *web3Result)
{
    BCHAR   *rpc_response_str;
    BUINT32 rpc_response_len;
    BSINT32 expected_string_size;
    BBOOL   malloc_size_expand_flag;
    BCHAR   *return_value_ptr = NULL;
    BOAT_RESULT result;
    
    boat_try_declare;
    
    if (web3intf_context_ptr == NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Web3 Interface context cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP

    web3intf_context_ptr->web3_message_id++;
    
    if (node_url_str ==  NULL)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "Arguments cannot be NULL.");
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP
    
    
    // Construct the REQUEST
    do{
        malloc_size_expand_flag = false;
        expected_string_size = snprintf(
                                        (BCHAR*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                                        web3intf_context_ptr->web3_json_string_buf.field_len,
                                        "{\"jsonrpc\":\"2.0\",\"method\":\"blockNumber\",\"params\":"
                                          "[],\"id\":%u}",
                                        web3intf_context_ptr->web3_message_id);

        if (expected_string_size >= web3intf_context_ptr->web3_json_string_buf.field_len)
        {
            result = BoatFieldVariable_malloc_size_expand(&web3intf_context_ptr->web3_json_string_buf, WEB3_STRING_BUF_STEP_SIZE);
            if (result != BOAT_SUCCESS)
            {// LCOV_EXCL_START
                BoatLog(BOAT_LOG_CRITICAL, "Failed to excute BoatFieldVariable_malloc_size_expand.");
                boat_throw(BOAT_ERROR_COMMON_OUT_OF_MEMORY, web3_cita_getBlockNumber_cleanup);
            }// LCOV_EXCL_STOP
            malloc_size_expand_flag = true;
        }
    }while(malloc_size_expand_flag);

    BoatLog(BOAT_LOG_VERBOSE, "REQUEST: %s", web3intf_context_ptr->web3_json_string_buf.field_ptr);

    // POST the REQUEST through curl
    result = RpcRequestSet(web3intf_context_ptr->rpc_context_ptr, node_url_str);
    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        boat_throw(BOAT_ERROR_COMMON_INVALID_ARGUMENT, web3_cita_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP
    
    result = RpcRequestSync(web3intf_context_ptr->rpc_context_ptr,
                            (BUINT8*)web3intf_context_ptr->web3_json_string_buf.field_ptr,
                            expected_string_size,
                            (BOAT_OUT BUINT8 **)&rpc_response_str,
                            &rpc_response_len);

    if (result != BOAT_SUCCESS)
    {// LCOV_EXCL_START
        BoatLog(BOAT_LOG_NORMAL, "RpcRequestSync() fails.");
        boat_throw(result, web3_cita_getBlockNumber_cleanup);
    }// LCOV_EXCL_STOP

    BoatLog(BOAT_LOG_VERBOSE, "RESPONSE: %s", rpc_response_str);
    
    // return entire RESPONSE content   
    return_value_ptr = rpc_response_str;

    // Exceptional Clean Up
    boat_catch(web3_cita_getBlockNumber_cleanup)
    {
        BoatLog(BOAT_LOG_NORMAL, "Exception: %d", boat_exception);
        return_value_ptr = NULL;
        result = boat_exception;
    }
    if (web3Result != NULL)
    {
        *web3Result = result;
    }
    return return_value_ptr;
}
#endif  //if (PROTOCOL_USE_CITA == 1) 
