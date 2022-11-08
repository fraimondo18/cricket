#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#define LOG_MODULE "KG"
#define LOG_LEVEL LOG_LEVEL_INFO

#include "project-conf.h"

#include "keyGenDefs.h"




#ifndef KG_RSSI_COLLECT

const int8_t rssi_data_test[256] = {
		-70, -70, -69, -67, -66, -66, -69, -64, -70, -71, -64, -66, -71, -67, -66, -64, -80, -70, -66, -71,
		-70, -71, -70, -71, -67, -64, -64, -64, -65, -64, -76, -74, -64, -72, -67, -64, -65, -61, -64, -70,
		-68, -65, -69, -61, -63, -59, -70, -67, -70, -74, -64, -62, -61, -62, -67, -64, -63, -66, -61, -61,
		-66, -63, -63, -65, -70, -63, -69, -64, -64, -71, -73, -67, -70, -66, -70, -73, -70, -67, -71, -69,
		-70, -70, -68, -78, -67, -71, -70, -73, -70, -71, -70, -68, -65, -68, -68, -67, -67, -70, -67, -72,
		-70, -70, -69, -67, -66, -66, -69, -64, -70, -71, -64, -66, -71, -67, -66, -64, -80, -70, -66, -71,
		-70, -71, -70, -71, -67, -64, -64, -64, -65, -64, -76, -74, -64, -72, -67, -64, -65, -61, -64, -70,
		-68, -65, -69, -61, -63, -59, -70, -67, -70, -74, -64, -62, -61, -62, -67, -64, -63, -66, -61, -61,
		-66, -63, -63, -65, -70, -63, -69, -64, -64, -71, -73, -67, -70, -66, -70, -73, -70, -67, -71, -69,
		-70, -70, -68, -78, -67, -71, -70, -73, -70, -71, -70, -68, -65, -68, -68, -67, -67, -70, -67, -72,
		-68, -65, -69, -61, -63, -59, -70, -67, -70, -74, -64, -62, -61, -62, -67, -64, -63, -66, -61, -61,
		-70, -71, -70, -71, -67, -64, -64, -64, -65, -64, -76, -74, -64, -72, -67, -64, -65, -61, -64, -70,
		66, -63, -63, -65, -70, -63, -69, -64, -64, -71, -73, -67, -70, -66, -70, -73
};

#endif


///////////////////////////////     DECLARATION      ///////////////////////////////

static kgEncodeParams_t kgEncParams[KG_NUM_ENC_PARAMS] =
{
		{0.01, 2, 0, 9},
		{0.02, 2, 0, 9},
		{0.03, 2, 0, 9},
		{0.04, 3, 0, 10},
		{0.05, 3, 0, 10},
		{0.06, 3, 0, 10},
		{0.07, 3, 0, 10},
		{0.08, 3, 0, 11},
		{0.09, 3, 0, 11},
		{0.1,  5, 1, 9},
		{0.11, 5, 1, 9},
		{0.12, 4, 0, 14},
		{0.13, 4, 0, 14},
		{0.14, 4, 0, 15},
		{0.15, 4, 0, 16},
		{0.16, 6, 1, 11},
		{0.17, 8, 2, 10},
		{0.18, 8, 2, 10},
		{0.19, 10, 3, 9},
		{0.2,  10, 3, 10},
		{0.21, 9, 2, 12},
		{0.22, 9, 2, 12},
		{0.23, 11, 3, 11},
		{0.24, 13, 4, 10},
		{0.25, 15, 5, 10},
		{0.26, 12, 3, 13},
		{0.27, 14, 4, 12},
		{0.28, 18, 6, 11},
		{0.29, 22, 8, 10},
		{0.3,  19, 6, 12}
};






//Only for stats
static uint32_t total_num_ch_sequences = 0;
static uint8_t received_ready_key[KG_OUT_KEY_LEN];


static uint8_t kg_m_digest[32];
static uint8_t kg_received_digest[32];



/////////////////   general variables  /////////////////
static uint16_t kg_mathIndex = 0, kg_varIterations=0;
static uint8_t kg_currentState;

static uint8_t collectRSSI = 0;

/////////////////   MAC layer variables  /////////////////
static uint16_t rssi_num_samples = 0;
static uint8_t key_gen_head;


/////////////////   quantization & shuffling /////////////////
static int8_t quant_rise_ind[KG_RSSI_SEQ_LEN];
static uint8_t index2bitlist[1<<KG_Q_BITS][KG_Q_BITS];
static uint8_t channel_shuffled[KG_CH_SEQ_LEN] = {0};
static float kg_var=0;


/////////////////  filtering & collected rssi  /////////////////
static uint8_t kg_rssi[KG_MAX_RSSI_SAMPLES];
static float kg_filtered[KG_RSSI_SEQ_LEN];
static int32_t window_sum = 0;
static uint8_t values_in_window = 0;
static float average;




/////////////////   key generation   /////////////////
static uint8_t key_index_count = 0;


static uint16_t discard_count_msg=0;


/////////////////  look-up table & send channel sample  ///////////////////
static uint8_t channel_sample_pair[KG_SAMPLE_SEQ_LEN];
static uint8_t channel_sample[KG_SAMPLE_SEQ_LEN];



/////////////////   key assemble   ///////////////////
static uint8_t ready_key[KG_OUT_KEY_LEN];
static uint8_t key_seq_len=0; // to be sent from bob

static uint8_t current_key_index=0;

static uint8_t kg_block_size;
static uint8_t kg_tau;
static uint16_t ini_key_len;


static uint8_t sampleSequenceReceived = 0;
static uint8_t encodeReceived = 0;
static uint8_t discardReceived = 0;
static uint8_t hashReceived = 0;

struct simple_udp_connection kg_udp_conn;
static uip_ipaddr_t kg_dest_ipaddr;


static uint8_t discarded_bits[DISC_BITS_LEN];
static uint8_t  encoded_message[KG_ENC_LEN];
static uint8_t  KEY[SOURCE_KEY_LEN];
static uint8_t key_stretched[KG_STRETCHED_LEN];
static uint8_t estimate_key[((KG_MAX_INIT_KEY_LEN/KG_BITS_PU))];

static uint8_t key_agreed_bits[KG_OUT_KEY_LEN];




