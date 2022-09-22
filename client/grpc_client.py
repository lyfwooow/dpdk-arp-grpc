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
            stub = grpc_pb2_grpc.GreeterStub(channel)
            
             # 客户端必须使用定义好的类型，这里是ReqMsg类型
            response = stub.GetMsg(grpc_pb2.ReqMsg(data='hello'))
            #输出arp报文
            if str_ == "show arp statistics":
                table_header = ['srcip','arp-request-pkts','rate']
                arp_list = []
                for i in range(len(response.arp_pkts)):
                    newList = []
                    newList.append(response.arp_pkts[i].scrip)
                    newList.append(str(response.arp_pkts[i].request_pkts_amount))
                    newList.append(str(response.arp_pkts[i].rate))
                    arp_list.append(newList)
                print(tabulate(arp_list, headers=table_header, tablefmt='grid'))
            #输出multicast报文
            if str_ == "show multicast statistics":
                table_header = ['multicast-request-pkts']
                multicast_list = []
                for i in range(len(response.multicast_pkts)):
                    newList = []
                    # newList.append(response.multicast_pkts[i].scrip)
                    newList.append(str(response.multicast_pkts[i].request_pkts_amount))
                    # newList.append(str(response.multicast_pkts[i].rate))
                    multicast_list.append(newList)
                print(tabulate(multicast_list, headers=table_header, tablefmt='grid'))

if __name__ == "__main__":
    logging.basicConfig()
    run()
