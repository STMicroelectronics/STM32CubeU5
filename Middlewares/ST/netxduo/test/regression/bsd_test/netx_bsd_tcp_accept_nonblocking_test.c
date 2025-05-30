/* This NetX test concentrates on the basic BSD non-blocking operation.  */
#include   "tx_api.h"
#include   "nx_api.h"
#if defined(NX_BSD_ENABLE) && !defined(NX_DISABLE_IPV4)
#include   "nxd_bsd.h"
#define     DEMO_STACK_SIZE         4096


/* Define the ThreadX and NetX object control blocks...  */

static TX_THREAD               ntest_0;
static TX_THREAD               ntest_1;

static NX_PACKET_POOL          pool_0;
static NX_IP                   ip_0;
static NX_IP                   ip_1;
static ULONG                   bsd_thread_area[DEMO_STACK_SIZE / sizeof(ULONG)];
static TX_SEMAPHORE            sema_0;
static TX_SEMAPHORE            sema_1;
static TX_SEMAPHORE            sema_2;
#define BSD_THREAD_PRIORITY    2
#define NUM_CLIENTS            NX_BSD_MAX_SOCKETS

/* Define the counters used in the test application...  */

static ULONG                   error_counter;
static ULONG                   packet_pool_area[(256 + sizeof(NX_PACKET)) * (NUM_CLIENTS + 4) * 8 / 4];


/* Define thread prototypes.  */