static void print_bin(uint8_t ind) // prints integers in bits in byte chunks
{
    uint8_t first, sec, third, four, fifth, six, sev, eight;

    first = ind& 1;
    sec =  (ind>>1) & 1;
    third =  (ind>>2) & 1;
    four =  (ind>>3) & 1;
    fifth =  (ind>>4) & 1;
    six =  (ind>>5) & 1;
    sev =  (ind>>6) & 1;
    eight =  (ind>>7) & 1;
    printf("%i%i%i%i%i%i%i%i ", eight, sev, six, fifth, four, third, sec, first);

}


static uint16_t compareStrings(uint8_t *string1, uint8_t *string2, uint8_t len)
{
	uint8_t err_xor, i;
	uint16_t err_sum = 0;

	for (i=0; i<len; i++)
	{
		err_xor = string1[i] ^ string2[i];
		err_sum += err_xor & 1;
		err_sum += (err_xor>>1) & 1;
		err_sum += (err_xor>>2) & 1;
		err_sum += (err_xor>>3) & 1;
		err_sum += (err_xor>>4) & 1;
		err_sum += (err_xor>>5) & 1;
		err_sum += (err_xor>>6) & 1;
		err_sum += (err_xor>>7) & 1;

		print_bin(err_xor); printf(" = ");
		print_bin(string1[i]);printf(" ^ ");print_bin(string2[i]);
		printf("\n");
	}

	printf("Number of mismatches %u\n", err_sum);

	return err_sum;
}




#ifndef KG_RSSI_COLLECT
static void set_rssi_test(uint8_t *data, uint16_t len)
{
	uint16_t i;
	int8_t r_val;

	for(i=0; i< len; i++)
	{
		r_val = ((rand() % 3) / 2);
		if(r_val%2){
			r_val = (-1) * r_val;
		}

		kg_rssi[i] = (-1)*data[i] + r_val;
		LOG_DBG("r_val %i, kg_rssi[%u]  %u\n",r_val, i , kg_rssi[i]);

		rssi_num_samples++;
	}

}
#endif

static void sendMsg(uint8_t msgType, uint8_t *msgOut, uint8_t *inData, uint16_t inDataLen)
{
    uint16_t msgLen = 0;

    switch(msgType)
    {
    case SAMPLE_SEQ_MSG:
    	LOG_INFO("send SAMPLE_SEQ_MSG\n");
    	msgOut[0] = msgType;
    	memcpy(&msgOut[1], inData, inDataLen);
    	msgLen = 1 + inDataLen;
    	break;
    case ENC_MSG:
    	LOG_INFO("send ENC_MSG\n");
    	msgOut[0] = msgType;
    	memcpy(&msgOut[1], inData, inDataLen);
    	msgLen = 1 + inDataLen;
    	break;
    case DATA_TRAFFIC_MSG:
    	msgOut[0] = msgType;
    	msgOut[1] = collectRSSI;
    	msgLen = 2;
    	break;
    case DISCARD_MSG:
    	LOG_INFO("send DISCARD_MSG\n");
		memcpy(msgOut, discarded_bits, discarded_bits[2] + 3);
		msgLen = discarded_bits[2] + 3;
    	break;
    case KEY_READY_HASH_MSG:
    	LOG_INFO("send KEY_READY_HASH_MSG\n");
    	msgOut[0] = msgType;
    	memcpy(&msgOut[1], inData, inDataLen);
    	msgLen = 1 + inDataLen;

    	//Temporary patch, only for test
    	memcpy(&msgOut[msgLen], ready_key,16);
    	msgLen += 16;

    	break;
    default:

    	break;
    }

	simple_udp_sendto(&kg_udp_conn, msgOut, msgLen, &kg_dest_ipaddr);

}


static void nearest_neighbor()
{
    float average_series;

    if(rssi_num_samples < KG_MAX_RSSI_SAMPLES)
    {
    	return;
    }

    kg_varIterations = 0;
    while(kg_mathIndex<( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2)))
    {
        if( kg_mathIndex < KG_RSSI_SEQ_LEN )
        {
            values_in_window += 1;
            window_sum += (int32_t)kg_rssi[kg_mathIndex];

            if (kg_mathIndex >= KG_WINDOW_SIZE)
            {
                values_in_window -= 1;
                window_sum -= (int32_t)kg_rssi[kg_mathIndex - KG_WINDOW_SIZE];
            }
        }

        if (kg_mathIndex >= KG_WINDOW_SIZE/2)
        {
        	average_series =  ((float) window_sum) / ( (float) values_in_window);

            kg_filtered[kg_mathIndex - (uint16_t)(KG_WINDOW_SIZE/2)] = ((float)kg_rssi[kg_mathIndex-( (uint16_t) (KG_WINDOW_SIZE/2) )]) - average_series;
            average += kg_filtered[kg_mathIndex - (uint16_t)(KG_WINDOW_SIZE/2)];

          // LOG_INFO("kg_rssi[%u]: %i\n", kg_mathIndex - (uint16_t)(KG_WINDOW_SIZE/2), kg_rssi[kg_mathIndex - (uint16_t)(KG_WINDOW_SIZE/2)]);
          // LOG_INFO("average_series (*100): %i\n", (int) (average_series * ((float) 100)));
          // LOG_INFO("filtered value: (*100) %i\n", (int) ( kg_filtered[kg_mathIndex - (uint16_t)(KG_WINDOW_SIZE/2)] * ((float) 100)));
        }

        kg_mathIndex++;

    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }


	if(kg_mathIndex  == (KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2)))
    {
		kg_currentState = KG_VARIANCE;
		kg_mathIndex = 0;
	}

    return;
}


static float getStdFactor(float input)
{
	float factor;

	if(input < 1){
		factor = 1;
	}
	else if(input < 2){
		factor = 0.81;
	}
	else if(input < 3){
		factor = 0.63;
	}
	else if(input < 4){
		factor = 0.53;
	}
	else if(input < 5){
		factor = 0.47;
	}
	else if(input < 6){
		factor = 0.42;
	}
	else if(input < 7){
		factor = 0.39;
	}
	else if(input < 8){
		factor = 0.36;
	}
	else if(input < 9){
		factor = 0.34;
	}
	else if(input < 10){
		factor = 0.32;
	}
	else if(input < 11){
		factor = 0.3;
	}
	else if(input < 12){
		factor = 0.29;
	}
	else if(input < 13){
		factor = 0.28;
	}
	else{
		factor = 0.25;
	}

	LOG_DBG("factor %i\n", (int)(factor*100.0));

	return factor;
}


