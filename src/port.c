#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_mempool.h>

#include "port.h"

void port_init(uint16_t port_id, struct rte_mempool *mbuf_pool)
{
    int ret;

    struct rte_eth_dev_info dev_info;
    ret = rte_eth_dev_info_get(port_id, &dev_info);
    struct rte_device *device=dev_info.device;
    printf("device info is %s \n",device->name);   
 if (ret < 0) {
        rte_exit(1, "rte_eth_dev_info_get: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    /* 配置设备 */
    struct rte_eth_conf eth_conf = {};
    ret = rte_eth_dev_configure(port_id, 1, 1, &eth_conf);
    if (ret < 0) {
        rte_exit(1, "rte_eth_dev_configure: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    uint16_t nb_rx_desc = 512;
    uint16_t nb_tx_desc = 512;
    ret = rte_eth_dev_adjust_nb_rx_tx_desc(port_id, &nb_rx_desc, &nb_tx_desc);
    if (ret < 0) {
        rte_exit(1, "rte_eth_dev_adjust_nb_rx_tx_desc: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    int socket_id = rte_eth_dev_socket_id(port_id);
    if (socket_id < 0) {
        rte_exit(1, "rte_eth_dev_socket_id: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    /* 配置接收队列 */
    ret = rte_eth_rx_queue_setup(port_id, 0, nb_rx_desc, socket_id, NULL, mbuf_pool);
    if (ret < 0) {
        rte_exit(1, "rte_eth_rx_queue_setup: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    /* 配置发送队列 */
    ret = rte_eth_tx_queue_setup(port_id, 0, nb_tx_desc, socket_id, NULL);
    if (ret < 0) {
        rte_exit(1, "rte_eth_tx_queue_setup: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    /* 启动设备 */
    ret = rte_eth_dev_start(port_id);
    if (ret < 0) {
        rte_exit(1, "rte_eth_dev_start: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    /* 启用混杂模式 */
    ret = rte_eth_promiscuous_enable(port_id);
    if (ret < 0) {
        rte_exit(1, "rte_eth_promiscuous_enable: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    struct rte_ether_addr mac_addr;
    ret = rte_eth_macaddr_get(port_id, &mac_addr);
    if (ret < 0) {
        rte_exit(1, "rte_eth_macaddr_get: err=%d, port=%" PRIu16 "\n", ret, port_id);
    } else {
        printf(
            "Initialize port %" PRIu16 ", mac " RTE_ETHER_ADDR_PRT_FMT "\n", port_id, RTE_ETHER_ADDR_BYTES(&mac_addr));
    }
}

void port_init_all(struct rte_mempool *mbuf_pool)
{
    uint16_t port_id;
    RTE_ETH_FOREACH_DEV(port_id)
    {
        port_init(port_id, mbuf_pool);
    }
}

void port_finalize(uint16_t port_id)
{
    int ret;

    ret = rte_eth_dev_stop(port_id);
    if (ret < 0) {
        rte_exit(1, "rte_eth_dev_stop: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }

    ret = rte_eth_dev_close(port_id);
    if (ret < 0) {
        rte_exit(1, "rte_eth_dev_close: err=%d, port=%" PRIu16 "\n", ret, port_id);
    }
}

void port_finalize_all()
{
    uint16_t port_id;
    RTE_ETH_FOREACH_DEV(port_id)
    {
        port_finalize(port_id);
    }
}
