
#include "nrf_crypto.h"
#include "nrf_crypto_error.h"
#include "mem_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include <nrf_drv_rng.h>
#include "sys/log.h"



#define PARTIAL_MESSAGE_SIZE (16u)

#define LOG_MODULE "SynEncryption"
#define LOG_LEVEL LOG_LEVEL_NONE


#define AES_ERROR_CHECK(error)  \
    do {            \
        if (error)  \
        {           \
            LOG_INFO("\r\nError = 0x%x\r\n%s\r\n",           \
                             ((unsigned int)error),                                \
                             nrf_crypto_error_string_get(error));    \
            return; \
        }           \
    } while (0);



static nrf_crypto_hash_context_t hash_context;




void kg_plat_init()
{
	ret_code_t ret;

	ret = nrf_crypto_init();
	APP_ERROR_CHECK(ret);

	ret = nrf_crypto_hash_init(&hash_context, &g_nrf_crypto_hash_sha256_info);
	APP_ERROR_CHECK(ret);
}




int kg_gen_random(void *p_rng, unsigned char *output, size_t output_len)
{
	ret_code_t ret_val;

	ret_val = nrf_crypto_rng_vector_generate(output, output_len);
	if(ret_val != NRF_SUCCESS)
	{
		LOG_INFO("nrf_crypto_rng_vector_generate returned %d\n", (int)ret_val);
		return (int)ret_val;
	}

	return (int)ret_val;
}


void calcHash(uint8_t *inData, uint8_t dataLen, uint8_t *outData, size_t *outDataLen)
{
	ret_code_t ret;

	ret = nrf_crypto_hash_calculate(&hash_context,
                                         &g_nrf_crypto_hash_sha256_info,
										 inData,
										 dataLen,
										 outData,
										 outDataLen);
    APP_ERROR_CHECK(ret);

}




