/*
   srcip           arp-request-pkts    rate
   ------------    ----------------    -----
   10.137.31.11    1000                10/s
   10.137.31.12    100                 20/s
*/
int sgw_p4_tbl_usage_get( sgw_p4_tbl_t *bf_usage)
{
    bf_status_t bf_ret=0;
    uint32_t i = 0;
    dpos_bf_rt_data_t bf_data;
	const char *tbl_name[16]={		
				"pipe0.Egress_P0.acl.egress_acl",
				"pipe0.Egress_P0.meter_action.meter_action", 
				"pipe0.Ingress_P0.dp.vrf",                   
				"pipe0.Ingress_P0.dp.dpout.dpout_id_common", 
				"pipe0.Ingress_P0.dp.dpout.dpout_id_pre",    
				"pipe0.Ingress_P0.dp.dpin.dpin_id_pre",      
				"pipe0.Ingress_P0.dp.dpin.dpin_id_common",   
				"pipe0.Ingress_P0.acl.ingress_acl",         
				"pipe0.Ingress_P0.dispatch.local_dispatch",  
				"pipe0.Ingress_P0.dispatch.dp_dispatch",     
				"pipe0.Ingress_P0.dispatch.vlan_map",        
				"pipe0.Ingress_P0.config.skip_meter",        
				"pipe0.Ingress_P0.config.port_info",         
				"pipe0.Ingress_P0.config.director",          
				"pipe1.Egress_P1.meter.meter_table",         
				"pipe1.Ingress_P1.meter_action.meter_action"};

	bf_usage->tbl_num=sizeof(tbl_name)/sizeof(tbl_name[0]);
	for(i=0;i<bf_usage->tbl_num;i++)
	{
		memset(&bf_data,0,sizeof(bf_data));
		bf_ret = dpos_bf_rt_table_op_new(&bf_rt_ctl,tbl_name[i],
							DPOS_TABLE_USAGE_GET,
							NULL,
							&bf_data,
							true);
		if (bf_ret != BF_SUCCESS)
		{
			app_debug(LOG_ERR, "%s usage get failed %d   \r\n",tbl_name[i],bf_ret);
			continue;       
		}  	
		
		bf_usage->tbl_info[i].cur_entry_num= bf_data.field_info[0].array_num;
		bf_usage->tbl_info[i].max_size = bf_data.field_info[1].array_num;
		bf_usage->tbl_info[i].name = tbl_name[i];
	}

    return 0;    
}