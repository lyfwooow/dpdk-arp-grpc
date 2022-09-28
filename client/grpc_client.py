from __future__ import print_function
import logging

import grpc
import grpc_pb2
import grpc_pb2_grpc

from tabulate import tabulate
import wcwidth

def run():
    str_ = input("请输入：")
    if str_ == "show arp statistics" or str_ == "show multicast statistics":
    # 建立连接
    # 使用with语法保证channel自动close
        with grpc.insecure_channel('localhost:50000') as channel:
        # 客户端通过stub来实现rpc通信
            stub = grpc_pb2_grpc.NetStatsStub(channel)
            
             # 客户端必须使用定义好的类型，这里是Request类型
            response = stub.GetNetStats(grpc_pb2.Request(data='hello'))
            #输出arp报文
            if str_ == "show arp statistics":
                table_header = ['srcip','arp-request-pkts','rate']
                arp_list = []
                #获得ip地址
                ip_list = list(response.arp_stats.keys())
                #输出arp报文
                for i in range (response.num_arp):
                    newList = []
                    newList.append(str(ip_list[i]))
                    newList.append(str(response.num_arp))
                    newList.append(str(response.arp_stats.get(ip_list[i]).rate))
                    arp_list.append(newList)
                print(tabulate(arp_list, headers=table_header, tablefmt='grid'))
            #输出multicast报文
            if str_ == "show multicast statistics":
                table_header = ['multicast-request-pkts']
                multicast_list = []
                for i in range (response.num_multicast):
                    newList = []
                    newList.append(str(response.num_multicast))
                    multicast_list.append(newList)
                print(tabulate(multicast_list, headers=table_header, tablefmt='grid'))

if __name__ == "__main__":
    logging.basicConfig()
    run()
