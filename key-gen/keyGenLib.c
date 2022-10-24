#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef KG_SOCKET
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#define LOG_MODULE "KG"
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#include "project-conf.h"
#include "keyGenDefs.h"

#ifdef KG_SOCKET
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif




#ifndef KG_RSSI_COLLECT
/*int8_t rssi_data_test[1024] = {
		-70, -70, -69, -67, -66, -66, -69, -64, -70, -71, -64, -66, -71, -67, -66, -64, -80, -70, -66, -71, -70, -71, -70, -71, -67, -64, -64, -64, -65, -64, -76, -74, -64, -72, -67, -64, -65,
		-61, -64, -70, -68, -65, -69, -61, -63, -59, -70, -67, -70, -74, -64, -62, -61, -62, -67, -64, -63, -66, -61, -61, -66, -63, -63, -65, -70, -63, -69, -64, -64, -71, -73, -67, -70, -66,
		-70, -73, -70, -67, -71, -69, -70, -70, -68, -78, -67, -71, -70, -73, -70, -71, -70, -68, -65, -68, -68, -67, -67, -70, -67, -72, -67, -67, -67, -65, -64, -71, -68, -67, -83, -82, -66,
		-69, -68, -64, -64, -68, -68, -70, -68, -72, -70, -67, -75, -68, -69, -72, -65, -71, -64, -66, -64, -74, -67, -73, -64, -66, -67, -68, -71, -70, -65, -66, -64, -64, -74, -64, -64, -62,
		-67, -61, -63, -64, -63, -67, -64, -62, -63, -64, -62, -68, -64, -72, -70, -65, -69, -65, -68, -64, -67, -67, -65, -65, -64, -67, -70, -65, -64, -66, -67, -68, -71, -72, -70, -70, -72,
		-69, -66, -67, -69, -70, -70, -71, -64, -67, -64, -64, -65, -69, -63, -64, -64, -66, -63, -67, -66, -64, -64, -64, -64, -66, -66, -65, -65, -64, -64, -64, -66, -66, -65, -70, -64, -66,
		-61, -62, -64, -64, -66, -73, -66, -69, -68, -67, -67, -67, -66, -65, -66, -67, -65, -66, -67, -65, -66, -65, -65, -65, -64, -64, -64, -65, -66, -65, -65, -66, -67, -67, -66, -66, -67,
		-66, -67, -70, -67, -66, -67, -67, -66, -66, -69, -66, -65, -67, -65, -65, -66, -67, -66, -67, -67, -66, -65, -66, -65, -65, -65, -66, -64, -65, -65, -65, -66, -68, -70, -70, -70, -70,
		-68, -69, -70, -69, -70, -70, -69, -69, -69, -69, -70, -70, -70, -68, -66, -67, -67, -67, -67, -67, -67, -67, -67, -67, -68, -67, -67, -68, -68, -69, -69, -69, -68, -70, -67, -68, -69,
		-68, -69, -66, -66, -65, -66, -66, -67, -68, -67, -70, -70, -71, -71, -69, -65, -67, -66, -66, -66, -67, -66, -66, -67, -67, -67, -67, -67, -68, -66, -66, -66, -66, -66, -66, -67, -67,
		-67, -68, -70, -70, -68, -67, -67, -67, -67, -70, -66, -67, -70, -69, -69, -66, -67, -67, -68, -67, -67, -68, -67, -67, -67, -66, -65, -66, -67, -67, -69, -66, -67, -65, -69, -74, -70,
		-76, -67, -70, -67, -65, -64, -67, -69, -70, -66, -70, -67, -69, -66, -67, -67, -63, -67, -70, -68, -68, -69, -68, -70, -67, -69, -70, -72, -72, -70, -73, -71, -76, -71, -70, -69, -72,
		-69, -67, -70, -69, -72, -67, -70, -69, -68, -68, -69, -68, -70, -67, -67, -68, -67, -68, -68, -68, -69, -70, -70, -67, -70, -70, -66, -66, -64, -65, -65, -64, -65, -67, -66, -66, -67,
		-67, -65, -65, -65, -64, -65, -67, -65, -65, -64, -67, -67, -69, -68, -70, -69, -68, -69, -68, -66, -66, -64, -67, -66, -67, -67, -70, -68, -67, -64, -73, -70, -59, -59, -65, -64, -61,
		-66, -63, -61, -61, -61, -63, -62, -64, -67, -69, -64, -65, -63, -61, -63, -64, -67, -65, -63, -65, -66, -66, -59, -65, -61, -66, -59, -60, -60, -59, -59, -60, -62, -63, -62, -61, -59,
		-65, -61, -68, -65, -67, -73, -64, -59, -67, -65, -76, -66, -63, -61, -65, -65, -64, -64, -64, -68, -66, -64, -66, -61, -63, -64, -63, -63, -62, -61, -63, -64, -64, -61, -61, -63, -62,
		-63, -65, -65, -64, -64, -65, -67, -66, -65, -63, -61, -61, -65, -64, -63, -62, -62, -64, -63, -62, -61, -64, -62, -64, -62, -63, -61, -61, -62, -62, -62, -63, -63, -62, -62, -65, -62,
		-64, -62, -62, -64, -66, -63, -64, -64, -64, -65, -64, -64, -63, -64, -65, -64, -65, -66, -65, -64, -64, -64, -64, -65, -64, -64, -64, -63, -63, -62, -63, -64, -64, -64, -64, -62, -63,
		-62, -61, -61, -63, -63, -62, -64, -65, -65, -63, -64, -64, -64, -64, -64, -65, -64, -64, -63, -64, -64, -64, -64, -64, -64, -64, -63, -63, -64, -65, -63, -64, -64, -64, -63, -64, -64,
		-64, -63, -62, -63, -61, -61, -61, -62, -64, -63, -61, -61, -62, -61, -61, -64, -64, -61, -62, -61, -62, -64, -63, -63, -63, -64, -64, -64, -63, -62, -63, -63, -63, -63, -63, -63, -62,
		-63, -62, -62, -62, -64, -64, -63, -64, -64, -64, -64, -64, -64, -64, -64, -64, -63, -61, -62, -61, -62, -62, -63, -62, -62, -62, -62, -63, -64, -63, -62, -63, -64, -64, -63, -63, -64,
		-64, -64, -64, -63, -64, -64, -64, -64, -64, -64, -64, -64, -63, -64, -64, -64, -64, -64, -64, -64, -64, -63, -64, -64, -64, -64, -63, -63, -63, -63, -62, -62, -62, -63, -62, -63, -62,
		-63, -63, -64, -63, -63, -63, -64, -63, -64, -64, -64, -64, -64, -64, -64, -64, -63, -63, -63, -63, -63, -63, -63, -63, -63, -62, -62, -62, -63, -64, -64, -64, -63, -63, -63, -63, -62,
		-63, -63, -63, -63, -63, -64, -63, -63, -64, -64, -64, -64, -63, -64, -64, -64, -64, -63, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64, -64,
		-64, -64, -66, -64, -61, -64, -64, -65, -64, -63, -64, -63, -64, -63, -64, -64, -64, -64, -64, -63, -64, -64, -63, -64, -63, -64, -64, -64, -64, -64, -65, -63, -67, -67, -61, -65, -63,
		-64, -61, -63, -62, -64, -59, -64, -61, -61, -62, -60, -61, -62, -60, -63, -62, -61, -61, -64, -64, -63, -61, -67, -62, -68, -69, -70, -65, -63, -64, -67, -61, -63, -64, -61, -61, -62,
		-61, -65, -62, -65, -64, -65, -63, -61, -63, -62, -64, -64, -63, -66, -62, -66, -62, -68, -59, -61, -70, -64, -63, -61, -59, -61, -64, -66, -64, -70, -69, -64, -66, -61, -61, -61, -66,
		-74, -69, -64, -66, -61, -61, -61, -66, -74, -69, -64, -66, -61, -61, -61, -66, -74, -69, -64, -66, -61, -61, -61, -66, -74};*/

