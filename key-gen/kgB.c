
#include <stdlib.h>

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "keyGenDefs.h"

#include "packetbuf.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static uint8_t dest_addr_set = 0;

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);



static struct simple_udp_connection udp_conn;

/*---------------------------------------------------------------------------*/

static void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

	if(dest_addr_set == 0){
		LOG_INFO("set address B\n");
		dest_addr_set=1;
        set_address(sender_addr, &udp_conn);
	}

	readMessage((uint8_t *)data, datalen, sender_addr);

	setOtherSideConnected(1);
}



/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
	static struct etimer periodic_timer;

	PROCESS_BEGIN();

	/* Initialize DAG root */
	NETSTACK_ROUTING.root_start();

	initKG_B();

	simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
			UDP_CLIENT_PORT, udp_rx_callback);


	etimer_set(&periodic_timer, 10* CLOCK_SECOND);

	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

		KG_sm_B();

		etimer_set(&periodic_timer, 2 * CLOCK_SECOND);

	}

	PROCESS_END();

}
/*---------------------------------------------------------------------------*/
