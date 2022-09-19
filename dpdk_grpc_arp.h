#ifndef _DPDK_GRPC_ARP_H_
#include <stdint.h>
#define ARP_INFO_MAX_TBL_NUM 100 //这里不确定max size

typedef struct arp_info_tbl_s{
	const char *srcip;
	uint32_t arp_request_pkts;
	uint32_t rate;
}arp_info_tbl_t;

typedef struct arp_pkts_tbl_s{
	uint32_t tbl_num;
	arp_info_tbl_t tbl_info[ARP_INFO_MAX_TBL_NUM];
}arp_pkts_tbl_t;

#endif