const int8_t rssi_data_test[100] = {
		-70, -70, -69, -67, -66, -66, -69, -64, -70, -71, -64, -66, -71, -67, -66, -64, -80, -70, -66, -71,
		-70, -71, -70, -71, -67, -64, -64, -64, -65, -64, -76, -74, -64, -72, -67, -64, -65, -61, -64, -70,
		-68, -65, -69, -61, -63, -59, -70, -67, -70, -74, -64, -62, -61, -62, -67, -64, -63, -66, -61, -61,
		-66, -63, -63, -65, -70, -63, -69, -64, -64, -71, -73, -67, -70, -66, -70, -73, -70, -67, -71, -69,
		-70, -70, -68, -78, -67, -71, -70, -73, -70, -71, -70, -68, -65, -68, -68, -67, -67, -70, -67, -72};
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




uint8_t kg_m_digest[32];
static uint8_t kg_received_digest[32];



/////////////////   general variables  /////////////////
static uint16_t kg_mathIndex = 0, kg_varIterations=0;
static uint8_t kg_currentState;



/////////////////   MAC layer variables  /////////////////
static uint16_t rssi_num_samples = 0;
static uint16_t rssi_block = 0;
static uint8_t key_gen_head;
static uint8_t stop_rssi_collection=0;


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
static uint16_t nn_current_index = 0;
static float average;




