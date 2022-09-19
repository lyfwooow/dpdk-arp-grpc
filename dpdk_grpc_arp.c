#include "dpdk_grpc_arp.h"

#define LOG_ERR  1

/*
   srcip           arp-request-pkts    rate
   ------------    ----------------    -----
   10.137.31.11    1000                10/s
   10.137.31.12    100                 20/s
*/

//生成表格
int arp_packet_tbl_s( arp_pkts_tbl_t *arp_params)
{
	int bg_ret = 0;
	arp_params->tbl_num=1; //表格的行数
	arp_pkts_tbl_t *new_arp_info_tbl;
	uint32_t i = 0;
	for(i=0;i<arp_params->tbl_num;i++)
	{
		// memset(&bf_data,0,sizeof(bf_data));
		bg_ret = arp_pkt_parse(&new_arp_info_tbl);
		// if (bf_ret != BF_SUCCESS)
		// {
		// 	app_debug(LOG_ERR, "%s getting arp packet failed %d   \r\n");
		// 	continue;       
		// }  	
		
		arp_params->tbl_info[i].srcip = 
		arp_params->tbl_info[i].arp_request_pkts = 
		arp_params->tbl_info[i].rate = 
	}

    return 0;    
}

//抓包，做统计

//包的结构





