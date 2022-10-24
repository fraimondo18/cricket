
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/


#ifndef NRF52840_TARGET
#define SICSLOWPAN_CONF_FRAG  0
#define QUEUEBUF_CONF_NUM     1
#define UIP_CONF_BUFFER_SIZE  140
#endif



#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_NONE



/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
/** @} */