/////////////////   key generation   /////////////////#
static uint8_t key_index_count = 0;



#ifdef KG_SOCKET
extern int kg_sock, kg_fd, addrlen;;
extern struct sockaddr_in address;
#endif

/////////////////   encoding & discaring bits  /////////////////
// to go
// uint16_t discarded_bits[KG_OUT_KEY_LEN] = {0}; // the length of that array varies based on the length of the inital key
// to go
// uint8_t encoded_message[KG_OUT_KEY_LEN*KG_BLOCK_SIZE/KG_BITS_PU]; // the length of that array varies based on the length of the inital key



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
static uint8_t otherSideConnected = 0;

struct simple_udp_connection kg_udp_conn;
static uip_ipaddr_t kg_dest_ipaddr;


#ifdef KG_USE_ALLOC

uint16_t* discarded_bits=NULL;
uint8_t* encoded_message=NULL;
uint16_t* disc_bits_msg=NULL;
uint8_t* estimate_key=NULL;
uint8_t* key_stretched=NULL;
uint8_t* KEY=NULL;

#else

static uint8_t discarded_bits[DISC_BITS_LEN];
static uint8_t  encoded_message[KG_ENC_LEN];
static uint8_t  KEY[SOURCE_KEY_LEN];
//uint16_t disc_bits_msg[(KG_MAX_INIT_KEY_LEN*sizeof(uint16_t))];
static uint8_t key_stretched[KG_STRETCHED_LEN];
static uint8_t estimate_key[((KG_MAX_INIT_KEY_LEN/KG_BITS_PU))];

static uint8_t key_agreed_bits[KG_OUT_KEY_LEN];

#endif




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


static void compareStrings(uint8_t *string1, uint8_t *string2, uint8_t len)
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

	return;
}




