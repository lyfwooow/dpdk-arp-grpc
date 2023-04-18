#include <cstdio>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include "../data.hpp"
#include "ns.grpc.pb.h"
#include "ns.pb.h"

class NetStatsImpl final : public ns::NetStats::Service
{
public:
    ::grpc::Status
    GetNetStats(::grpc::ServerContext *context, const ::ns::Request *request, ::ns::Response *response) override
    {
        WriteLockGuard guard(lock); 
        NetStats st;
        st.num_arp = 0;
        st.num_bcast_arp = 0;
        st.num_ipv6 = 0;
        st.num_ipv4 = 0;
        st.num_multicast = 0;
        NetStats st_2;
        st_2.num_arp = 0;
        st_2.num_bcast_arp = 0;
        st_2.num_ipv6 = 0;
        st_2.num_ipv4 = 0;
        st_2.num_multicast = 0;
        for (auto &item : net_stats_list) {
            memcpy (st.pci_id, item.pci_id, sizeof(item.pci_id));
            st.num_arp += item.num_arp;
            st.num_bcast_arp += item.num_bcast_arp;
            st.num_ipv4 += item.num_ipv4;
            st.num_ipv6 += item.num_ipv6;
            st.num_multicast += item.num_multicast;
            for (auto it = item.arp_stats.begin(); it != item.arp_stats.end(); ++it) {
                st.arp_stats[it->first] += it->second;
            }
        }
	for (auto &item_2 : net_stats_list_2) {
            memcpy (st_2.pci_id, item_2.pci_id, sizeof(item_2.pci_id));
            st_2.num_arp += item_2.num_arp;
            st_2.num_bcast_arp += item_2.num_bcast_arp;
            st_2.num_ipv4 += item_2.num_ipv4;
            st_2.num_ipv6 += item_2.num_ipv6;
            st_2.num_multicast += item_2.num_multicast;
            for (auto it = item_2.arp_stats.begin(); it != item_2.arp_stats.end(); ++it) {
                st_2.arp_stats[it->first] += it->second;
            }
}

        uint32_t seconds = static_cast<uint32_t>(net_stats_list.size());
        response->set_pci_id(st.pci_id);
     	response->set_num_arp(st.num_arp);
        response->set_num_bcast_arp(st.num_bcast_arp);
        response->set_num_ipv4(st.num_ipv4);
        response->set_num_ipv6(st.num_ipv6);
        response->set_num_multicast(st.num_multicast);
        response->set_pci_id_2(st_2.pci_id);
        response->set_num_arp_2(st_2.num_arp);
        response->set_num_bcast_arp_2(st_2.num_bcast_arp);
        response->set_num_ipv4_2(st_2.num_ipv4);
        response->set_num_ipv6_2(st_2.num_ipv6);
        response->set_num_multicast_2(st_2.num_multicast);
        auto &arp_stats = *response->mutable_arp_stats();
        for (auto it = st.arp_stats.begin(); it != st.arp_stats.end(); ++it) {
            ns::ARPStats entry;
            entry.set_num_pkts(it->second);
            entry.set_rate(it->second / seconds);

            auto ip = ipv4_to_string(it->first);
            arp_stats.insert({ip, std::move(entry)});
        }

        return grpc::Status::OK;
    }
};

void run_server()
{
    std::string server_address("localhost:50051");
    NetStatsImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    printf("Server listening on %s\n", server_address.c_str());

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}
