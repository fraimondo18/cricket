
/*---------------------------------------------------------------------------*/
#ifndef KEY_GEN_DEFS_H_
#define KEY_GEN_DEFS_H_
/*---------------------------------------------------------------------------*/


#include "uip.h"
#include "simple-udp.h"



#define KG_NUM_ENC_PARAMS (30)

#define KG_BITS_PU (8)
#define KG_SEQ_SAMPLE_LEN (64)
#define KG_Q_BITS  (2)
#define KG_MAX_BLOCK_SIZE  (22)
#define KG_MAX_INIT_KEY_LEN  (16)

#define KG_RSSI_LEN ((KG_MAX_BLOCK_SIZE*KG_MAX_INIT_KEY_LEN)/KG_Q_BITS)

// number of rssi readings + the channel smaple offset
#define KG_RSSI_SEQ_LEN ((KG_SEQ_SAMPLE_LEN/KG_Q_BITS) + KG_RSSI_LEN)
#define KG_OUT_KEY_LEN (128)
#define KG_KEY_SEG_LEN (8)
#define KG_KEY_SEG_NUM (KG_OUT_KEY_LEN/KG_KEY_SEG_LEN)
#define KG_VAR_FACTOR (0.7)
#define KG_TAU (2)
#define KG_BLOCK_SIZE (9)
#define KG_WINDOW_SIZE (4)


#define KG_MAX_RSSI_SAMPLES (256)

#define KG_SAMPLE_SEQ_LEN (KG_SEQ_SAMPLE_LEN/KG_BITS_PU)

#define KG_CH_SEQ_LEN ((KG_RSSI_SEQ_LEN*KG_Q_BITS)/KG_BITS_PU)

//#define KG_STRETCHED_LEN (((KG_MAX_INIT_KEY_LEN*KG_MAX_BLOCK_SIZE)/KG_BITS_PU))
#define KG_STRETCHED_LEN (256)

//#define KG_ENC_LEN (((KG_MAX_INIT_KEY_LEN*KG_MAX_BLOCK_SIZE)/KG_BITS_PU))
#define KG_ENC_LEN (64)

//#define DISC_BITS_LEN KG_MAX_INIT_KEY_LEN
#define DISC_BITS_LEN (256)

//#define SOURCE_KEY_LEN ((KG_MAX_INIT_KEY_LEN/KG_BITS_PU))
#define SOURCE_KEY_LEN (64)





#define ENC_MSG (0xf0)
#define DATA_TRAFFIC_MSG (0xf1)
#define SAMPLE_SEQ_MSG (0xf2)
#define DISCARD_MSG (0xf3)
#define KEY_READY_HASH_MSG (0xf4)

#define KG_MATH_NUM_ITERATIONS (50)



typedef enum keyGen_SM
{
	KG_COLLECTING_RSSI=0,
	KG_VARIANCE=1,
	KG_MID_RISE=2,
	KG_QUANT=3,
    KG_SHUFFLE=4,
    KG_SAMPLE=5,
    KG_TABLE=6,
    KG_ENCODE=7,
    KG_ENCODE_READY=8,
    KG_WAIT_DISCARDED=9,
    KG_DISCARD=10,
    KG_KEY_READY=11,
	KG_DECODE=12,
    KG_ASSEMBLE=13,
    KG_WAIT_HASH_KEY=14,
	KG_SAMPLE_RESPONSE=15,
	KG_WAIT_ENCODED_MSG=16,
    KG_ERROR=17
} keyGen_SM_t;


typedef struct kgEncodeParams
{
	float mismatch;
	uint8_t block_size;
	uint8_t tau;
	uint8_t ini_key_len;
} kgEncodeParams_t;




void initKG_A(void);
void initKG_B(void);
uint8_t KG_sm_A(void);
uint8_t KG_sm_B(void);
uint8_t waitResponse(uint8_t msgType, uint16_t dataLen);
uint8_t readMessage(uint8_t *inData, uint16_t dataLen, const uip_ipaddr_t *sender_addr);
void set_address(const uip_ipaddr_t *ipaddr, struct simple_udp_connection *conn);

void kg_plat_init();
int kg_gen_random(void *p_rng, unsigned char *output, size_t output_len);
void kg_calcHash(uint8_t *inData, uint8_t dataLen, uint8_t *outData, size_t *outDataLen);

/*---------------------------------------------------------------------------*/
#endif /* KEY_GEN_DEFS_H_ */
/*---------------------------------------------------------------------------*/
/** @} */