#ifndef KG_RSSI_COLLECT
static void set_rssi_test(uint8_t *data, uint8_t len)
{
	uint8_t i;
	int8_t r_val;

	for(i=0; i< 100; i++)
	{
		r_val = ((rand() % (7)) / 2);
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
    	msgOut[1] = (uint8_t)(rssi_num_samples/255);
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


#ifdef KG_SOCKET
	send(kg_sock, msgOut, msgLen, 0);
#else
	simple_udp_sendto(&kg_udp_conn, msgOut, msgLen, &kg_dest_ipaddr);
#endif

/*	LOG_INFO("send msg %x len %u\n", msgType, msgLen);

	LOG_INFO("to ");
	LOG_INFO_6ADDR(&kg_dest_ipaddr);
	LOG_INFO_("\n");*/

}


static void nearest_neighbor()
{
    float average_series;

    if (rssi_num_samples < KG_WINDOW_SIZE)
    {
        return;
    }

    if( nn_current_index < KG_RSSI_SEQ_LEN )
    {
        values_in_window += 1;
        window_sum += (int32_t)kg_rssi[nn_current_index];

        if (nn_current_index >= KG_WINDOW_SIZE)
        {
            values_in_window -= 1;
            window_sum -= (int32_t)kg_rssi[nn_current_index - KG_WINDOW_SIZE];
        }
    }

    if (nn_current_index >= KG_WINDOW_SIZE/2)
    {
    	average_series =  ((float) window_sum) / ( (float) values_in_window);

        kg_filtered[nn_current_index - (uint16_t)(KG_WINDOW_SIZE/2)] = ((float)kg_rssi[nn_current_index-( (uint16_t) (KG_WINDOW_SIZE/2) )]) - average_series;
        average += kg_filtered[nn_current_index - (uint16_t)(KG_WINDOW_SIZE/2)];
   
      // LOG_INFO("kg_rssi[%u]: %i\n", nn_current_index - (uint16_t)(KG_WINDOW_SIZE/2), kg_rssi[nn_current_index - (uint16_t)(KG_WINDOW_SIZE/2)]);
      // LOG_INFO("average_series (*100): %i\n", (int) (average_series * ((float) 100)));
      // LOG_INFO("filtered value: (*100) %i\n", (int) ( kg_filtered[nn_current_index - (uint16_t)(KG_WINDOW_SIZE/2)] * ((float) 100)));
    }

    nn_current_index++;

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

	LOG_INFO("factor %i\n", (int)(factor*100.0));

	return factor;
}


static void variance()
{
	LOG_INFO("variance\n");

    if(kg_mathIndex == 0)
    {
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
the bit that is to be added to the key stertched: random_bit (it counts how many times a certain bit has to be repeated and then shifts for the bit to be extracted from the initial key)
the_stretch - counts the number of bits that were added to one byte of the stretched key array it resets when we move to a new byte
*/
static void key_stretch(void)
{
	uint32_t i, the_streched_bit_index, the_bit;
	uint8_t random_bit, the_stretch, bit_to_or;

	LOG_INFO("key_stretch\n");

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

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,100);
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

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,100);
#endif

	kg_plat_init();

    return;
}



static void quantize()
{
	uint16_t i, chIdx;
	uint8_t offset, index_value, val;
	offset = (1<<(KG_Q_BITS-1));

	LOG_INFO("quantize\n");

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

    LOG_INFO("mid_rise\n");

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
    while(kg_mathIndex < ((ini_key_len*kg_block_size)/KG_BITS_PU))
    {
        encoded_message[kg_mathIndex] = key_stretched[kg_mathIndex] ^ channel_shuffled[kg_mathIndex + KG_SAMPLE_SEQ_LEN]; // to account for the burned bits
    	kg_mathIndex++;
    	kg_varIterations++;
    	if(kg_varIterations> KG_MATH_NUM_ITERATIONS)
    	{
    		break;
    	}
    }

    if(kg_mathIndex == (ini_key_len*kg_block_size/KG_BITS_PU))
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

        if (kg_mathIndex%kg_block_size==0 && kg_mathIndex>0)
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

    LOG_INFO("ini_key_len %u kg_block_size %u key_index_count %u, discarded_bit_count %u\n", ini_key_len,kg_block_size, key_index_count, discarded_bit_count);

    if(kg_mathIndex == ini_key_len*kg_block_size || key_index_count==128)
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

	LOG_INFO("gen_key\n");

	kg_gen_random(NULL, KEY, 16);

/*    for (i=0; i<16; i++){
    	 print_bin(KEY[i]);
    }
    printf("\n");*/

    key_stretch();
}


static uint8_t look_up_table_A(void)
{   
    float missmatch;
    uint8_t idx, err_sum=0, err_xor, i;

    LOG_INFO("look_up_table_A\n");


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

	kg_tau = kgEncParams[KG_NUM_ENC_PARAMS-1].tau;
	ini_key_len = kgEncParams[KG_NUM_ENC_PARAMS-1].ini_key_len;
	kg_block_size = kgEncParams[KG_NUM_ENC_PARAMS-1].block_size;
    for(i=0; i<KG_NUM_ENC_PARAMS; i++)
    {
    	if(missmatch <= kgEncParams[i].nissmatch)
    	{
    		kg_tau = kgEncParams[i].tau;
    		ini_key_len = kgEncParams[i].ini_key_len;
    		kg_block_size = kgEncParams[i].block_size;
    		break;
    	}
    }


    LOG_INFO("missmatch: %i%% ,kg_tau: %i, kg_block_size: %i,ini_key_len: %i\n", (int)(missmatch*100), kg_tau, kg_block_size, ini_key_len);

    // decalre global function based on the parameters
#ifdef KG_USE_ALLOC
    discarded_bits = malloc(ini_key_len);
    if(discarded_bits==NULL)
    {
        LOG_ERR("Failed malloc: discarded_bits\n");
        return 1;
    } 

    encoded_message = malloc((ini_key_len*kg_block_size/KG_BITS_PU)*sizeof(uint8_t));
    if(encoded_message==NULL)
    {
    	LOG_ERR("Failed malloc: encoded_message\n");
        return 1;
    } 

    KEY = malloc((ini_key_len/KG_BITS_PU)*sizeof(uint8_t));
    if(KEY==NULL)
    {
    	LOG_ERR("Failed malloc: KEY\n");
        return 1;
    } 

/*    disc_bits_msg = malloc(ini_key_len*sizeof(uint16_t));
    if(disc_bits_msg==NULL)
    {
    	LOG_ERR("Failed malloc: disc_bits_msg\n");
        return 1;
    } */

    key_stretched = malloc((ini_key_len*kg_block_size/KG_BITS_PU)*sizeof(uint8_t));
    if(key_stretched==NULL)
    {
    	LOG_ERR("Failed malloc: key_stretched\n");
        return 1;
    } 
#endif


    gen_key();


    return 0;
    
}



static void look_up_table_B(void)
{
    float missmatch;
    uint8_t idx, err_sum=0, err_xor, i;

    LOG_INFO("look_up_table_B\n");

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

	kg_tau = kgEncParams[KG_NUM_ENC_PARAMS-1].tau;
	ini_key_len = kgEncParams[KG_NUM_ENC_PARAMS-1].ini_key_len;
	kg_block_size = kgEncParams[KG_NUM_ENC_PARAMS-1].block_size;
    for(i=0; i<KG_NUM_ENC_PARAMS; i++)
    {
    	if(missmatch <= kgEncParams[i].nissmatch)
    	{
    		kg_tau = kgEncParams[i].tau;
    		ini_key_len = kgEncParams[i].ini_key_len;
    		kg_block_size = kgEncParams[i].block_size;
    		break;
    	}
    }


    LOG_INFO("missmatch: %i%% ,kg_tau: %i, kg_block_size: %i,ini_key_len: %i\n", (int)(missmatch*100), kg_tau, kg_block_size, ini_key_len);


#ifdef KG_USE_ALLOC
    encoded_message = malloc((ini_key_len*kg_block_size/KG_BITS_PU)*sizeof(uint8_t));

    discarded_bits = malloc((ini_key_len+3)*sizeof(uint16_t));

    estimate_key = malloc((ini_key_len/KG_BITS_PU)*sizeof(uint8_t));
#endif
}




static void shuffle()
{
    uint8_t current_shift, shuffled_shift, shuffled_byte_idx, current_byte_idx, curr_bit, shuffled_bit;
    
    LOG_INFO("shuffle\n");

    kg_varIterations = 0;
    
    uint8_t rand_idx[KG_CH_SEQ_LEN*KG_BITS_PU] = {
    		174, 70, 198, 176, 107, 15, 166, 135, 3, 73, 164, 153, 86, 187, 74, 58, 77, 42, 45, 108,
    		45, 182, 169, 105, 64, 71, 196, 87, 48, 78, 116, 171, 132, 134, 131, 127, 33, 189, 89, 193,
			151, 198, 80, 116, 77, 76, 186, 63, 93, 58, 95, 135, 116, 42, 74, 17, 67, 40, 46, 186,
			147, 7, 184, 86, 184, 162, 60, 161, 173, 98, 11, 170, 89, 72, 4, 96, 169, 105, 2, 106,
			11, 65, 36, 7, 91, 155, 26, 114, 156, 59, 87, 11, 5, 146, 13, 161, 65, 168, 67, 152,
			140, 9, 149, 189, 172, 8, 84, 134, 59, 124, 48, 103, 28, 134, 6, 32, 175, 106, 134, 36,
			51, 74, 133, 93, 126, 137, 1, 141, 148, 166, 198, 173, 102, 93, 181, 193, 153, 130, 97, 24,
			186, 66, 175, 1, 55, 63, 88, 53, 48, 168, 67, 62, 110, 157, 124, 157, 89, 117,16, 58,
			146, 135, 43, 132, 51, 6, 105, 37, 105, 180, 165, 132, 143, 87, 36, 86, 117, 108, 34, 24,
			88, 49, 156, 80, 74, 154, 159, 36, 185, 162, 100,187, 76, 43, 99, 195, 28, 147, 121, 80};
    

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
            channel_shuffled[current_byte_idx] ^= 1<<current_shift;
            channel_shuffled[shuffled_byte_idx] ^= 1<<shuffled_shift;
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

    stop_rssi_collection=0;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,100);
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

    stop_rssi_collection=0;

#ifndef KG_RSSI_COLLECT
	set_rssi_test((uint8_t *)rssi_data_test,100);
#endif
}



