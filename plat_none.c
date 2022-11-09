


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/*static uint8_t generate_IV(uint8_t * p_buff, uint8_t size)
{
    uint8_t    available;
    ret_code_t err_code = NRF_SUCCESS;

    nrf_drv_rng_bytes_available(&available);

    uint8_t length = (size < available) ? size : available;
    err_code = nrf_drv_rng_rand(p_buff, length);
    APP_ERROR_CHECK(err_code);
    return length;
}*/



void kg_plat_init()
{

}




int kg_gen_random(void *p_rng, unsigned char *output, size_t output_len)
{
	int i;

	for(i=0; i< output_len; i++)
	{
		output[i] = (rand() % (255));
	}


	return 0;
}

void kg_calcHash(uint8_t *inData, uint8_t dataLen, uint8_t *outData, size_t *outDataLen)
{

	return;
}


