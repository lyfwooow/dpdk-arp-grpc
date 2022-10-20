#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

#include <grpcpp/grpcpp.h>

#include <cstdio>
#include "ns.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using ns::NetStats;
using ns::Request;
using ns::Response;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(NetStats::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int GetNetStats() {
    // Data we are sending to the server.
    Request request;

    // Container for the data we expect from the server.
    Response response;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetNetStats(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
        //std::cout << "@##################################################"
        //        << std::endl;
        std::cout << std::left;
        //std::cout << std::setw(12) << "scrip" << std::setw(12) << "arp-request-pkts" 
        //            << std::setw(12) << "rate"<< std::endl;
        //std::cout << "----------------------------------------------------" << std::endl;
	std::cout <<"arp total:  " << response.num_arp() << std::endl;
	std::cout <<"ipv4 total:  " << response.num_ipv4() << std::endl;
	std::cout <<"ipv6 total:  " << response.num_ipv6() << std::endl;
	std::cout <<"multicast total:  " << response.num_multicast() << std::endl;
	for(auto it : response.arp_stats()){
            ns::ARPStats tmp = it.second;
            std::cout << it.first <<std::setw(12) << "   amount: "<< tmp.num_pkts() << std::setw(12) << "    rate: " << tmp.rate() << std::endl;
	} 

	return 1;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

 private:
  std::unique_ptr<NetStats::Stub> stub_;
};

int main(int argc, char** argv) {
  GreeterClient greeter(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  int reply = greeter.GetNetStats();
 // std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