static void assemble_key_A()
{
    uint8_t key_bit_idx, agreed_key_byte, agreed_key_bit, agreed_key_shift;
    uint8_t key_byte, key_shift;
    int i;

    LOG_INFO("assemble_key_A\n");

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
            key_bit_idx = 0;
            kg_currentState = KG_COLLECTING_RSSI;
            reInitKG_A();
            break;
        }
    }

    LOG_INFO("current_key_index %u\n", current_key_index);

    if (current_key_index==KG_OUT_KEY_LEN)
    {
        kg_currentState = KG_KEY_READY;
        stop_rssi_collection=1;

        LOG_INFO("KEY ready\n");
    }
    else
    {
    	LOG_INFO("partial key updated\n");
    }


    for (i=0; i<((uint8_t) (KG_OUT_KEY_LEN/8)); i++){
    	 print_bin(ready_key[i]);
    }
    printf("\n");
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
			key_bit_idx = 0;
			break;
		}
	}

	LOG_INFO("current_key_index %u\n", current_key_index);

	if (current_key_index==KG_OUT_KEY_LEN)
	{
		kg_currentState = KG_KEY_READY;
		stop_rssi_collection=1;

		LOG_INFO("KEY ready\n");
	}
	else
	{
		kg_currentState = KG_COLLECTING_RSSI;
		reInitKG_B();

		LOG_INFO("partial key updated\n");
	}

	for (i=0; i<((uint8_t) (KG_OUT_KEY_LEN/8)); i++){
		print_bin(ready_key[i]);
	}
	printf("\n");

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

	calcHash(ready_key, 16, kg_m_digest, &digest_len);

	if(memcmp(kg_received_digest, kg_m_digest, 32) == 0){
		LOG_INFO("Hash key correct\n");
	}
	else{
		LOG_INFO("Hash key different\n");
	}


    //compareStrings(kg_received_digest, kg_m_digest, 32);
}


