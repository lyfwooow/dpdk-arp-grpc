#include <cstdint>
#include <cstdio>

#include <rte_arp.h>
#include <rte_byteorder.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_mbuf.h>

#include "common.h"
#include "data.hpp"
#include "process.hpp"

#define NB_PKTS 4

static inline void collect_stats_arp(struct rte_arp_hdr *arp_hdr, NetStats &st)
{
    // 只考虑 ARP 请求
    if (rte_be_to_cpu_16(arp_hdr->arp_opcode) != RTE_ARP_OP_REQUEST)
        return;

    uint32_t src_ip = rte_be_to_cpu_32(arp_hdr->arp_data.arp_sip);
    st.arp_stats[src_ip]++;
    printf("Detect ARP requested by " IPV4_PRT_FMT "\n", IPV4_BYTES(src_ip));
}

void collect_stats(uint16_t port_id, NetStats &st)
{
    struct rte_mbuf *pkts[NB_PKTS];
    uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, pkts, NB_PKTS);

    for (uint16_t i = 0; i < nb_rx; i++) {
        struct rte_ether_hdr *ether_hdr = rte_pktmbuf_mtod(pkts[i], struct rte_ether_hdr *);
        uint16_t ether_type = rte_be_to_cpu_16(ether_hdr->ether_type);

        printf("Receive a packet from port %" PRIu16 ", type %04x\n", port_id, ether_type);

        switch (ether_type) {
        case RTE_ETHER_TYPE_ARP:
            collect_stats_arp(rte_pktmbuf_mtod_offset(pkts[i], struct rte_arp_hdr *, sizeof(struct rte_ether_hdr)), st);
            st.num_arp++;
            break;
        case RTE_ETHER_TYPE_IPV4:
            st.num_ipv4++;
            break;
        case RTE_ETHER_TYPE_IPV6:
            st.num_ipv6++;
            break;
        default:
            break;
        }

        if (rte_is_multicast_ether_addr(&ether_hdr->dst_addr)) {
            st.num_multicast++;
        }

        rte_pktmbuf_free(pkts[i]);
    }
}
