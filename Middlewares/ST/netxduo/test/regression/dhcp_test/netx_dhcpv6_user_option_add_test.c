/* This tests adding user class option.  */

#include    <stdio.h>
#include   "tx_api.h"
#include   "nx_api.h"
extern void test_control_return(UINT status);

#if defined(FEATURE_NX_IPV6) && !defined(NX_DISABLE_FRAGMENTATION)
#include   "nxd_dhcpv6_client.h"
#include   "nxd_dhcpv6_server.h"

#define     DEMO_STACK_SIZE                     2048
#define     NX_DHCPV6_THREAD_STACK_SIZE         2048
#define     NX_PACKET_SIZE                      1536
#define     NX_PACKET_POOL_SIZE                 (NX_PACKET_SIZE * 8)

/* Define the ThreadX and NetX object control blocks...  */

static NX_PACKET_POOL          pool_0;
static TX_THREAD               thread_client;
static TX_THREAD               thread_server;
static NX_IP                   client_ip;
static NX_IP                   server_ip;

/* Define the Client and Server instances. */

static NX_DHCPV6               dhcp_client;
static NX_DHCPV6_SERVER        dhcp_server;

/* Define the error counter used in the demo application...  */
static ULONG                   error_counter;
static CHAR                    *pointer;

static NXD_ADDRESS             server_address; 
static NXD_ADDRESS             dns_ipv6_address;
static NXD_ADDRESS             start_ipv6_address;
static NXD_ADDRESS             end_ipv6_address;


/* Define thread prototypes.  */

static void    thread_client_entry(ULONG thread_input);
static void    thread_server_entry(ULONG thread_input);
extern void    _nx_ram_network_driver(NX_IP_DRIVER *driver_req_ptr);
extern VOID    _nx_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr);

/* Define some DHCPv6 parameters.  */

#define DHCPV6_IANA_ID                  0xC0DEDBAD 
#define DHCPV6_T1                       NX_DHCPV6_INFINITE_LEASE 
#define DHCPV6_T2                       NX_DHCPV6_INFINITE_LEASE 
#define NX_DHCPV6_REFERRED_LIFETIME     NX_DHCPV6_INFINITE_LEASE
#define NX_DHCPV6_VALID_LIFETIME        NX_DHCPV6_INFINITE_LEASE