static void    ntest_0_entry(ULONG thread_input);
static void    ntest_1_entry(ULONG thread_input);
extern void    test_control_return(UINT status);
extern void    _nx_ram_network_driver_256(struct NX_IP_DRIVER_STRUCT *driver_req);
static char *requests[4] = {"Request1", "Request2", "Request3", "Request4"};
static char *response[4] = {"Response1", "Response2", "Response3", "Response4"};
static void validate_bsd_structure(void);
/* Define what the initial system looks like.  */
#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_bsd_tcp_accept_nonblocking_test_application_define(void *first_unused_memory)
#endif
{

CHAR    *pointer;
UINT    status;

    
    /* Setup the working pointer.  */
    pointer =  (CHAR *) first_unused_memory;

    error_counter =  0;

    /* Create the main thread.  */
    tx_thread_create(&ntest_0, "thread 0", ntest_0_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;

    /* Create the main thread.  */
    tx_thread_create(&ntest_1, "thread 1", ntest_1_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer =  pointer + DEMO_STACK_SIZE;


    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create a packet pool.  */
    status =  nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", 256, packet_pool_area, sizeof(packet_pool_area));

    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&ip_0, "NetX IP Instance 0", IP_ADDRESS(1, 2, 3, 4), 0xFFFFFF00UL, &pool_0, _nx_ram_network_driver_256,
                    pointer, 4096, 1);
    pointer =  pointer + 4096;

    /* Create another IP instance.  */
    status += nx_ip_create(&ip_1, "NetX IP Instance 1", IP_ADDRESS(1, 2, 3, 5), 0xFFFFFF00UL, &pool_0, _nx_ram_network_driver_256,
                    pointer, 4096, 2);
    pointer =  pointer + 4096;
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for IP Instance 0.  */
    status =  nx_arp_enable(&ip_0, (void *) pointer, 1024);
    pointer = pointer + 1024;
    if (status)
        error_counter++;

    /* Enable ARP and supply ARP cache memory for IP Instance 1.  */
    status  =  nx_arp_enable(&ip_1, (void *) pointer, 1024);
    pointer = pointer + 1024;
    if (status)
        error_counter++;

    /* Enable TCP processing for both IP instances.  */
    status =  nx_tcp_enable(&ip_0);
    status += nx_tcp_enable(&ip_1);

    /* Enable BSD */
    status += bsd_initialize(&ip_0, &pool_0, (CHAR*)&bsd_thread_area[0], sizeof(bsd_thread_area), BSD_THREAD_PRIORITY);

    /* Check TCP enable status.  */
    if (status)
        error_counter++;

    status = tx_semaphore_create(&sema_0, "SEMA 0", 0);
    status += tx_semaphore_create(&sema_1, "SEMA 1", 0);
    status += tx_semaphore_create(&sema_2, "SEMA 2", 0);
    if(status != TX_SUCCESS)
        error_counter++;
}


typedef struct client_info_struct
{
    int sockfd;
    int message_id;
} client_info;

static client_info client_data[NUM_CLIENTS];
static ULONG stack_space[NUM_CLIENTS][DEMO_STACK_SIZE / sizeof(ULONG)];
static TX_THREAD helper_thread[NUM_CLIENTS];

static VOID bsd_server_helper_thread_entry(ULONG thread_input)
{
int ret;
int sockfd, message_id;
fd_set   read_fd;
struct timeval tv;
char  buf[30];
    /* Note the socket is already in non-blocking mode. */
    sockfd = client_data[thread_input].sockfd;
    message_id = client_data[thread_input].message_id;
    /* Receive data from the client. */
    
    FD_ZERO(&read_fd);
    FD_SET(sockfd, &read_fd);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    ret = select(sockfd + 1, &read_fd, NULL, NULL, &tv);
    if(ret < 0)
    {
        error_counter++;
        return;
    }
    if(ret == 1)
    {
        /* Check for read_fd */
        if(FD_ISSET(sockfd, &read_fd))
        {
            ret = recv(sockfd, buf, sizeof(buf), 0);

            /* Validate the data. */
            if((ret != (int)strlen(requests[message_id & 3])) || strncmp(buf, requests[message_id & 3], ret))
                error_counter++;

            /* Send a response back. */
            ret = send(sockfd, response[message_id & 3], strlen(response[message_id & 3]), 0);
            if(ret != (int)strlen(response[message_id & 3]))
                error_counter++;

            /* Wait until client received. */
            if((sockfd - NX_BSD_SOCKFD_START + 1) != (NUM_CLIENTS / 2))
                tx_semaphore_get(&sema_1, 5 * NX_IP_PERIODIC_RATE);
            else 
                tx_semaphore_get(&sema_2, 5 * NX_IP_PERIODIC_RATE);

            ret = soc_close(sockfd);
            if(ret < 0)
                error_counter++;
               
        }
    }
    else error_counter++;   

    tx_semaphore_put(&sema_0);
    return;
}




static void test_tcp_server4(void)
{
int                sockfd;
struct sockaddr_in remote_addr, local_addr;
int                address_length;
int                ret;
int                newsock;
int                i;
fd_set             read_fd;
struct timeval     tv;
int                accept_no_memory = 0;
int                error, len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error_counter++;

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(12345);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    /* Set the socket to non-blocking mode. */
    if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
        error_counter++;            
    ret = bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if(ret < 0)
        error_counter++;
    
    ret = listen(sockfd, 5);
    if(ret < 0)
        error_counter++;

    /* 3 iterations. */
    for(i = 0; i < NUM_CLIENTS; i++)
    {
        address_length = sizeof(remote_addr);

        tv.tv_sec = 2;
        tv.tv_usec = 0;
        FD_ZERO(&read_fd);
        FD_SET(sockfd, &read_fd);

        ret = select(sockfd + 1, &read_fd, NULL, NULL, &tv);

        if(ret != 1)
            error_counter++;
        else if(!FD_ISSET(sockfd, &read_fd))
        {
            error_counter++;
        }
        
        /* Check pending error. */
        len = sizeof(error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        if(error)
            error_counter++;

        if(error_counter == 0)
        {
            newsock = accept(sockfd, (struct sockaddr*)&remote_addr, &address_length);
            if(newsock > 0)
            {
                if(address_length != sizeof(remote_addr))
                    error_counter++;
                else if((remote_addr.sin_family != AF_INET) || (remote_addr.sin_addr.s_addr != htonl(0x01020305)))
                    error_counter++;
                
                if(error_counter == 0)
                {

                    /* Set the new socket to non-blocking mode. */
                    if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
                        error_counter++;            
                    else
                    {
                        /* Set the client data */
                        client_data[i].sockfd = newsock;
                        client_data[i].message_id = i;
                        
                        /* Create a helper thread to handle the new socket. */
                        tx_thread_create(&helper_thread[i], "helper thread", bsd_server_helper_thread_entry, 
                                         i, stack_space[i], DEMO_STACK_SIZE, 2,2, TX_NO_TIME_SLICE, TX_AUTO_START);
                        continue;
                    }
                }
            }
            else 
            {
                if(i != (NUM_CLIENTS - 1))
                    error_counter++;
                else
                {
                    if(errno != ENOMEM)
                        error_counter++;
                    else
                    {
                        accept_no_memory = 1;
                        tx_thread_sleep(10);
                        i--;
                        continue;
                    }
                }
            }
        }
    }


    /* Close downt he socket. */
    ret = soc_close(sockfd);
    if(ret < 0)
        error_counter++;

    if(accept_no_memory != 1)
        error_counter++;

    for(i = 0; i < NUM_CLIENTS; i++)
    {

        /* Wakeup server thread. */
        tx_semaphore_get(&sema_0, 5 * NX_IP_PERIODIC_RATE);
    }
}

static void    ntest_0_entry(ULONG thread_input)
{

    printf("NetX Test:   Basic BSD TCP Accept Nonblocking Test.........");

    /* Check for earlier error.  */
    if (error_counter)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Wakeup client. */
    tx_semaphore_put(&sema_1);

    test_tcp_server4();

    /* Wait until client finish. */
    tx_semaphore_get(&sema_0, 5 * NX_IP_PERIODIC_RATE);

    validate_bsd_structure();

    if(error_counter)
        printf("ERROR!\n");
    else
        printf("SUCCESS!\n");

    if(error_counter)
        test_control_return(1);    

   
    test_control_return(0);

}
    
static NX_TCP_SOCKET tcp_sockets[NUM_CLIENTS];
static void    multiple_client4(void)
{

int           i;
UINT          status = NX_SUCCESS;
NX_PACKET     *packet_ptr;
    for(i = 0; i < NUM_CLIENTS; i++)
    {
        status +=  nx_tcp_socket_create(&ip_1, &tcp_sockets[i], "Server Socket", 
                                        NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 100,
                                        NX_NULL, NX_NULL);
        status +=  nx_tcp_client_socket_bind(&tcp_sockets[i], NX_ANY_PORT, 0);
    }
    if(status != NX_SUCCESS)
        error_counter++;

    status = NX_SUCCESS;
    for(i = 0; i < (NUM_CLIENTS - 1); i++)
    {
        status += nx_tcp_client_socket_connect(&tcp_sockets[i], IP_ADDRESS(1, 2, 3, 4), 12345, NX_IP_PERIODIC_RATE);
    }
    if(status != NX_SUCCESS)
        error_counter++;

    status = NX_SUCCESS;

    /* Send messages to each server */
    for(i = 0; i < (NUM_CLIENTS - 1); i++)
    {
        status += nx_packet_allocate(&pool_0, &packet_ptr, NX_TCP_PACKET, NX_NO_WAIT);
        status += nx_packet_data_append(packet_ptr, requests[i&3], strlen(requests[i&3]),
                                        &pool_0, NX_NO_WAIT);
        status += nx_tcp_socket_send(&tcp_sockets[i], packet_ptr, NX_IP_PERIODIC_RATE);
        
    }

    if(status != NX_SUCCESS)
        error_counter++;

    status = NX_SUCCESS;
    /* Receive 3 messages. */

    for(i = 0; i < (NUM_CLIENTS - 1); i++)
    {
        status = nx_tcp_socket_receive(&tcp_sockets[i], &packet_ptr, 2 * NX_IP_PERIODIC_RATE);
        if(status != NX_SUCCESS)
        {
            error_counter++;
            continue;
        }

        /* Validate the received data. */
        else if(packet_ptr -> nx_packet_length != strlen(response[i&3]))
            error_counter++;
        else if(strncmp((char*)packet_ptr -> nx_packet_prepend_ptr, response[i&3], packet_ptr -> nx_packet_length))
            error_counter++;
        nx_packet_release(packet_ptr);
    }

    /* Wakeup server thread. */
    tx_semaphore_put(&sema_2);

    /* Shutdown one socket (the one in the middle of the group. */
    status = nx_tcp_socket_disconnect(&tcp_sockets[NUM_CLIENTS/2], NX_IP_PERIODIC_RATE);
    if(status == NX_NOT_CONNECTED || status == NX_DISCONNECT_FAILED)
        status = 0;
    
    if(tcp_sockets[NUM_CLIENTS/2].nx_tcp_socket_bound_next)
        status += nx_tcp_client_socket_unbind(&tcp_sockets[NUM_CLIENTS/2]);

    status += nx_tcp_socket_delete(&tcp_sockets[NUM_CLIENTS/2]);
    if(status != NX_SUCCESS)
        error_counter++;

    /* Now we should be able to try another connection */
    status = nx_tcp_client_socket_connect(&tcp_sockets[NUM_CLIENTS - 1], IP_ADDRESS(1, 2, 3, 4), 12345, NX_IP_PERIODIC_RATE);
    if(status != NX_SUCCESS)
    {
        /* At this point, we anticpate the BSD system to run out of resource and is unable to make
           this connection.  */
        error_counter++;
    }

    status = nx_packet_allocate(&pool_0, &packet_ptr, NX_TCP_PACKET, NX_NO_WAIT);
    status += nx_packet_data_append(packet_ptr, requests[(NUM_CLIENTS - 1) & 3], strlen(requests[(NUM_CLIENTS - 1) & 3]),
                                    &pool_0, NX_NO_WAIT);
    status += nx_tcp_socket_send(&tcp_sockets[NUM_CLIENTS - 1], packet_ptr, NX_IP_PERIODIC_RATE);
    if(status != NX_SUCCESS)
        error_counter++;
    
    status = nx_tcp_socket_receive(&tcp_sockets[NUM_CLIENTS - 1], &packet_ptr, 2 * NX_IP_PERIODIC_RATE);
    if(status != NX_SUCCESS)
        error_counter++;
    /* Validate the received data. */
    else if(packet_ptr -> nx_packet_length != strlen(response[(NUM_CLIENTS - 1) & 3]))
        error_counter++;
    else if(strncmp((char*)packet_ptr -> nx_packet_prepend_ptr, response[(NUM_CLIENTS - 1) & 3], packet_ptr -> nx_packet_length))
        error_counter++;
    if(status == NX_SUCCESS)
        nx_packet_release(packet_ptr);

    /* Wakeup server thread. */
    for(i = 0; i < NUM_CLIENTS - 2; i++)
    {

        /* Wakeup server thread. */
        tx_semaphore_put(&sema_1);
    }
    tx_semaphore_put(&sema_2);

    /* Shutdown the socket. */
    for(i = 0; i < NUM_CLIENTS; i++)
    {
    
        if(i == NUM_CLIENTS / 2)
            continue;

        status = nx_tcp_socket_disconnect(&tcp_sockets[i], 1 * NX_IP_PERIODIC_RATE);
        if((status == NX_SUCCESS) || (status == NX_NOT_CONNECTED) || (status == NX_DISCONNECT_FAILED))
            status = 0;
        else
            error_counter++;
        if(tcp_sockets[i].nx_tcp_socket_bound_next)
        {
            status = nx_tcp_client_socket_unbind(&tcp_sockets[i]);
            if(status)
                error_counter++;
        }

        status = nx_tcp_socket_delete(&tcp_sockets[i]);

        if(status != NX_SUCCESS)
            error_counter ++;        
        

    }
}

static void    ntest_1_entry(ULONG thread_input)
{

UINT            status;
ULONG           actual_status;


    /* Ensure the IP instance has been initialized.  */
    status =  nx_ip_status_check(&ip_1, NX_IP_INITIALIZE_DONE, &actual_status, 1 * NX_IP_PERIODIC_RATE);

    /* Check status...  */
    if (status != NX_SUCCESS)
    {

        printf("ERROR!\n");
        test_control_return(3);
    }

    /* Server run first. */
    tx_semaphore_get(&sema_1, 5 * NX_IP_PERIODIC_RATE);

    multiple_client4();

    /* Client finished. */
    tx_semaphore_put(&sema_0);

}


extern NX_BSD_SOCKET  nx_bsd_socket_array[NX_BSD_MAX_SOCKETS];
extern TX_BLOCK_POOL nx_bsd_socket_block_pool;
static void validate_bsd_structure(void)
{
int i;
    /* Make sure every BSD socket should be free by now. */
    
    for(i = 0; i < NX_BSD_MAX_SOCKETS; i++)
    {
        if(nx_bsd_socket_array[i].nx_bsd_socket_status_flags & NX_BSD_SOCKET_IN_USE)
        {
            error_counter++;
            return;
        }

        if(nx_bsd_socket_array[i].nx_bsd_socket_tcp_socket ||
           nx_bsd_socket_array[i].nx_bsd_socket_udp_socket)
        {
            error_counter++;
            return;
        }
    }
    
    /* Make sure all the NX SOCKET control blocks are released. */
    if(nx_bsd_socket_block_pool.tx_block_pool_available != 
       nx_bsd_socket_block_pool.tx_block_pool_total)
    {
        error_counter++;
        return;
    }

    /* Make sure all the sockets are released */
    if(ip_0.nx_ip_tcp_created_sockets_ptr ||
       ip_0.nx_ip_udp_created_sockets_ptr)
    {
        error_counter++;
        return;
    }
}

#else
extern void       test_control_return(UINT status);

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_bsd_tcp_accept_nonblocking_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   Basic BSD TCP Accept Nonblocking Test.........N/A\n"); 

    test_control_return(3);  
}      
#endif
