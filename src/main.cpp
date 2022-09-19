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

#include "port.h"

void main_loop()
{
    for (;;) {
        uint16_t port_id;
        RTE_ETH_FOREACH_DEV(port_id)
        {
            // TODO
        }
    }
}

int main(int argc, char *argv[])
{
    int ret;

    ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(1, "Fail to initialize EAL\n");
    } else {
        argc -= ret;
        argv += ret;
        printf("Initialize EAL OK\n");
    }

    uint16_t nb_ports = rte_eth_dev_count_avail();
    if (nb_ports == 0) {
        rte_exit(1, "No available Ethernet ports\n");
    } else {
        printf("Available %" PRIu16 " Ethernet ports\n", nb_ports);
    }

    struct rte_mempool *mbuf_pool =
        rte_pktmbuf_pool_create("mbuf_pool", 65535, 512, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL) {
        rte_exit(1, "Fail to create a mbuf pool\n");
    }

    port_init_all(mbuf_pool);

    main_loop();

    port_finalize_all();

    ret = rte_eal_cleanup();
    if (ret < 0) {
        rte_exit(1, "Fail to finalize EAL\n");
    } else {
        printf("Finalize EAL OK\n");
    }

    return 0;
}