/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_dhcpv6_user_option_add_test_application_define(void *first_unused_memory)
#endif
{

UINT    status;

    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create the Client thread.  */
    status = tx_thread_create(&thread_client, "Client thread", thread_client_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;

    /* Check for IP create errors.  */
    if (status)
    {
        error_counter++;
    }

    /* Create the Server thread.  */
    status = tx_thread_create(&thread_server, "Server thread", thread_server_entry, 0,  
                              pointer, DEMO_STACK_SIZE, 
                              3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Check for IP create errors.  */
    if (status)
    {
        error_counter++;
    }

    /* Create a packet pool.  */
    status =  nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", 1024, pointer, NX_PACKET_POOL_SIZE);
    pointer = pointer + NX_PACKET_POOL_SIZE;

    /* Check for pool creation error.  */
    if (status)
        error_counter++;

    /* Create a Client IP instance.  */
    status = nx_ip_create(&client_ip, "Client IP", IP_ADDRESS(0, 0, 0, 0), 
                          0xFFFFFF00UL, &pool_0, _nx_ram_network_driver,
                          pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
    {
        error_counter++;
    }

    /* Create a Server IP instance.  */
    status = nx_ip_create(&server_ip, "Server IP", IP_ADDRESS(1, 2, 3, 4), 
                          0xFFFFFF00UL, &pool_0, _nx_ram_network_driver,
                          pointer, 2048, 1);

    pointer =  pointer + 2048;

    /* Check for IP create errors.  */
    if (status)
    {
        error_counter++;
    }

    /* Enable UDP traffic for sending DHCPv6 messages.  */
    status =  nx_udp_enable(&client_ip);
    status +=  nx_udp_enable(&server_ip);

    /* Check for UDP enable errors.  */
    if (status)
    {
        error_counter++;
    }
    
    /* Enable the IPv6 services. */
    status = nxd_ipv6_enable(&client_ip);
    status += nxd_ipv6_enable(&server_ip);
    
    /* Check for IPv6 enable errors.  */
    if (status)
    {
        error_counter++;
    }
    
    /* Enable the ICMPv6 services. */
    status = nxd_icmp_enable(&client_ip);
    status += nxd_icmp_enable(&server_ip);
    
    /* Check for ICMP enable errors.  */
    if (status)
    {
        error_counter++;
    }

    /* Enable the fragment feature.  */
    status = nx_ip_fragment_enable(&client_ip);
    status += nx_ip_fragment_enable(&server_ip);
    
    /* Check for ICMP enable errors.  */
    if (status)
    {
        error_counter++;
    }
}


static CHAR user_class[] = "USERCLASS";
static UINT user_class_len = sizeof(user_class) - 1;
static USHORT user_class_code = 15;

UINT dhcpv6_user_option_add(NX_DHCPV6 *dhcpv6_ptr, UINT interface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length)
{

    NX_PARAMETER_NOT_USED(dhcpv6_ptr);
    NX_PARAMETER_NOT_USED(interface_index);
    NX_PARAMETER_NOT_USED(message_type);

    /* Application can check if add options into the packet by interface_index and message_type.
       message_type are defined in header file, such as: NX_DHCPV6_MESSAGE_TYPE_SOLICIT.  */

    /* Add user class option refer to RFC8415.  */
    /* The format of the User Class option is:
       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |       OPTION_USER_CLASS       |          option-len           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      .                                                               .
      .                          user-class-data                      .
      .                                                               .
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      The data area of the User Class option MUST contain one or more
      instances of user-class-data information.  Each instance of
      user-class-data is formatted as follows:
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+-+-+-+-+
      |        user-class-len         |          opaque-data          |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-...-+-+-+-+-+-+-+

      The user-class-len field is 2 octets long and specifies the length of
      the opaque user-class-data in network byte order.
    */

    /* Check if have enough space for this option.  */
    if (*user_option_length < (user_class_len + 6))
        return(NX_FALSE);

    /* Set the option code.  */
    *user_option_ptr = (UCHAR)(user_class_code >> 8);
    *(user_option_ptr + 1) = (UCHAR)user_class_code;

    /* Set the option length.  */
    *(user_option_ptr + 2) = (UCHAR)((user_class_len + 2) >> 8);
    *(user_option_ptr + 3) = (UCHAR)(user_class_len + 2);
    
    /* Set the user class length, which is the total length of the user class data filed.  */
    *(user_option_ptr + 4) = (UCHAR)((user_class_len + 2) >> 8);
    *(user_option_ptr + 5) = (UCHAR)(user_class_len + 2);

    /* Set the option value.  */
    memcpy((user_option_ptr + 6), user_class, user_class_len);

    /* Update the option length.  */
    *user_option_length = (user_class_len + 6);
    return(NX_TRUE);
}

VOID    my_udp_packet_receive(NX_IP *ip_ptr, NX_PACKET *packet_ptr)
{
UCHAR *user_class_ptr = NX_NULL;
UCHAR *dhcpv6_option_ptr = NX_NULL;
UINT index, status;
ULONG option_length = 0, option_code = 0;

    /* Update index to the options. */
    index = 8 + 4;
    dhcpv6_option_ptr = packet_ptr -> nx_packet_prepend_ptr + index;

    /* Search the user class option.  */
    while (index + 4 <= packet_ptr -> nx_packet_length)
    {

        /* Get the option code and length of data of the current option block. */
        status = _nx_dhcpv6_utility_get_block_option_length(dhcpv6_option_ptr, &option_code, &option_length);

        /* Check that the block data is valid. */
        if (status != NX_SUCCESS)
        {
            error_counter++;
            break;
        }

        /* Check if option data is in the packet.  */
        if ((dhcpv6_option_ptr + 4 + option_length) > packet_ptr -> nx_packet_append_ptr)
        {
            error_counter++;
            break;
        }

        if (option_code == user_class_code)
        {
            user_class_ptr = dhcpv6_option_ptr;
            break;
        }

        /* Move to the next top level option. */
        dhcpv6_option_ptr += option_length + 4;

        /* Keep track of how far into the packet we have parsed. */
        index += option_length + 4; 
    }

    /* Check if have the  user class option.  */
    if (user_class_ptr == NX_NULL || option_length != (user_class_len + 2))
    {
        error_counter++;
    }
    else
    {
        /* Skip the option header.  */
        user_class_ptr += 4;

        /* Check the user class length.  */
        status = _nx_dhcpv6_utility_get_data(user_class_ptr, 2, &option_length);
        if (status || (option_length != (user_class_len + 2)))
            error_counter++;

        /* Skip the user class length filed.  */
        user_class_ptr += 2;

        /* Check the user class option.  */
        if(memcmp(user_class_ptr, user_class, user_class_len))
            error_counter++;
    }

    /* Call the actual function to receive the UDP packet.  */
    _nx_udp_packet_receive(ip_ptr, packet_ptr);
}

/* Define the Client host application thread. */

void    thread_client_entry(ULONG thread_input)
{

UINT        status;
NXD_ADDRESS ipv6_address;
NXD_ADDRESS dns_address;
ULONG       prefix_length; 
UINT        interface_index;
ULONG       T1, T2, preferred_lifetime, valid_lifetime;
UINT        address_count;
UINT        address_index;
NX_PACKET   *my_packet;


    /* Print out test information banner.  */
    printf("NetX Test:   DHCPv6 User Option Add Test...............................");
                                                                              
    /* Check for earlier error.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Establish the link local address for the host. The RAM driver creates
       a virtual MAC address of 0x1122334456. */
    status = nxd_ipv6_address_set(&client_ip, 0, NX_NULL, 10, NULL);

    /* Let NetX Duo and the network driver get initialized. Also give the server time to get set up. */
    tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);        
    
    /* Create the DHCPv6 Client. */
    status =  nx_dhcpv6_client_create(&dhcp_client, &client_ip, "DHCPv6 Client", &pool_0, pointer, NX_DHCPV6_THREAD_STACK_SIZE,
                                      NX_NULL, NX_NULL);
    pointer = pointer + NX_DHCPV6_THREAD_STACK_SIZE;

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Create a Link Layer Plus Time DUID for the DHCPv6 Client. Set time ID field 
       to NULL; the DHCPv6 Client API will supply one. */
    status = nx_dhcpv6_create_client_duid(&dhcp_client, NX_DHCPV6_DUID_TYPE_LINK_TIME, 
                                          NX_DHCPV6_HW_TYPE_IEEE_802, 0);

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Create the DHCPv6 client's Identity Association (IA-NA) now. 

       Note that if this host had already been assigned in IPv6 lease, it
       would have to use the assigned T1 and T2 values in loading the DHCPv6
       client with an IANA block. 
    */
    status = nx_dhcpv6_create_client_iana(&dhcp_client, DHCPV6_IANA_ID, DHCPV6_T1,  DHCPV6_T2); 

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Set callback function to add user options.  */
    status =  nx_dhcpv6_user_option_add_callback_set(&dhcp_client, dhcpv6_user_option_add);
    if (status)
        error_counter++;

    /* Starting up the NetX DHCPv6 Client. */
    status =  nx_dhcpv6_start(&dhcp_client);

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* If the host also want to get the option message, set the list of desired options to enabled. */
    nx_dhcpv6_request_option_timezone(&dhcp_client, NX_TRUE); 
    nx_dhcpv6_request_option_DNS_server(&dhcp_client, NX_TRUE);
    nx_dhcpv6_request_option_time_server(&dhcp_client, NX_TRUE);
    nx_dhcpv6_request_option_domain_name(&dhcp_client, NX_TRUE);

    /* Now, the host send the solicit message to get the IPv6 address and other options from the DHCPv6 server. */
    status = nx_dhcpv6_request_solicit(&dhcp_client);

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Waiting for get the IPv6 address and do the duplicate address detection. */
    tx_thread_sleep(6 * NX_IP_PERIODIC_RATE);

    /* Get the valid IPv6 address count which the DHCPv6 server assigned .  */
    status = nx_dhcpv6_get_valid_ip_address_count(&dhcp_client, &address_count);

    /* Check for errors.  */
    if ((status) || (address_count != 1))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    
    /* Get the IPv6 address, preferred lifetime and valid lifetime according to the address index. */
    address_index = 0;
    status = nx_dhcpv6_get_valid_ip_address_lease_time(&dhcp_client, address_index, &ipv6_address, &preferred_lifetime, &valid_lifetime);

    /* Check for errors.  */
    if ((status) ||
        (preferred_lifetime != NX_DHCPV6_DEFAULT_PREFERRED_TIME) ||
        (valid_lifetime != NX_DHCPV6_DEFAULT_VALID_TIME))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Get the IPv6 address by DHCPv6 API. */
    status = nx_dhcpv6_get_IP_address(&dhcp_client, &ipv6_address);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Check the address.  */
    if ((ipv6_address.nxd_ip_version != start_ipv6_address.nxd_ip_version) ||
        (ipv6_address.nxd_ip_address.v6[0] != start_ipv6_address.nxd_ip_address.v6[0]) ||
        (ipv6_address.nxd_ip_address.v6[1] != start_ipv6_address.nxd_ip_address.v6[1]) ||
        (ipv6_address.nxd_ip_address.v6[2] != start_ipv6_address.nxd_ip_address.v6[2]) ||
        (ipv6_address.nxd_ip_address.v6[3] != start_ipv6_address.nxd_ip_address.v6[3]))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
     
    /* Get the IPv6 address by NetX Duo API. */
    status = nxd_ipv6_address_get(&client_ip, 1, &ipv6_address, &prefix_length, &interface_index);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Check the IPv6 address.  */
    if ((prefix_length != 64) || (interface_index != 0) ||
        (ipv6_address.nxd_ip_version != start_ipv6_address.nxd_ip_version) ||
        (ipv6_address.nxd_ip_address.v6[0] != start_ipv6_address.nxd_ip_address.v6[0]) ||
        (ipv6_address.nxd_ip_address.v6[1] != start_ipv6_address.nxd_ip_address.v6[1]) ||
        (ipv6_address.nxd_ip_address.v6[2] != start_ipv6_address.nxd_ip_address.v6[2]) ||
        (ipv6_address.nxd_ip_address.v6[3] != start_ipv6_address.nxd_ip_address.v6[3]))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Get IP address lease time. 
       Note, This API only applies to one IA. */
    status = nx_dhcpv6_get_lease_time_data(&dhcp_client, &T1, &T2, &preferred_lifetime, &valid_lifetime);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {                   
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Check the value.  */
    if ((T1 != NX_DHCPV6_DEFAULT_T1_TIME) ||
        (T2 != NX_DHCPV6_DEFAULT_T2_TIME) ||
        (preferred_lifetime != NX_DHCPV6_DEFAULT_PREFERRED_TIME) ||
        (valid_lifetime != NX_DHCPV6_DEFAULT_VALID_TIME))
    {    
        printf("ERROR!\n");
        test_control_return(1);
    }
    
    /* Get the DNS Server address. */
    status = nx_dhcpv6_get_DNS_server_address(&dhcp_client, 0, &dns_address);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
    
    /* Check the IPv6 DNS address.  */
    if ((dns_address.nxd_ip_version != dns_ipv6_address.nxd_ip_version) ||
        (dns_address.nxd_ip_address.v6[0] != dns_ipv6_address.nxd_ip_address.v6[0]) ||
        (dns_address.nxd_ip_address.v6[1] != dns_ipv6_address.nxd_ip_address.v6[1]) ||
        (dns_address.nxd_ip_address.v6[2] != dns_ipv6_address.nxd_ip_address.v6[2]) ||
        (dns_address.nxd_ip_address.v6[3] != dns_ipv6_address.nxd_ip_address.v6[3]))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Ping the DHCPv6 Server.  */
    status = nxd_icmp_ping(&client_ip, &server_address, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 28, &my_packet, NX_IP_PERIODIC_RATE);

    /* Check for errors.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Release the address.  */
    status = nx_dhcpv6_request_release(&dhcp_client);

    /* Check status.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
     
    /* Waiting for release the IPv6 address. */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* Get the IPv6 address by DHCPv6 API. */
    status = nx_dhcpv6_get_IP_address(&dhcp_client, &ipv6_address);

    /* Check status.  */
    if (status == NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
     
    /* Get the IPv6 address by NetX Duo API. */
    status = nxd_ipv6_address_get(&client_ip, 1, &ipv6_address, &prefix_length, &interface_index);

    /* Check status.  */
    if (status == NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Stop the Client task. */
    status = nx_dhcpv6_stop(&dhcp_client);

    /* Check status.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Delete the DHCPv6 client. */
    status = nx_dhcpv6_client_delete(&dhcp_client);

    /* Check status.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Output successfully.  */
    printf("SUCCESS!\n");
    test_control_return(0);
}

/* Define the test server thread. */
void    thread_server_entry(ULONG thread_input)
{

UINT        status;
ULONG       duid_time;
UINT        addresses_added;

    /* Set the IPv6 address of DHCPv6 Server.  */
    server_address.nxd_ip_version = NX_IP_VERSION_V6 ;
    server_address.nxd_ip_address.v6[0] = 0x20010db8;
    server_address.nxd_ip_address.v6[1] = 0xf101;
    server_address.nxd_ip_address.v6[2] = 0x00000000;
    server_address.nxd_ip_address.v6[3] = 0x00000101;    

    /* Set the link local and global addresses. */
    status = nxd_ipv6_address_set(&server_ip, 0, NX_NULL, 10, NULL);
    status += nxd_ipv6_address_set(&server_ip, 0, &server_address, 64, NULL);

    /* Check for errors. */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Validate the link local and global addresses. */
    tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);

    /* Create the DHCPv6 Server. */
    status =  nx_dhcpv6_server_create(&dhcp_server, &server_ip, "DHCPv6 Server", &pool_0, pointer, NX_DHCPV6_SERVER_THREAD_STACK_SIZE, NX_NULL, NX_NULL);
    pointer += NX_DHCPV6_SERVER_THREAD_STACK_SIZE;

    /* Check for errors.  */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Set up the DNS IPv6 server address. */
    dns_ipv6_address.nxd_ip_version = NX_IP_VERSION_V6 ;
    dns_ipv6_address.nxd_ip_address.v6[0] = 0x20010db8;
    dns_ipv6_address.nxd_ip_address.v6[1] = 0x0000f101;
    dns_ipv6_address.nxd_ip_address.v6[2] = 0x00000000;
    dns_ipv6_address.nxd_ip_address.v6[3] = 0x00000107;

    status = nx_dhcpv6_create_dns_address(&dhcp_server, &dns_ipv6_address);

    /* Check for errors. */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

     /* Note: For DUID types that do not require time, the 'duid_time' input can be left at zero. 
        The DUID_TYPE and HW_TYPE are configurable options that are user defined in nx_dhcpv6_server.h.  */

    /* Set the DUID time as the start of the millenium. */
    duid_time = SECONDS_SINCE_JAN_1_2000_MOD_32;
    status = nx_dhcpv6_set_server_duid(&dhcp_server,
                                    NX_DHCPV6_SERVER_DUID_TYPE, NX_DHCPV6_SERVER_HW_TYPE,
                                    dhcp_server.nx_dhcpv6_ip_ptr -> nx_ip_arp_physical_address_msw,
                                    dhcp_server.nx_dhcpv6_ip_ptr -> nx_ip_arp_physical_address_lsw,
                                    duid_time);

    /* Check for errors. */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Set the IPv6 start address.  */
    start_ipv6_address.nxd_ip_version = NX_IP_VERSION_V6 ;
    start_ipv6_address.nxd_ip_address.v6[0] = 0x20010db8;
    start_ipv6_address.nxd_ip_address.v6[1] = 0x00000f101;
    start_ipv6_address.nxd_ip_address.v6[2] = 0x0;
    start_ipv6_address.nxd_ip_address.v6[3] = 0x00000110;    

    /* Set the IPv6 end address.  */
    end_ipv6_address.nxd_ip_version = NX_IP_VERSION_V6 ;
    end_ipv6_address.nxd_ip_address.v6[0] = 0x20010db8;
    end_ipv6_address.nxd_ip_address.v6[1] = 0x0000f101;
    end_ipv6_address.nxd_ip_address.v6[2] = 0x00000000;
    end_ipv6_address.nxd_ip_address.v6[3] = 0x00000120;  

    /* Set the IPv6 address range.  */
    status = nx_dhcpv6_create_ip_address_range(&dhcp_server, &start_ipv6_address, &end_ipv6_address, &addresses_added);

    /* Check for errors. */
    if ((status) || (addresses_added != 16))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Change the pointer function to check user options.  */
    server_ip.nx_ip_udp_packet_receive = my_udp_packet_receive;

    /* Start the NetX DHCPv6 server!  */
    status =  nx_dhcpv6_server_start(&dhcp_server);

    /* Check for errors. */
    if(status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
} 
#else
#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void netx_dhcpv6_user_option_add_test_application_define(void * first_unused_memory)
#endif
{
    printf("NetX Test:   DHCPv6 User Option Add Test...............................N/A\n");
    test_control_return(3);
}     
#endif /* FEATURE_NX_IPV6 */