static void variance()
{
	LOG_DBG("variance\n");

    if(kg_mathIndex == 0)
    {
/*    	LOG_INFO("kg_rssi:\n");
        int i;
    	for(i=0; i<KG_RSSI_SEQ_LEN; i++)
    	{
    		printf("%i, ", kg_rssi[i]);
    	}
    	printf("\n");*/

    	average = average/( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2) );
    }

    kg_varIterations = 0;
    while(kg_mathIndex<( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2)))
    {
    	kg_var += (kg_filtered[kg_mathIndex] - average)*(kg_filtered[kg_mathIndex] - average);
    	kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    if(kg_mathIndex == ( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2) ))
    {
    	kg_var = kg_var/( KG_RSSI_SEQ_LEN- (uint16_t)(KG_WINDOW_SIZE/2) );

    	kg_var = kg_var*getStdFactor(kg_var);  //Approximate sqrt to calculate std

    	kg_mathIndex = 0;
    	kg_currentState = KG_MID_RISE;

    	LOG_INFO("average %i\n", (int)(average*((float)100)));
    	LOG_INFO("Std dev %i.%i\n", (int)kg_var, ((int)(kg_var*((float)100)))%100 );
    }

    return;
}


void index2binary(int8_t value)
{
    uint8_t lsb;
    uint8_t list_index = value;
    int8_t i = KG_Q_BITS-1;
    while(i>=0){
        lsb = value & 0x01;
        index2bitlist[list_index][i] = lsb;
        value = value >> 1;
        i -= 1;
    }

    return;
}




/*
byte of the stretched key array: the_streched_bit_index
the byte of the initial key: the_bit (awful name)
the bit that is to be added to the key stertched: random_bit (it counts how many times a certain bit has to be repeated and then shifts
for the bit to be extracted from the initial key)
the_stretch - counts the number of bits that were added to one byte of the stretched key array it resets when we move to a new byte
*/
static void key_stretch(void)
{
	uint32_t i, the_streched_bit_index, the_bit;
	uint8_t random_bit, the_stretch, bit_to_or;

	LOG_DBG("key_stretch\n");

    for (i=0; i<(ini_key_len*kg_block_size); i++)
    {
    	the_bit =  (uint32_t) i/kg_block_size/KG_BITS_PU;
    	the_stretch = (KG_BITS_PU-1) - (i % KG_BITS_PU);
    	random_bit = (KEY[the_bit] >> ( (KG_BITS_PU-1) - ((i/kg_block_size) % KG_BITS_PU) ) ) & 1;
    	the_streched_bit_index = (uint32_t) i/KG_BITS_PU;
    	bit_to_or = random_bit << the_stretch;
    	key_stretched[the_streched_bit_index] |= bit_to_or;
    }

    return;
}



void initKG_A()
{
	uint8_t i;

	kg_currentState = KG_COLLECTING_RSSI;

	memset(quant_rise_ind, 0, KG_RSSI_SEQ_LEN*sizeof(quant_rise_ind[0]));
    memset(channel_shuffled, 0, KG_CH_SEQ_LEN*sizeof(channel_shuffled[0]));
	memset(kg_rssi, 0, KG_MAX_RSSI_SAMPLES*sizeof(kg_rssi[0]));
	memset(kg_filtered, 0, KG_RSSI_SEQ_LEN*sizeof(kg_filtered[0]));
    memset(key_agreed_bits, 0, KG_OUT_KEY_LEN*sizeof(key_agreed_bits[0]));
    memset(channel_sample, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample[0]));
    memset(ready_key, 0, KG_OUT_KEY_LEN*sizeof(ready_key[0]));
    memset(channel_sample_pair, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample_pair[0]));
    memset(key_stretched, 0, KG_STRETCHED_LEN);

	for (i=0; i <(1<<KG_Q_BITS); i++){
		index2binary(i);
	}

	key_gen_head = 1;

	discard_count_msg = 0;
	average = 0;
	window_sum = 0;
	values_in_window = 0;
	kg_var = 0;

	kg_mathIndex = 0;

	collectRSSI = 1;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,KG_MAX_RSSI_SAMPLES);
#endif

	kg_plat_init();

    return;
}



void initKG_B()
{
	uint8_t i;
	kg_currentState = KG_COLLECTING_RSSI;

	memset(quant_rise_ind, 0, KG_RSSI_SEQ_LEN*sizeof(quant_rise_ind[0]));
    memset(channel_shuffled, 0, KG_CH_SEQ_LEN*sizeof(channel_shuffled[0]));
	memset(kg_rssi, 0, KG_MAX_RSSI_SAMPLES*sizeof(kg_rssi[0]));
	memset(kg_filtered, 0, KG_RSSI_SEQ_LEN*sizeof(kg_filtered[0]));
    memset(channel_sample, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample[0]));
    memset(ready_key, 0, KG_OUT_KEY_LEN*sizeof(ready_key[0]));
    memset(estimate_key, 0, (KG_MAX_INIT_KEY_LEN/KG_BITS_PU)*sizeof(estimate_key[0]));

	for (i=0; i <(1<<KG_Q_BITS); i++){
		index2binary(i);
	}

	key_gen_head = 0;
	average = 0;
	window_sum = 0;
	values_in_window = 0;
	kg_var = 0;

	kg_mathIndex = 0;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,KG_MAX_RSSI_SAMPLES);
#endif

	kg_plat_init();

    return;
}



