/* This NetX test concentrates on overlapping TCP data packets.  */

#include   "tx_api.h"
#include   "nx_api.h"

extern void    test_control_return(UINT status);

#if !defined(NX_TCP_ACK_EVERY_N_PACKETS) && !defined(NX_DISABLE_IPV4)

#define     DEMO_STACK_SIZE         2048

#define MSG "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"



/* Define the ThreadX and NetX object control blocks...  */

static TX_THREAD               thread_0;
static TX_THREAD               thread_1;

static NX_PACKET_POOL          pool_0;
static NX_IP                   ip_0;
static NX_IP                   ip_1;
static NX_TCP_SOCKET           client_socket;
static NX_TCP_SOCKET           server_socket;



/* Define the counters used in the demo application...  */

static ULONG                   error_counter;


extern ULONG    packet_gather;

/* Define thread prototypes.  */

static void    thread_0_entry(ULONG thread_input);
static void    thread_1_entry(ULONG thread_input);
static void    thread_1_connect_received(NX_TCP_SOCKET *server_socket, UINT port);
static void    thread_1_disconnect_received(NX_TCP_SOCKET *server_socket);
extern void    test_control_return(UINT status);
extern void    _nx_ram_network_driver_256(struct NX_IP_DRIVER_STRUCT *driver_req);


