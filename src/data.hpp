#pragma once

#include <cstdint>
#include <unordered_map>

class NetStats
{
  public:
    NetStats() : num_arp(0), num_ipv4(0), num_ipv6(0), num_multicast(0) {}

  private:
    uint32_t num_arp;
    uint32_t num_ipv4;
    uint32_t num_ipv6;
    uint32_t num_multicast;
    std::unordered_map<uint32_t, uint32_t> arp_stats;
};