void reInitKG_A()
{
    memset(channel_shuffled, 0, KG_CH_SEQ_LEN*sizeof(channel_shuffled[0]));
	memset(kg_rssi, 0, KG_MAX_RSSI_SAMPLES*sizeof(kg_rssi[0]));
	memset(kg_filtered, 0, KG_RSSI_SEQ_LEN*sizeof(kg_filtered[0]));
	memset(quant_rise_ind, 0, KG_RSSI_SEQ_LEN*sizeof(quant_rise_ind[0]));
    memset(discarded_bits, 0, DISC_BITS_LEN*sizeof(discarded_bits[0]));
    memset(key_agreed_bits, 0, KG_OUT_KEY_LEN*sizeof(key_agreed_bits[0]));
    memset(channel_sample, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample[0]));
    memset(channel_sample_pair, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample_pair[0]));
    memset(key_stretched, 0, KG_STRETCHED_LEN);

    discard_count_msg = 0;
    rssi_num_samples = 0;
    average = 0;
    window_sum = 0;
    values_in_window = 0;
    kg_var = 0;

    kg_mathIndex = 0;

    collectRSSI = 1;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,KG_MAX_RSSI_SAMPLES);
#endif
}

void reInitKG_B()
{
    memset(channel_shuffled, 0, KG_CH_SEQ_LEN*sizeof(channel_shuffled[0]));
	memset(kg_rssi, 0, KG_MAX_RSSI_SAMPLES*sizeof(kg_rssi[0]));
	memset(kg_filtered, 0, KG_RSSI_SEQ_LEN*sizeof(kg_filtered[0]));
	memset(quant_rise_ind, 0, KG_RSSI_SEQ_LEN*sizeof(quant_rise_ind[0]));
    memset(channel_sample, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample[0]));
    memset(channel_sample_pair, 0, KG_SAMPLE_SEQ_LEN*sizeof(channel_sample_pair[0]));
    memset(estimate_key, 0, (KG_MAX_INIT_KEY_LEN/KG_BITS_PU)*sizeof(estimate_key[0]));

    key_index_count = 0;
    rssi_num_samples = 0;
    average = 0;
    window_sum = 0;
    values_in_window = 0;
    kg_var = 0;

    kg_mathIndex = 0;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,KG_MAX_RSSI_SAMPLES);
#endif
}



static void quantize()
{
	uint16_t i, chIdx;
	uint8_t offset, index_value, val;
	offset = (1<<(KG_Q_BITS-1));

	LOG_DBG("quantize\n");

    kg_varIterations = 0;
    while(kg_mathIndex<( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2) ))
    {
    	index_value = quant_rise_ind[kg_mathIndex] + offset;
		for (i=0; i<KG_Q_BITS; i++){
			chIdx = (2*kg_mathIndex+i)/((uint16_t)KG_BITS_PU);
			val = index2bitlist[index_value][i];
			if(val)
			{
                channel_shuffled[chIdx] |= (1 << ( (KG_BITS_PU-1) - ( (2*kg_mathIndex+i) % KG_BITS_PU ) ));
			}
		}     

    	kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    if(kg_mathIndex == ( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2) ))
    {   

    	LOG_INFO("Channel sequence\n");
    	for(chIdx=0; chIdx<KG_CH_SEQ_LEN; chIdx++)
    	{
    		print_bin(channel_shuffled[chIdx]);
    	}
    	printf("\n");


    	kg_mathIndex = 0;
    	kg_currentState = KG_SHUFFLE;
    }

    return;
}


