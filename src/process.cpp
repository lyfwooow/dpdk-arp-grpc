#include <cstdint>
#include <cstdio>

#include <rte_arp.h>
#include <rte_byteorder.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_mbuf.h>

#define IPV4_PRT_FMT "%d.%d.%d.%d"
#define IPV4_BYTES(ip) (((ip) >> 24) & 0xff), (((ip) >> 16) & 0xff), (((ip) >> 8) & 0xff), ((ip)&0xff)

#define NB_PKTS 4

static void collect_stats_arp(struct rte_arp_hdr *arp_hdr)
{
    /* 只考虑 ARP 请求 */
    if (rte_be_to_cpu_16(arp_hdr->arp_opcode) != RTE_ARP_OP_REQUEST)
        return;

    uint32_t src_ip = rte_be_to_cpu_32(arp_hdr->arp_data.arp_sip);
    printf("Detect ARP requested by " IPV4_PRT_FMT "\n", IPV4_BYTES(src_ip));
}

void collect_stats(uint16_t port_id)
{
    struct rte_mbuf *pkts[NB_PKTS];
    uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, pkts, NB_PKTS);

    for (size_t i = 0; i < nb_rx; i++) {
        struct rte_ether_hdr *ether_hdr = rte_pktmbuf_mtod(pkts[i], struct rte_ether_hdr *);
        uint16_t ether_type = rte_be_to_cpu_16(ether_hdr->ether_type);

        printf("Receive a packet from port %" PRIu16 ", type %04x\n", port_id, ether_type);

        switch (ether_type) {
        case RTE_ETHER_TYPE_ARP:
            struct rte_arp_hdr *arp_hdr =
                rte_pktmbuf_mtod_offset(pkts[i], struct rte_arp_hdr *, sizeof(struct rte_ether_hdr));
            collect_stats_arp(arp_hdr);
            break;
        case RTE_ETHER_TYPE_IPV4:
            break;
        case RTE_ETHER_TYPE_IPV6:
            break;
        default:
            break;
        }

        if (rte_is_multicast_ether_addr(&ether_hdr->dst_addr)) {
        }

        rte_pktmbuf_free(pkts[i]);
    }
}
