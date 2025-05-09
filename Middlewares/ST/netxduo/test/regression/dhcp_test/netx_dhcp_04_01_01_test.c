/* A client that can't receive unicast IP datagrams until its protocol software has been configured with an IP address SHOULD set the BROADCAST 
 * bit in the 'flags' field to 1 in any DHCPDISCOVER or DHCPREQUEST messages that client sends.
 * rfc 2131, page 25, 4.1 Constructing and sending DHCP messages
 * 
 */

#include   "tx_api.h"
#include   "nx_api.h"
#include   "nxd_dhcp_client.h"
#include   "nxd_dhcp_server.h" 
#include   "netx_dhcp_clone_function.h"

extern void    test_control_return(UINT status);

#if !defined(NX_DISABLE_IPV4)

#define     DEMO_STACK_SIZE             4096
#define     NX_PACKET_SIZE              1536
#define     NX_PACKET_POOL_SIZE         NX_PACKET_SIZE * 8

#define     NX_DHCP_SERVER_IP_ADDRESS_0 IP_ADDRESS(10,0,0,1)   
#define     START_IP_ADDRESS_LIST_0     IP_ADDRESS(10,0,0,10)
#define     END_IP_ADDRESS_LIST_0       IP_ADDRESS(10,0,0,19)

#define     NX_DHCP_SUBNET_MASK_0       IP_ADDRESS(255,255,255,0)
#define     NX_DHCP_DEFAULT_GATEWAY_0   IP_ADDRESS(10,0,0,1)
#define     NX_DHCP_DNS_SERVER_0        IP_ADDRESS(10,0,0,1)


/* Define the ThreadX and NetX object control blocks...  */
static TX_THREAD               client_thread;
static NX_PACKET_POOL          client_pool;
static NX_IP                   client_ip;
static NX_DHCP                 dhcp_client;

static TX_THREAD               server_thread;
static NX_PACKET_POOL          server_pool;
static NX_IP                   server_ip;
static NX_DHCP_SERVER          dhcp_server;

/* Define the counters used in the demo application...  */

static ULONG                   error_counter;
static CHAR                    *pointer;

static UINT                    discover_flag;
static UINT                    request_flag;

/* Define thread prototypes.  */

static void    server_thread_entry(ULONG thread_input);
static void    client_thread_entry(ULONG thread_input);

/******** Optionally substitute your Ethernet driver here. ***********/
extern void    _nx_ram_network_driver_1500(struct NX_IP_DRIVER_STRUCT *driver_req);
static void    my_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);