static void mid_rise()
{
    float limit_down= -2*kg_var, limit_up=2*kg_var;
    float step_size, value;
    float k;
    step_size = (limit_up - limit_down) / (1<<KG_Q_BITS);

    LOG_DBG("mid_rise\n");

    kg_varIterations = 0;
    while(kg_mathIndex<( KG_RSSI_SEQ_LEN - (uint16_t)(KG_WINDOW_SIZE/2) ))
    {
        if (kg_filtered[kg_mathIndex] < limit_down)
        {
            value = limit_down;
        }
        else if (kg_filtered[kg_mathIndex] >= limit_up)
        {
            value = limit_up - 0.0001;
        }
        else{
            value = kg_filtered[kg_mathIndex];
        }
     
        k = (value / step_size) + 0.00001; 
        if (k<0){
            k -= 1;
        }

        quant_rise_ind[kg_mathIndex] = (int8_t) k;

    	kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    if(kg_mathIndex == ( KG_RSSI_SEQ_LEN - ( (uint16_t)(KG_WINDOW_SIZE/2) ) ))
    {
    	kg_mathIndex = 0;
    	kg_currentState = KG_QUANT;
    }

    return;
}


static void encode()
{
	LOG_INFO("encode ini_key_len %u, kg_block_size %u\n", ini_key_len, kg_block_size);

    kg_varIterations = 0;
    while(kg_mathIndex < ((ini_key_len*kg_block_size)/KG_BITS_PU) + 1)
    {
        encoded_message[kg_mathIndex] = key_stretched[kg_mathIndex] ^ channel_shuffled[kg_mathIndex + KG_SAMPLE_SEQ_LEN]; // to account for the burned bits
    	kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    if(kg_mathIndex == ((ini_key_len*kg_block_size)/KG_BITS_PU) + 1)
    {
        kg_mathIndex = 0;
        kg_currentState = KG_ENCODE_READY;
    }

    return;
}





static void decode(void)
{
	uint8_t encoded = 0, channel = 0, shifted_bit, shift_key, key_byte, shift_decode, the_bit=0;
    uint8_t key_index, discarded_bit_count=0;

    LOG_INFO("decode ini_key_len %u kg_block_size %u\n", ini_key_len,kg_block_size);

    while (kg_mathIndex<ini_key_len*kg_block_size)
    {
    	encoded = encoded_message[(uint16_t) (kg_mathIndex/KG_BITS_PU)];
    	channel = channel_shuffled[(uint16_t) ((kg_mathIndex + KG_SEQ_SAMPLE_LEN)/KG_BITS_PU)]; // to account for the burned bits

        key_byte = encoded ^ channel;
        shift_decode = (KG_BITS_PU-1) - (kg_mathIndex%KG_BITS_PU);
        shifted_bit = (key_byte >> shift_decode) & 1;
        the_bit += shifted_bit;
        kg_mathIndex++;

        if( ((kg_mathIndex%kg_block_size)==0) && (kg_mathIndex>0))
        {
        	LOG_DBG("the_bit %u\n", the_bit);

        	if (the_bit >= kg_block_size-kg_tau)
        	{
        		key_index = key_index_count/KG_BITS_PU;
        		shift_key = (KG_BITS_PU-1) - key_index_count%KG_BITS_PU;
        	    estimate_key[key_index] |= (1 <<  shift_key);

        	    LOG_DBG("estimate_key kg_mathIndex %u, shift_key %u, key_index_count %u\n", kg_mathIndex, shift_key, key_index_count);

                key_index_count++;
        	}
            else if (the_bit > kg_tau && the_bit < kg_block_size-kg_tau)
            {
            	key_index = (kg_mathIndex/kg_block_size) - 1;
            	if(discarded_bit_count+3 < DISC_BITS_LEN){
            		discarded_bits[discarded_bit_count+3] = key_index;
            	}
            	discarded_bit_count++;

            	LOG_DBG("discard key_index %u\n", key_index);
            }
            else
            {
                key_index_count++;
                //Bit set to zero already done in the initial reinit
                LOG_DBG("else key_index_count %u\n", key_index_count);
            }
        	the_bit = 0;

        }

        if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    LOG_DBG("ini_key_len %u kg_block_size %u key_index_count %u, discarded_bit_count %u\n", ini_key_len,kg_block_size, key_index_count, discarded_bit_count);

    if((kg_mathIndex == (ini_key_len*kg_block_size)) || (key_index_count==128))
    {
        discarded_bits[0] = DISCARD_MSG;
        discarded_bits[1] = key_index_count;
        discarded_bits[2] = discarded_bit_count;
    	kg_mathIndex = 0;
        kg_currentState = KG_ASSEMBLE;
    }

    return;
}





static void discard_bits()
{
    uint8_t bit=0;
    uint16_t index_new=0, index_old=0, discarded_count=0;

    LOG_INFO("discard_bits key_seq_len %u, discard_count_msg %u\n", key_seq_len, discard_count_msg);

    if(key_seq_len == 0)
    {
       return;
    }

    while (index_old< (key_seq_len+discard_count_msg) )
    {
    	LOG_DBG("index_old %u, bit index %u, discarded_count %u\n", index_old, discarded_bits[discarded_count], discarded_count);

        if(discard_count_msg)
        {
            if(index_old != discarded_bits[discarded_count])
            {
                bit = (( KEY[(uint16_t) (index_old/KG_BITS_PU)] >> ( (KG_BITS_PU-1) - (index_old%KG_BITS_PU) ) ) & 1);
                key_agreed_bits[(uint16_t) (index_new/KG_BITS_PU)] |= ( bit << ( (KG_BITS_PU-1) - (index_new%KG_BITS_PU) ) );

                LOG_DBG("index_old %u\n", index_old);
                LOG_DBG("index_new %u\n", index_new);
                LOG_DBG("bit %u\n", bit);
               //print_bin(key_agreed_bits[(uint16_t) (index_new/KG_BITS_PU)]);
               //printf("\n");

                index_new++;
            }
            else
            {
            	LOG_DBG("discarded %u\n", index_old);
           		discarded_count++;
            }
        }
        else
        {
           // if ((index_old%KG_BITS_PU) == (KG_BITS_PU)){
            	key_agreed_bits[(uint8_t) index_old/KG_BITS_PU] = KEY[(uint8_t) index_old/KG_BITS_PU];
            //}
        }

        index_old++;
    }
}


static void gen_key()
{
	//uint8_t i;

	LOG_DBG("gen_key\n");

	kg_gen_random(NULL, KEY, 16);

    key_stretch();
}


static uint8_t look_up_table_A(void)
{   
    float missmatch;
    uint8_t idx, err_xor, i;

    uint16_t err_sum = 0;

    LOG_DBG("look_up_table_A\n");


    for (idx=0; idx<KG_SAMPLE_SEQ_LEN; idx++)
    {
        err_xor = channel_sample[idx] ^ channel_sample_pair[idx];
        err_sum += err_xor & 1;
        err_sum += (err_xor>>1) & 1;
        err_sum += (err_xor>>2) & 1;
        err_sum += (err_xor>>3) & 1;
        err_sum += (err_xor>>4) & 1;
        err_sum += (err_xor>>5) & 1;
        err_sum += (err_xor>>6) & 1;
        err_sum += (err_xor>>7) & 1;
    }

#ifdef KG_DBG
    compareStrings(channel_sample, channel_sample_pair, KG_SAMPLE_SEQ_LEN);
    printf("  errsum: %i\n", err_sum);
#endif

    missmatch = ( (float) err_sum)/ ((float) KG_SEQ_SAMPLE_LEN);

    LOG_INFO("missmatch: %i%%\n", (int)(missmatch*100));

    if(missmatch > kgEncParams[KG_NUM_ENC_PARAMS-1].nissmatch)
    {
    	kg_currentState = KG_COLLECTING_RSSI;
    	reInitKG_A();
    	return 0;
    }

	kg_tau = kgEncParams[KG_NUM_ENC_PARAMS-1].tau;
	ini_key_len = kgEncParams[KG_NUM_ENC_PARAMS-1].ini_key_len;
	kg_block_size = kgEncParams[KG_NUM_ENC_PARAMS-1].block_size;
    for(i=0; i<KG_NUM_ENC_PARAMS; i++)
    {
    	if(missmatch < kgEncParams[i].nissmatch)
    	{
    		kg_tau = kgEncParams[i].tau;
    		ini_key_len = kgEncParams[i].ini_key_len;
    		kg_block_size = kgEncParams[i].block_size;
    		break;
    	}
    }

    LOG_INFO("kg_tau: %i\n", kg_tau);
    LOG_INFO("kg_block_size: %i\n", kg_block_size);
    LOG_INFO("ini_key_len: %i\n", ini_key_len);

    gen_key();

    kg_currentState = KG_ENCODE;

    return 0;
    
}



static void look_up_table_B(void)
{
    float missmatch;
    uint8_t idx, err_sum=0, err_xor, i;

    LOG_DBG("look_up_table_B\n");

    for (idx=0; idx<KG_SAMPLE_SEQ_LEN; idx++)
    {
        err_xor = channel_sample[idx] ^ channel_sample_pair[idx];
        err_sum += err_xor & 1;
        err_sum += (err_xor>>1) & 1;
        err_sum += (err_xor>>2) & 1;
        err_sum += (err_xor>>3) & 1;
        err_sum += (err_xor>>4) & 1;
        err_sum += (err_xor>>5) & 1;
        err_sum += (err_xor>>6) & 1;
        err_sum += (err_xor>>7) & 1;
    }

#ifdef KG_DBG
    compareStrings(channel_sample, channel_sample_pair, KG_SAMPLE_SEQ_LEN);
    printf("  errsum: %i\n", err_sum);
#endif

    missmatch = ( (float) err_sum)/ ((float) KG_SEQ_SAMPLE_LEN);

    LOG_INFO("missmatch: %i%%\n", (int)(missmatch*100));

    if(missmatch > kgEncParams[KG_NUM_ENC_PARAMS-1].nissmatch)
    {
    	kg_currentState = KG_COLLECTING_RSSI;
    	reInitKG_B();
    	return;
    }

	kg_tau = kgEncParams[KG_NUM_ENC_PARAMS-1].tau;
	ini_key_len = kgEncParams[KG_NUM_ENC_PARAMS-1].ini_key_len;
	kg_block_size = kgEncParams[KG_NUM_ENC_PARAMS-1].block_size;
    for(i=0; i<KG_NUM_ENC_PARAMS; i++)
    {
    	if(missmatch < kgEncParams[i].nissmatch)
    	{
    		kg_tau = kgEncParams[i].tau;
    		ini_key_len = kgEncParams[i].ini_key_len;
    		kg_block_size = kgEncParams[i].block_size;
    		break;
    	}
    }


    kg_currentState = KG_WAIT_ENCODED_MSG;

    LOG_INFO("kg_tau: %i, kg_block_size: %i,ini_key_len: %i\n", kg_tau, kg_block_size, ini_key_len);

}




static void shuffle()
{
    uint8_t current_shift, shuffled_shift, shuffled_byte_idx, current_byte_idx, curr_bit, shuffled_bit;
    
    LOG_DBG("shuffle %u\n", KG_CH_SEQ_LEN*KG_BITS_PU);

    kg_varIterations = 0;

    uint16_t rand_idx[KG_CH_SEQ_LEN*KG_BITS_PU] = {
    		300, 351, 250, 360, 118, 161, 180, 386, 397, 407, 338, 123, 291, 4, 249, 205, 0, 357, 408, 327,
			285, 339, 89, 282, 190, 96, 282, 213, 121, 257, 154, 278, 335, 201, 174, 188, 1, 380, 359, 291,
			145, 319, 122, 329, 330, 104, 375, 391, 342, 7, 90, 212, 345, 333, 386, 373, 266, 92, 102, 69,
			149, 332, 24, 342, 139, 381, 270, 286, 185, 61, 244, 402, 326, 47, 9, 371, 49, 335, 154, 9, 12,
			286, 348, 27, 3, 388, 289, 282, 234, 306, 22, 128, 207, 27, 409, 1, 239, 232, 128, 72, 228, 74,
			167, 398, 114, 153, 189, 1, 75, 211, 119, 185, 8, 398, 52, 43, 321, 216, 184, 22, 379, 214, 183,
			298, 108, 128, 5, 164, 263, 143, 203, 236, 194, 355, 376, 49, 111, 326, 384, 59, 308, 398, 269,
			316, 236, 114, 36, 126, 98, 273, 159, 368, 146, 258, 32, 129, 297, 261, 320, 251, 72, 359, 402,
			232, 171, 112, 3, 62, 289, 118, 73, 31, 143, 365, 103, 219, 279, 288, 366, 115, 153, 283, 385,
			40, 30, 37, 134, 87, 345, 157, 100, 91, 193, 245, 155, 314, 11, 220, 334, 400, 52, 177, 282, 296,
			232, 255, 272, 316, 402, 325, 77, 334, 290, 252, 7, 60, 9, 207, 399, 193, 133, 158, 24, 242, 166,
			204, 60, 71, 191, 55, 16, 405, 287, 201, 345, 182, 300, 179, 412, 181, 326, 50, 64, 2, 183, 398,
			125, 308, 403, 4, 408, 132, 261, 107, 323, 361, 293, 276, 202, 303, 362, 374, 116, 362, 84, 266,
			375, 81, 279, 60, 49, 107, 14, 402, 89, 128, 319, 129, 83, 16, 196, 346, 105, 295, 26, 402, 274,
			182, 55, 170, 52, 150, 95, 36, 353, 153, 31, 262, 374, 297, 182, 295, 253, 59, 10, 18, 291, 32,
			306, 18, 315, 130, 185, 218, 351, 115, 141, 352, 63, 23, 17, 186, 146, 387, 251, 405, 173, 207,
			324, 8, 26, 129, 137, 68, 379, 290, 168, 373, 147, 318, 33, 194, 300, 88, 16, 149, 373, 26, 114,
			309, 111, 36, 259, 100, 368, 385, 225, 277, 203, 61, 92, 134, 277, 208, 127, 275, 22, 351, 27, 381,
			341, 214, 260, 72, 269, 329, 201, 70, 134, 179, 142, 60, 56, 408, 409, 289, 86, 345, 45, 335, 45,
			310, 303, 285, 334, 112, 70, 68, 366, 3, 115, 38, 165, 214, 157, 61, 87, 174, 275, 287, 15, 31,
			20, 138, 172, 109};


    while (kg_mathIndex<KG_CH_SEQ_LEN*KG_BITS_PU)
    {
        current_byte_idx = (kg_mathIndex/KG_BITS_PU);
        current_shift = (KG_BITS_PU-1) - (kg_mathIndex%KG_BITS_PU);

        shuffled_byte_idx = (rand_idx[kg_mathIndex]/KG_BITS_PU);
        shuffled_shift = (KG_BITS_PU-1) - (rand_idx[kg_mathIndex]%KG_BITS_PU);

        curr_bit = (channel_shuffled[current_byte_idx] >> current_shift) & 1;
        shuffled_bit = (channel_shuffled[shuffled_byte_idx] >> shuffled_shift) & 1;

        if (curr_bit != shuffled_bit)
        {
            channel_shuffled[current_byte_idx] ^= (1<<current_shift);
            channel_shuffled[shuffled_byte_idx] ^= (1<<shuffled_shift);
        }

        kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    LOG_DBG("kg_mathIndex %u, total %u\n", kg_mathIndex, (KG_CH_SEQ_LEN*KG_BITS_PU));

    if(kg_mathIndex == KG_CH_SEQ_LEN*KG_BITS_PU)
    {   
        memcpy(channel_sample, channel_shuffled, KG_SAMPLE_SEQ_LEN);

        kg_mathIndex = 0;
    	kg_currentState = KG_SAMPLE;
    }

    return;
}






static void assemble_key_A()
{
    uint8_t key_bit_idx, agreed_key_byte, agreed_key_bit, agreed_key_shift;
    uint8_t key_byte, key_shift;
    int i;

    LOG_INFO("assemble_key_A\n");

    if(key_seq_len > 0)
    {
        key_bit_idx = 0;
        while(current_key_index<KG_OUT_KEY_LEN)
        {
            agreed_key_byte = (uint8_t) (key_bit_idx/KG_BITS_PU);
            agreed_key_shift = ((KG_BITS_PU-1) - (key_bit_idx%KG_BITS_PU));
            agreed_key_bit = ( key_agreed_bits[agreed_key_byte]>>agreed_key_shift ) & 1;

            LOG_DBG("key_bit_idx %u\n", key_bit_idx);
            LOG_DBG("agreed_key_bit %u\n", agreed_key_bit);

            key_byte = (uint8_t) (current_key_index/KG_BITS_PU);
            key_shift = ((KG_BITS_PU-1) - (current_key_index%KG_BITS_PU));

            LOG_DBG("key_bit_idx %u\n", key_bit_idx);
            LOG_DBG("key_shift %u\n", key_shift);

            ready_key[key_byte] |= (agreed_key_bit << key_shift);

            current_key_index++;
            key_bit_idx++;

            if (key_bit_idx==key_seq_len)
            {
                break;
            }
        }
    }


    LOG_INFO("current_key_index %u\n", current_key_index);

    total_num_ch_sequences += (kg_block_size*ini_key_len);

    if (current_key_index==KG_OUT_KEY_LEN)
    {
        kg_currentState = KG_KEY_READY;

        LOG_INFO("KEY ready\n");

        for (i=0; i<((uint8_t) (KG_OUT_KEY_LEN/8)); i++){
        	 print_bin(ready_key[i]);
        }
        printf("\n");

    }
    else
    {
        kg_currentState = KG_COLLECTING_RSSI;
        reInitKG_A();
    	LOG_DBG("partial key updated\n");
    }

}



static void assemble_key_B()
{
	uint8_t key_bit_idx, est_key_byte, new_bit, est_key_shift;
	uint8_t key_byte, key_shift;
	int i;

	LOG_INFO("assemble_key_B\n");

	key_bit_idx = 0;
	while(current_key_index<KG_OUT_KEY_LEN)
	{
		est_key_byte = (uint8_t) (key_bit_idx/KG_BITS_PU);
		est_key_shift = ((KG_BITS_PU-1) - (key_bit_idx%KG_BITS_PU));
		new_bit = ( estimate_key[est_key_byte]>>est_key_shift ) & 1;

		key_byte = (uint8_t) (current_key_index/KG_BITS_PU);
		key_shift = ((KG_BITS_PU-1) - (current_key_index%KG_BITS_PU));

		ready_key[key_byte] |=  (new_bit << key_shift);

		current_key_index++;
		key_bit_idx++;

		if (key_bit_idx==key_index_count)
		{
			break;
		}
	}

	LOG_INFO("current_key_index %u\n", current_key_index);

	total_num_ch_sequences += (kg_block_size*ini_key_len);

	if (current_key_index==KG_OUT_KEY_LEN)
	{
		kg_currentState = KG_KEY_READY;
		LOG_INFO("KEY ready\n");

		for (i=0; i<((uint8_t) (KG_OUT_KEY_LEN/8)); i++){
			print_bin(ready_key[i]);
		}
		printf("\n");
	}
	else
	{
		kg_currentState = KG_COLLECTING_RSSI;
		reInitKG_B();
		LOG_DBG("partial key updated\n");
	}

	return;
}


static void sendHash()
{
	size_t digest_len = 32;
	uint8_t msgOut[128];

	calcHash(ready_key, 16, kg_m_digest, &digest_len);

	sendMsg(KEY_READY_HASH_MSG, msgOut, kg_m_digest, 32);

	kg_currentState = KG_WAIT_HASH_KEY;
}


static void verifyHash()
{
	size_t digest_len = 32;
	uint16_t numMismatches;
	float kg_KDR;

	calcHash(ready_key, 16, kg_m_digest, &digest_len);

	if(memcmp(kg_received_digest, kg_m_digest, 32) == 0){
		LOG_INFO("Hash key correct\n");
	}
	else{
		LOG_INFO("Hash key different\n");
	}

	numMismatches = compareStrings(received_ready_key, ready_key, 16);

	kg_KDR = ((float)numMismatches)/((float)KG_OUT_KEY_LEN);
	LOG_INFO("KDR (*1000) %i\n", (int)(kg_KDR*1000.0));

	kg_KDR = ((float)KG_OUT_KEY_LEN)/((float)total_num_ch_sequences);
	LOG_INFO("Encoding rate (*1000) %i\n", (int)(kg_KDR*1000.0));

	LOG_INFO("total_num_ch_sequences %lu\n", total_num_ch_sequences);

	total_num_ch_sequences = 0;

}


uint8_t KG_sm_A()
{
	uint8_t msgOut[256];

	LOG_DBG("KG_sm_A state %u\n", kg_currentState);

	if(kg_currentState< KG_SAMPLE){
		sendMsg(DATA_TRAFFIC_MSG, msgOut, NULL, 0);
	}

	switch(kg_currentState)
	{
	case KG_COLLECTING_RSSI:
		LOG_DBG("rssi_num_samples %u\n", rssi_num_samples);
		nearest_neighbor();

		break;
	case KG_VARIANCE:
		variance();
		break;
	case KG_MID_RISE:
		mid_rise();
		break;
	case KG_QUANT:
		quantize();
		break;
    case KG_SHUFFLE:
		shuffle();
		break;
    case KG_SAMPLE:
    	if(sampleSequenceReceived == 1){
    		sampleSequenceReceived = 0;
    		sendMsg(SAMPLE_SEQ_MSG, msgOut, channel_sample, KG_SAMPLE_SEQ_LEN);
    		kg_currentState = KG_TABLE;
    	}
    	else{
    		LOG_INFO("Waiting sample seq\n");
    	}
		break;
    case KG_TABLE:
		look_up_table_A();
		break;
	case KG_ENCODE:
		encode();
		break;
	case KG_ENCODE_READY:
		sendMsg(ENC_MSG, msgOut, encoded_message, ((ini_key_len*kg_block_size)/KG_BITS_PU) + 1);

		kg_currentState = KG_WAIT_DISCARDED;
		break;
	case KG_WAIT_DISCARDED:
		if(discardReceived == 1){
			discardReceived = 0;
			kg_currentState = KG_DISCARD;
		}
		break;
    case KG_DISCARD:
        discard_bits();
        assemble_key_A();
        break;
    case KG_KEY_READY:
        sendHash();
		break;
    case KG_WAIT_HASH_KEY:
    	if(hashReceived){
    		hashReceived = 0;
            verifyHash();
            kg_currentState = KG_COLLECTING_RSSI;
            reInitKG_A();
            memset(ready_key, 0, KG_OUT_KEY_LEN*sizeof(ready_key[0]));
            current_key_index=0;
    	}
    	else{
    		LOG_INFO("Waiting hash\n");
    	}
		break;
	default:
		break;
	}

    return kg_currentState;
}



uint8_t KG_sm_B()
{
	uint8_t msgOut[256];

	LOG_DBG("KG_sm_B state %u\n", kg_currentState);

	switch(kg_currentState)
	{
	case KG_COLLECTING_RSSI:
		LOG_DBG("rssi_num_samples %u\n", rssi_num_samples);
		nearest_neighbor();

		break;
	case KG_VARIANCE:
		variance();
		break;
	case KG_MID_RISE:
		mid_rise();
		break;
	case KG_QUANT:
		quantize();
		break;
    case KG_SHUFFLE:
		shuffle();
		break;
    case KG_SAMPLE:
    	sendMsg(SAMPLE_SEQ_MSG, msgOut, channel_sample, KG_SAMPLE_SEQ_LEN);
    	kg_currentState = KG_SAMPLE_RESPONSE;
		break;
    case KG_SAMPLE_RESPONSE:
    	if(sampleSequenceReceived == 1)
    	{
    		sampleSequenceReceived = 0;
    		kg_currentState = KG_TABLE;
    	}
    	else{
    		LOG_INFO("Waiting sample seq\n");
    	}
		break;
    case KG_TABLE:
        look_up_table_B();
		break;
    case KG_WAIT_ENCODED_MSG:
    	if(encodeReceived == 1)
    	{
    		encodeReceived = 0;
    		kg_currentState = KG_DECODE;
    	}
    	else{
    		LOG_INFO("Waiting encoded msg\n");
    	}
    	break;
	case KG_DECODE: // will happen upon receiveing encrypted message from Alice
                    // and the look-up table being ready. The interval between receiving
                    // the encrypted message and the channel sample should be long
                    // enough to assume that once we receive encrypted message look-up
                    // table is already present
		decode();

		sendMsg(DISCARD_MSG, msgOut, (uint8_t *)discarded_bits, 0);
		break;
    case KG_ASSEMBLE:
    	assemble_key_B();
		break;
    case KG_KEY_READY:
    	sendHash();
		break;
    case KG_WAIT_HASH_KEY:
    	if(hashReceived){
    		hashReceived = 0;
            verifyHash();
            kg_currentState = KG_COLLECTING_RSSI;
            reInitKG_B();
            memset(ready_key, 0, KG_OUT_KEY_LEN*sizeof(ready_key[0]));
            current_key_index=0;
    	}
    	else{
    		LOG_INFO("Waiting hash\n");
    	}
    	break;
	default:

		break;
	}

    return kg_currentState;
}





void set_address(const uip_ipaddr_t *ipaddr, struct simple_udp_connection *conn)
{
	memcpy(kg_dest_ipaddr.u8, ipaddr->u8, 16);
	memcpy(&kg_udp_conn, conn, sizeof(kg_udp_conn));
}



uint8_t readMessage(uint8_t *inData, uint16_t dataLen, const uip_ipaddr_t *sender_addr)
{

	switch(inData[0])
	{
	case SAMPLE_SEQ_MSG:
		LOG_INFO("received SAMPLE_SEQ_MSG len %u\n", dataLen);
		memcpy(channel_sample_pair, &inData[1], KG_SAMPLE_SEQ_LEN);
		sampleSequenceReceived++;
		return SAMPLE_SEQ_MSG;
		break;
	case ENC_MSG:
		LOG_INFO("received ENC_MSG len %u\n", dataLen);
		encodeReceived++;
		if(dataLen < KG_ENC_LEN)
		{
			memcpy(encoded_message, &inData[1], dataLen-1);
		}
		break;
	case DATA_TRAFFIC_MSG:
		collectRSSI = inData[1];
		break;
	case DISCARD_MSG:
		LOG_INFO("received DISCARD_MSG len %u\n", dataLen);
		discardReceived++;
		key_seq_len = inData[1];
		discard_count_msg = (uint16_t)inData[2];
		memcpy(discarded_bits, &inData[3], discard_count_msg);

		break;
	case KEY_READY_HASH_MSG:
		LOG_INFO("received KEY_READY_HASH_MSG len %u\n", dataLen);
		hashReceived++;
		memcpy(kg_received_digest, &inData[1], 32);

		//Temporary patch only for test. Compare ready_key
		memcpy(received_ready_key, &inData[33], 16);

		break;
	default:

		break;
	}

	return 0;
}



void kg_collect_rssi(uint8_t index, int8_t rssi, uint8_t messageType)
{
	//key_gen_head = 1 -> Send initial message and receiving ack
	//messageType = 0 -> on TSCH Acknowledge
	//messageType = 1 -> on receiving a message

	if((collectRSSI == 0) && (rssi_num_samples == 0))
	{
		return;
	}

	if( ((key_gen_head == 1) && (messageType == 1)) ||
		((key_gen_head == 0) && (messageType == 0)) )
	{
		return;
	}

	kg_rssi[(index%KG_RSSI_SEQ_LEN)] = -1*rssi;

	rssi_num_samples++;

	if((key_gen_head == 1) && (rssi_num_samples == KG_MAX_RSSI_SAMPLES))
	{
		collectRSSI = 0;
	}

	return;
}