uint8_t KG_sm_A() // has to be change depending on how channel sample is sent
{
	uint8_t msgOut[1024];

	//LOG_INFO("KG_sm_A state %u\n", kg_currentState);

	if(kg_currentState< KG_SAMPLE){
		sendMsg(DATA_TRAFFIC_MSG, msgOut, NULL, 0);
	}

	switch(kg_currentState)
	{
	case KG_COLLECTING_RSSI:

		//LOG_INFO("rssi_num_samples %u\n", rssi_num_samples);

		nearest_neighbor();

		if(nn_current_index == KG_RSSI_SEQ_LEN)
        {
			kg_currentState = KG_VARIANCE;
            nn_current_index = 0;
		}
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
		if(look_up_table_A() != 0){
            kg_currentState = KG_ERROR;
        }
		else{
			kg_currentState = KG_ENCODE;
		}
		break;

	case KG_ENCODE:
		encode();
		break;

	case KG_ENCODE_READY:
		sendMsg(ENC_MSG, msgOut, encoded_message, (ini_key_len*kg_block_size/KG_BITS_PU));

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



uint8_t KG_sm_B() // has to be change depending on how channel sample is sent
{
	uint8_t msgOut[1024];

	//LOG_INFO("KG_sm_B state %u\n", kg_currentState);

	switch(kg_currentState)
	{
	case KG_COLLECTING_RSSI:

		//LOG_INFO("rssi_num_samples %u\n", rssi_num_samples);

		nearest_neighbor();

		if(nn_current_index == KG_RSSI_SEQ_LEN)
        {
			kg_currentState = KG_VARIANCE;
            nn_current_index = 0;
		}
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
    		kg_currentState = KG_WAIT_ENCODED_MSG;
    	}
    	else{
    		LOG_INFO("Waiting sample seq\n");
    	}
		break;
    case KG_WAIT_ENCODED_MSG:
    	if(encodeReceived == 1)
    	{
    		encodeReceived = 0;
    		kg_currentState = KG_TABLE;
    	}
    	else{
    		LOG_INFO("Waiting encoded msg\n");
    	}
    	break;
    case KG_TABLE:
        look_up_table_B();
        kg_currentState = KG_DECODE;
		break;
	case KG_DECODE: // will happen upon receiveing encrypted message from Alice
                    // and the look-up table being ready. The inteval between receiving
                    // the encrypted message and the channel sample should be long
                    // enough to assume that once we receive encrypted message look-up
                    // table is already present
		decode();

		//In this case the length of the message is inside the array, we do not use the last parameter
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
		rssi_block = inData[0];
		break;
	case DISCARD_MSG:
		LOG_INFO("received DISCARD_MSG len %u\n", dataLen);
		discardReceived++;
		key_seq_len = inData[1];
		discard_count_msg = (uint16_t)inData[2];
		memcpy(discarded_bits, &inData[3], discard_count_msg);

		//LOG_DBG("key_seq_len %u discard_count_msg %u\n", key_seq_len, discard_count_msg);
		break;
	case KEY_READY_HASH_MSG:
		LOG_INFO("received KEY_READY_HASH_MSG len %u\n", dataLen);
		hashReceived++;
		memcpy(kg_received_digest, &inData[1], 32);

		//Temporary patch only for test. Compare ready_key
		compareStrings(&inData[33], ready_key, 16);

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

	static uint8_t index_offset=0;
	uint8_t rssi_sample_index = 0;

	if( (otherSideConnected == 0) || (stop_rssi_collection==1))
	{
		return;
	}

	if( ((key_gen_head == 1) && (messageType == 1)) ||
		((key_gen_head == 0) && (messageType == 0)) )
	{
		return;
	}

	if(rssi_num_samples == 0)
	{
		index_offset = index;
	}

	if(index >= index_offset){
		rssi_sample_index = (index - index_offset);
	}
	else{
		rssi_sample_index = (255 - index_offset + index);
	}


	rssi_num_samples++;
	if((key_gen_head == 1) && (rssi_sample_index>0)){
		kg_rssi[rssi_sample_index-1] = -1*rssi;
	}
	else{
		kg_rssi[rssi_sample_index] = -1*rssi;
	}


	if(rssi_num_samples == KG_RSSI_SEQ_LEN+1){
		stop_rssi_collection = 1;
	}


	return;
}

void setOtherSideConnected(uint8_t state)
{
	otherSideConnected = state;
}




#ifdef KG_SOCKET
uint8_t waitResponse(uint8_t msgType, uint16_t dataLen)
{
    uint16_t msgLen = 0;
    uint8_t valread;
    uint8_t buffer[2048];

    printf("\nwaitResponse %u\n", msgType);


    if(key_gen_head == 1)
    {
       // printf("\nwaiting to authenticate server;\n");
    	if ((kg_sock = accept(kg_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
    		perror("accept");
    		exit(EXIT_FAILURE);
    	}
    }
    printf("\nwaiting for a new message\n");
    valread = read(kg_sock, buffer, dataLen);


    switch(buffer[0])
    {
    case SAMPLE_SEQ_MSG:
    	printf("Received SAMPLE_SEQ_MSG\n");
    	memcpy(channel_sample_pair, &buffer[1], KG_SAMPLE_SEQ_LEN);
        return SAMPLE_SEQ_MSG;
    	break;
    case ENC_MSG:
    	printf("Received ENC_MSG\n");
    	memcpy(encoded_message, &buffer[1], ini_key_len*kg_block_size/KG_BITS_PU);
    	break;
    case DATA_TRAFFIC_MSG:
    	printf("Received DATA_TRAFFIC_MSG\n");
    	break;
    case DISCARD_MSG:
    	printf("Received DISCARD_MSG\n");
    	key_seq_len = discarded_bits[1];
    	discard_count_msg = (uint16_t)discarded_bits[2];
    	memcpy(discarded_bits, &buffer[3], discard_count_msg);

        return DISCARD_MSG;
    	break;
    default:

    	break;
    }


    return 0;

}

#endif