/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_tcp_overlapping_packet_test_application_define(void *first_unused_memory)
#endif
{

    CHAR    *pointer;
    UINT    status;


    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    error_counter =  0;

    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;

    /* Create the main thread.  */
    tx_thread_create(&thread_1, "thread 1", thread_1_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;


    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create a packet pool.  */
    status =  nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", 512, pointer, 8192);
    pointer = pointer + 8192;

    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&ip_0, "NetX IP Instance 0", IP_ADDRESS(1, 2, 3, 4), 0xFFFFFF00UL, &pool_0, _nx_ram_network_driver_256,
                          pointer, 2048, 1);
    pointer =  pointer + 2048;

    /* Create another IP instance.  */
    status += nx_ip_create(&ip_1, "NetX IP Instance 1", IP_ADDRESS(1, 2, 3, 5), 0xFFFFFF00UL, &pool_0, _nx_ram_network_driver_256,
                           pointer, 2048, 1);
    pointer =  pointer + 2048;

    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for IP Instance 0.  */
    status =  nx_arp_enable(&ip_0, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Enable ARP and supply ARP cache memory for IP Instance 1.  */
    status +=  nx_arp_enable(&ip_1, (void *) pointer, 1024);
    pointer = pointer + 1024;

    /* Check ARP enable status.  */
    if (status)
        error_counter++;

    /* Enable TCP processing for both IP instances.  */
    status =  nx_tcp_enable(&ip_0);
    status += nx_tcp_enable(&ip_1);

    /* Check TCP enable status.  */
    if (status)
        error_counter++;
}



/* Define the test threads.  */

static void    thread_0_entry(ULONG thread_input)
{

UINT        status;
NX_PACKET   *my_packet1;
NX_PACKET   *my_packet2;
NX_PACKET   *my_packet3;
NX_PACKET   *my_packet4;
NX_PACKET   *my_packet23;
char        *msg = MSG;
ULONG        seq1, seq2;

    /* Print out test information banner.  */
    printf("NetX Test:   TCP Overlapping Packet Test 1.............................");

    /* Check for earlier error.  */
    if (error_counter)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Create a socket.  */
    status =  nx_tcp_socket_create(&ip_0, &client_socket, "Client Socket", 
                                   NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 300,
                                   NX_NULL, NX_NULL);
    
    /* Check for error.  */
    if (status)
        error_counter++;

    /* Bind the socket.  */
    status =  nx_tcp_client_socket_bind(&client_socket, 12, NX_WAIT_FOREVER);

    /* Check for error.  */
    if (status)
        error_counter++;

    /* Attempt to connect the socket.  */
    tx_thread_relinquish();

    status =  nx_tcp_client_socket_connect(&client_socket, IP_ADDRESS(1, 2, 3, 5), 12, 5 * NX_IP_PERIODIC_RATE);

    /* Check for error.  */
    if (status)
        error_counter++;


    /* Create 4 packets */
    status = nx_packet_allocate(&pool_0, &my_packet1, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += nx_packet_allocate(&pool_0, &my_packet2, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += nx_packet_allocate(&pool_0, &my_packet3, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += nx_packet_allocate(&pool_0, &my_packet4, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += nx_packet_allocate(&pool_0, &my_packet23, NX_TCP_PACKET, NX_WAIT_FOREVER);

    if (status)
        error_counter++;

    /* Fill in the packet with data.      */
    /* Packet 1 contains bytes   0 - 37 
       Packet 2 contains bytes  38 - 75
       Packet 3 contains bytes  76 - 113
       Packet 4 contains bytes  76 - 85
       packet 23 contains bytes 38 - 113  */
    
    memcpy(my_packet1 -> nx_packet_prepend_ptr, &msg[0], 38);
    my_packet1 -> nx_packet_length = 38;
    my_packet1 -> nx_packet_append_ptr = my_packet1 -> nx_packet_prepend_ptr + 38;

    memcpy(my_packet2 -> nx_packet_prepend_ptr, &msg[38], 38);
    my_packet2 -> nx_packet_length = 38;
    my_packet2 -> nx_packet_append_ptr = my_packet2 -> nx_packet_prepend_ptr + 38;

    memcpy(my_packet3 -> nx_packet_prepend_ptr, &msg[76], 38);
    my_packet3 -> nx_packet_length = 38;
    my_packet3 -> nx_packet_append_ptr = my_packet3 -> nx_packet_prepend_ptr + 38;

    memcpy(my_packet4 -> nx_packet_prepend_ptr, &msg[76], 10);
    my_packet4 -> nx_packet_length = 10;
    my_packet4 -> nx_packet_append_ptr = my_packet4 -> nx_packet_prepend_ptr + 10;

    memcpy(my_packet23 -> nx_packet_prepend_ptr, &msg[38], 76);
    my_packet23 -> nx_packet_length = 76;
    my_packet23 -> nx_packet_append_ptr = my_packet23 -> nx_packet_prepend_ptr + 76;

    /* Send the 1st one */
    status = nx_tcp_socket_send(&client_socket, my_packet1, NX_IP_PERIODIC_RATE);

    /* Update the seq number and send the 3rd one. */
    seq1 = client_socket.nx_tcp_socket_tx_sequence;
    client_socket.nx_tcp_socket_tx_sequence += 38;
    status += nx_tcp_socket_send(&client_socket, my_packet3, NX_IP_PERIODIC_RATE);
    seq2 = client_socket.nx_tcp_socket_tx_sequence;

    /* Update the seq number and send the 4th one. */
    client_socket.nx_tcp_socket_tx_sequence = seq1 + 38;
    status += nx_tcp_socket_send(&client_socket, my_packet4, NX_IP_PERIODIC_RATE);

    /* Restore the Seq number for packet 2. */
    client_socket.nx_tcp_socket_tx_sequence = seq1;
    status += nx_tcp_socket_send(&client_socket, my_packet2, NX_IP_PERIODIC_RATE);

    /* Restore the seq number for packet 23 */
    client_socket.nx_tcp_socket_tx_sequence = seq1;

#ifdef __PRODUCT_NETXDUO__
    /* Also reduce the outstanding byte count. */
    client_socket.nx_tcp_socket_tx_outstanding_bytes -= my_packet23 -> nx_packet_length;
#endif
    status += nx_tcp_socket_send(&client_socket, my_packet23, NX_IP_PERIODIC_RATE);

    if (status)
    {
        error_counter++;
    }

    if(client_socket.nx_tcp_socket_tx_sequence != seq2)
    {
        error_counter++;
    }
}

static char rcv_buffer[200];
static void    thread_1_entry(ULONG thread_input)
{

    UINT            status;
    NX_PACKET       *packet_ptr;
    ULONG           actual_status;
    ULONG           recv_length = 0;

    /* Ensure the IP instance has been initialized.  */
    status =  nx_ip_status_check(&ip_1, NX_IP_INITIALIZE_DONE, &actual_status, NX_IP_PERIODIC_RATE);

    /* Check status...  */
    if (status != NX_SUCCESS)
    {

        error_counter++;
        test_control_return(1);
    }

    /* Create a socket.  */
    status =  nx_tcp_socket_create(&ip_1, &server_socket, "Server Socket", 
                                   NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 200,
                                   NX_NULL, thread_1_disconnect_received);
                                
    /* Check for error.  */
    if (status)
        error_counter++;

    /* Setup this thread to listen.  */
    status =  nx_tcp_server_socket_listen(&ip_1, 12, &server_socket, 5, thread_1_connect_received);

    /* Check for error.  */
    if (status)
        error_counter++;

    /* Accept a client socket connection.  */
    status =  nx_tcp_server_socket_accept(&server_socket, 5 * NX_IP_PERIODIC_RATE);

    /* Check for error.  */
    if (status)
        error_counter++;

    /* Receive a TCP message from the socket.  */
    while (nx_tcp_socket_receive(&server_socket, &packet_ptr, 5 * NX_IP_PERIODIC_RATE) == NX_SUCCESS)
    {

        if(packet_ptr -> nx_packet_length == 0)
            error_counter++;

        memcpy(&rcv_buffer[recv_length], packet_ptr -> nx_packet_prepend_ptr, packet_ptr -> nx_packet_length);
        recv_length += packet_ptr -> nx_packet_length;

        /* Release the packet.  */
        nx_packet_release(packet_ptr);
    }

    if(recv_length != 114)
        error_counter++;

    if(memcmp(rcv_buffer, (void*)MSG, recv_length))
        error_counter++;

#ifndef NX_DISABLE_PACKET_INFO
    /* Check packet pool state.  */
    if(pool_0.nx_packet_pool_invalid_releases)
        error_counter++;
#endif

    /* Determine if the test was successful.  */
    if (error_counter)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }
    else
    {

        printf("SUCCESS!\n");
        test_control_return(0);
    }
}


static void  thread_1_connect_received(NX_TCP_SOCKET *socket_ptr, UINT port)
{

    /* Check for the proper socket and port.  */
    if ((socket_ptr != &server_socket) || (port != 12))
        error_counter++;
}


static void  thread_1_disconnect_received(NX_TCP_SOCKET *socket)
{

    /* Check for proper disconnected socket.  */
    if (socket != &server_socket)
        error_counter++;
}

#else
#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void          netx_tcp_overlapping_packet_test_application_define(void *first_unused_memory)
#endif
{
    printf("NetX Test:   TCP Overlapping Packet Test 1.............................N/A\n");
    test_control_return(3);
}
#endif
