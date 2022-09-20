#include "dpdk_grpc_arp.h"
#include "grpc.pb-c.h"
#include "string.h"
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

//typedef struct GrpcDpdkArpReq GrpcDpdkArpReq;
//typedef struct GrpcDpdkArpRes GrpcDpdkArpRes;
//typedef struct GrpcDpdkArpRes__TablePkts GrpcDpdkArpRes__TablePkts;

static int get_dpdk_arp_table(void* in_buf, uint32_t in_len, void* out_buf, uint32_t* out_len)
{
    GrpcDpdkArpRes res = SGW_TABLE_GET_USAGE_RES__INIT;
    int ret = EOK;
    arp_pkts_tbl_t bf_arp;
    GrpcDpdkArpReq* msg = NULL;
    GrpcDpdkArpRes__TablePkts table_data[ARP_INFO_MAX_TBL_NUM];
    GrpcDpdkArpRes__TablePkts* pointer_table_data[ARP_INFO_MAX_TBL_NUM];
    int index=0;

    memset(&bf_arp, 0, sizeof bf_arp);

    msg = grpc_dpdk_arp_req__unpack(NULL, in_len, in_buf);
    ret = arp_packet_tbl_s(&bf_arp);
    if(0!=ret){
        //"Err: arp get failed ret (%d).\n"
        goto out;
    }

    res.arp_pkts = pointer_table_data;
    if(bf_arp.tbl_num == 0){
        goto out;
    }
    for (int i = 0; i < bf_arp.tbl_num; ++i) {
//        if(bf_arp.tbl_info[i].arp_request_pkts = null){
//            continue;
//        }

        pointer_table_data[i] = &table_data[i];
        grpc_dpdk_arp_res__table_pkts__init(&table_data[i]);

        table_data[i].scrip = bf_arp.tbl_info[i].srcip;
        table_data[i].request_pkts_amount = bf_arp.tbl_info[i].arp_request_pkts;
        table_data[i].rate = bf_arp.tbl_info[i].rate;
    }
    //res.n_usage = index;
    //
out:
    *out_len = grpc_dpdk_arp_res__pack(&res, out_buf);

    if(msg){
        grpc_dpdk_arp_req__free_unpacked(msg, null);
    }
    return ret;
}





