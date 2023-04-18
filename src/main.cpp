#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <rte_cycles.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>
#include <rte_mempool.h>
#include <rte_rwlock.h>

#include "common.h"
#include "data.hpp"
#include "port.h"
#include "process.hpp"
#include "proto/server.hpp"
#include "unistd.h"

uint32_t interval = 60;
static inline void print_stats(NetStats &st)
{
    for(int i = 0 ; i < sizeof(st.pci_id) ; i++ ){
        printf("%c",st.pci_id[i]);
    }
    printf("\n");
    printf(
        "num_arp: %" PRIu32 "\nnum_bcast_arp: % " PRIu32 "\nnum_ipv4: %" PRIu32 "\nnum_ipv6: %" PRIu32 "\nnum_multicast: %" PRIu32 "\n", st.num_arp,st.num_bcast_arp,
        st.num_ipv4, st.num_ipv6, st.num_multicast
    );
    for (auto it = st.arp_stats.begin(); it != st.arp_stats.end(); ++it) {
        printf(IPV4_PRT_FMT "\t%" PRIu32 "\n", IPV4_BYTES(it->first), it->second);
    }
   // printf("------------------------\n");
}

static inline void init_list()
{
    WriteLockGuard guard(lock);

    net_stats_list.clear();
    net_stats_list.emplace_front();
    net_stats_list_2.clear();
    net_stats_list_2.emplace_front();
}

__rte_noreturn int main_loop(__rte_unused void *arg)
{
    const uint64_t period = rte_get_tsc_hz();
    uint64_t cur_period = rte_get_tsc_cycles();

    for (;;) {
        uint16_t port_id;
        RTE_ETH_FOREACH_DEV(port_id)
        {   
            WriteLockGuard guard(lock);    
            auto &st=net_stats_list.front();
            auto &st_2=net_stats_list_2.front();
            if(port_id==0){
   //            auto st = net_stats_list.front();
                collect_stats(port_id, st);
            }else if (port_id==1)
            {
           //     st_2 = net_stats_list_2.front();
                collect_stats(port_id, st_2);
            }
            uint64_t cur_tsc = rte_get_timer_cycles();
            if (cur_tsc - cur_period >= period) {
                print_stats(st);
                print_stats(st_2);
                cur_period = cur_tsc;
                printf("------------------------\n");
                // 添加新节点，删除旧节点
                net_stats_list.emplace_front();
                if (net_stats_list.size() > interval)
                    net_stats_list.pop_back();
                net_stats_list_2.emplace_front();
                if (net_stats_list_2.size() > interval)
                    net_stats_list_2.pop_back(); 
               
               // printf("------------------------\n");
          }
        }
    }
}

int main(int argc, char *argv[])
{
    int ret;
    int ac = 0;
    char *av[] = {};
    ret = rte_eal_init(ac, av);    
    if (ret < 0) {
        rte_exit(1, "Fail to initialize EAL\n");
    } else {
        argc -= ret;
        argv += ret;
        printf("Initialize EAL OK\n");
    }

    /*uint16_t nb_ports = rte_eth_dev_count_avail();
    if (nb_ports == 0) {
        rte_exit(1, "No available Ethernet ports\n");
    } else {
        printf("Available %" PRIu16 " Ethernet ports\n", nb_ports);
    }*/

    struct rte_mempool *mbuf_pool =
        rte_pktmbuf_pool_create("mbuf_pool", 65535, 512, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL) {
        rte_exit(1, "Fail to create a mbuf pool\n");
    }
     
    if(argc == 3){
        if(strcmp(argv[1],"-i")==0){
           interval = atoi(argv[2]);
           if(interval < 1){
             rte_exit(1, "1.Please enter the correct parameters, example: monitor -i 60\n");
           }
        }else{
          rte_exit(1, "2.Please enter the correct parameters,example: monitor -i 60\n");
        }
    }else if (argc == 2)
    { 
        if(strcmp(argv[1],"-i")==0){
            rte_exit(1, "3.Please enter a parameter, example: monitor -i 60\n");
        }else{
            rte_exit(1, "4.Please enter the correct parameters, example: monitor -i 60\n");
        }
    }else if (argc == 1)
    {
        interval = 60;
    }else {
        rte_exit(1, "5.Please enter the correct parameters, example: monitor -i 60 \n");
    }
    port_init_all(mbuf_pool);

    init_list();
    unsigned int lcore_id = rte_get_next_lcore(-1, 1, 1);
    rte_eal_remote_launch(main_loop, NULL, lcore_id);

    run_server();

    rte_eal_mp_wait_lcore();

    port_finalize_all();

    ret = rte_eal_cleanup();
    if (ret < 0) {
        rte_exit(1, "Fail to finalize EAL\n");
    } else {
        printf("Finalize EAL OK\n");
    }

    return 0;
}
