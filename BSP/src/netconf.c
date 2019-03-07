
#include "netconf.h"
#include "lwip/tcpip.h"

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;

static void LwIP_TCP_Init_CallBack(void *arg);

void LwIP_Init(void)
{
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( LwIP_TCP_Init_CallBack, NULL );
	/* add the network interface (IPv4/IPv6) with RTOS */
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
}	


static void LwIP_TCP_Init_CallBack(void *arg)
{
	
}



void LwIP_ETH_Connect(void)
{
	err_t status;

  /* Registers the default network interface */
  netif_set_default(&gnetif);
	
	/* Set device state link up */
	netif_set_link_up(&gnetif);
	
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
		printf("netif set up succeed \n");
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
		printf("netif set up error \n");
  }

  /* Start DHCP negotiation for a network interface (IPv4) */
  status = dhcp_start(&gnetif);
	if(status == ERR_OK)
		printf("DHCP START is succeed \n");
	else
		printf("DHCP START is false %d\n",status);
}

void LwIP_ETH_DisConnect(void)
{
	/* Stop DHCP negotiation for a network interface (IPv4) */
	dhcp_stop(&gnetif);

	/* Set device state link down */
	netif_set_link_down(&gnetif);
	
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
		printf("netif set down error \n");
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
		printf("netif set down succeed \n");
  }
}

