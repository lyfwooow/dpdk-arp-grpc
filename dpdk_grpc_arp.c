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
	arp_params->tbl_num=1; //表格的行数
	for(i=0;i<arp_params->tbl_num;i++)
	{
		// memset(&bf_data,0,sizeof(bf_data));
		// bf_ret = dpos_bf_rt_table_op_new(&bf_rt_ctl,tbl_name[i],
		// 					DPOS_TABLE_USAGE_GET,
		// 					NULL,
		// 					&bf_data,
		// 					true);
		if (bf_ret != BF_SUCCESS)
		{
			app_debug(LOG_ERR, "%s getting arp packet failed %d   \r\n");
			continue;       
		}  	
		
		arp_params->tbl_info[i].srcip= 
		arp_params->tbl_info[i].arp_request_pkts = 
		arp_params->tbl_info[i].rate = 
	}

    return 0;    
}

//获取arp包信息并解析


//抓包