#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "packetbuf.h"
#include "project-conf.h"

#include "keyGenDefs.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

#define SEND_INTERVAL		  (10 * CLOCK_SECOND)


static uint8_t dest_addr_set = 0;


static struct simple_udp_connection udp_conn;
/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

	readMessage((uint8_t *)data, datalen, sender_addr);

}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
	static struct etimer periodic_timer;
	uip_ipaddr_t dest_ipaddr;

	PROCESS_BEGIN();

	initKG_A();

	simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
			UDP_SERVER_PORT, udp_rx_callback);

	etimer_set(&periodic_timer, 10* CLOCK_SECOND);


	while(1)
	{
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

		if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {

			if(dest_addr_set == 0){
				set_address(&dest_ipaddr, &udp_conn);
				dest_addr_set = 1;
			}

			//LOG_INFO("reach\n");

			setOtherSideConnected(1);
			KG_sm_A();

		}
		else
		{
			setOtherSideConnected(0);
			LOG_INFO("Not reach\n");
		}
		
		etimer_set(&periodic_timer, 2 * CLOCK_SECOND);
	}


	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