/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_04_01_01_test_application_define(void *first_unused_memory)
#endif
{

UINT    status;


    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Create the client thread.  */
    tx_thread_create(&client_thread, "thread client", client_thread_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Create the server thread.  */
    tx_thread_create(&server_thread, "thread server", server_thread_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create the client packet pool.  */
    status =  nx_packet_pool_create(&client_pool, "NetX Main Packet Pool", 1024, pointer, NX_PACKET_POOL_SIZE);
    pointer = pointer + NX_PACKET_POOL_SIZE;

    /* Check for pool creation error.  */
    if (status)
        error_counter++;
    
    /* Create the server packet pool.  */
    status =  nx_packet_pool_create(&server_pool, "NetX Main Packet Pool", 1024, pointer, NX_PACKET_POOL_SIZE);
    pointer = pointer + NX_PACKET_POOL_SIZE;

    /* Check for pool creation error.  */
    if (status)
        error_counter++;

    /* Create an IP instance for the DHCP Client.  */
    status = nx_ip_create(&client_ip, "DHCP Client", IP_ADDRESS(0, 0, 0, 0), 0xFFFFFF00UL, &client_pool, _nx_ram_network_driver_1500, pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
        error_counter++;
    
    /* Create an IP instance for the DHCP Server.  */
    status = nx_ip_create(&server_ip, "DHCP Server", NX_DHCP_SERVER_IP_ADDRESS_0, 0xFFFFFF00UL, &server_pool, _nx_ram_network_driver_1500, pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for DHCP Client IP.  */
    status =  nx_arp_enable(&client_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;
    
    /* Enable ARP and supply ARP cache memory for DHCP Server IP.  */
    status =  nx_arp_enable(&server_ip, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;

    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&client_ip);

    /* Check for UDP enable errors.  */
    if (status)
        error_counter++;
    
    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&server_ip);

    /* Check for UDP enable errors.  */
    if (status)
        error_counter++;

    /* Enable ICMP.  */
    status =  nx_icmp_enable(&client_ip);

    /* Check for errors.  */
    if (status)
        error_counter++;

    /* Enable ICMP.  */
    status =  nx_icmp_enable(&server_ip);

    /* Check for errors.  */
    if (status)
        error_counter++;

    return;
}

/* Define the test threads.  */

void    server_thread_entry(ULONG thread_input)
{

UINT        status;
UINT   	    iface_index;
UINT        addresses_added;

    printf("NetX Test:   DHCP 04_01_01 Test........................................");

    /* Create the DHCP Server.  */
    status =  nx_dhcp_server_create(&dhcp_server, &server_ip, pointer, DEMO_STACK_SIZE, 
                                   "DHCP Server", &server_pool);
    
    pointer = pointer + DEMO_STACK_SIZE;
    
    /* Check for errors creating the DHCP Server. */
    if (status)
        error_counter++;

    /* Load the assignable DHCP IP addresses for the first interface.  */
    iface_index = 0;

    status = nx_dhcp_create_server_ip_address_list(&dhcp_server, iface_index, START_IP_ADDRESS_LIST_0, 
                                                   END_IP_ADDRESS_LIST_0, &addresses_added);

    /* Check for errors creating the list. */
    if (status)
        error_counter++;

    /* Verify all the addresses were added to the list. */
    if (addresses_added != 10)
        error_counter++;

    status = nx_dhcp_set_interface_network_parameters(&dhcp_server, iface_index, NX_DHCP_SUBNET_MASK_0, 
                                                      NX_DHCP_DEFAULT_GATEWAY_0, NX_DHCP_DNS_SERVER_0);

    /* Check for errors setting network parameters. */
    if (status)
        error_counter++;

    /* set the udp packet receive function to mine. */
    server_ip.nx_ip_udp_packet_receive = my_udp_packet_receive;

    /* Start DHCP Server task.  */
    status = nx_dhcp_server_start(&dhcp_server);

    /* Check for errors starting up the DHCP server.  */
    if (status)
        error_counter++;
    
    tx_thread_sleep(3 * NX_IP_PERIODIC_RATE);

    if((error_counter) || (discover_flag == 0) || (request_flag == 0))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {
        printf("SUCCESS!\n");
        test_control_return(0);
    }

    return;
}

/* Define the test threads.  */

void    client_thread_entry(ULONG thread_input)
{

UINT        status;

    discover_flag = 0;
    request_flag = 0;

    /* Create the DHCP instance.  */
    status =  nx_dhcp_create(&dhcp_client, &client_ip, "dhcp_client");
    if (status)
        error_counter++;

#ifdef NX_DHCP_CLIENT_USER_CREATE_PACKET_POOL
    status = nx_dhcp_packet_pool_set(&dhcp_client, &client_pool);
    if (status)
        error_counter++;
#endif /* NX_DHCP_CLIENT_USER_CREATE_PACKET_POOL  */


    /* Start the DHCP Client.  */
    status =  nx_dhcp_start(&dhcp_client);
    if (status)
        error_counter++;

    /* Wait for DHCP to assign the IP address.  */
    do
    {
        /* Check for address resolution.  */
        status =  nx_ip_status_check(&client_ip, NX_IP_ADDRESS_RESOLVED, (ULONG *) &status, NX_IP_PERIODIC_RATE);

        /* Check status.  */
        if (status)
        {
            /* wait a bit. */
            tx_thread_sleep(NX_IP_PERIODIC_RATE);
        }

    } while (status != NX_SUCCESS);

    /* Stopping the DHCP client. */
    nx_dhcp_stop(&dhcp_client);

    /* All done. Return resources to NetX and ThreadX. */    
    nx_dhcp_delete(&dhcp_client);

}


void    my_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr)
{

UINT    status;
ULONG   message_type;
ULONG   bcd_flag;

    /* Get the dhcp message type. */
    status = dhcp_get_option_value((packet_ptr -> nx_packet_prepend_ptr + 8), NX_DHCP_SERVER_OPTION_DHCP_TYPE, 
                                       &message_type, (packet_ptr -> nx_packet_length - 8));
    if(status)
        error_counter++;

    /* Check if the message is a DHCPOFFER. */
    if(message_type == NX_DHCP_TYPE_DHCPDISCOVER)
    {
        /* Get flags field value. */
        bcd_flag = dhcp_get_data((packet_ptr -> nx_packet_prepend_ptr + 8 + NX_BOOTP_OFFSET_FLAGS), 2);

        /* Check wether BROADCAST bit is set to 1. */
        if((bcd_flag& 0x8000) != 0x8000)
            error_counter++;
        else
            discover_flag = 1; 

    }
    /* Check if the message is a DHCPREQUEST. */
    else if((message_type == NX_DHCP_TYPE_DHCPREQUEST) && (discover_flag == 1))
    {
        /* Get flags field value. */
        bcd_flag = dhcp_get_data((packet_ptr -> nx_packet_prepend_ptr + 8 + NX_BOOTP_OFFSET_FLAGS), 2);

        /* Check wether BROADCAST bit is set to 1. */
        if((bcd_flag& 0x8000) != 0x8000)
            error_counter++;
        else
            request_flag = 1; 

        /* Restore the udp packet receiving function. */
        server_ip.nx_ip_udp_packet_receive = _nx_udp_packet_receive;

    }

    /* Let server receives the packet.  */
    _nx_udp_packet_receive(ip_ptr, packet_ptr); 
}
#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcp_04_01_01_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   NetX DHCP 04_01_01 Test...................................N/A\n"); 

    test_control_return(3);  
}      
#endif