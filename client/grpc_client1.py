from __future__ import print_function
import logging

import grpc
import grpc_pb2
import grpc_pb2_grpc

from tabulate import tabulate
import wcwidth

def run():
    str_ = input("input")
    if str_ == "show arp statistics" or str_ == "show multicast statistics":
        with grpc.insecure_channel('localhost:50051') as channel:
            stub = grpc_pb2_grpc.NetStatsStub(channel)
      
            list_result = {"a":grpc_pb2.ARPStats(num_pkts=1, rate=1)}
            response = stub.GetNetStats(grpc_pb2.Request(), grpc_pb2.Response(num_arp=0, num_ipv4=0, num_ipv6=0, num_multicast=0,arp_stats=list_result))
            if str_ == "show arp statistics":
                table_header = ['srcip','arp-request-pkts','rate']
                arp_list = []
                ip_list = list(response.arp_stats.keys())
                for i in range (response.num_arp):
                    newList = []
                    newList.append(str(ip_list[i]))
                    newList.append(str(response.num_arp))
                    newList.append(str(response.arp_stats.get(ip_list[i]).rate))
                    arp_list.append(newList)
                print(tabulate(arp_list, headers=table_header, tablefmt='grid'))
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

