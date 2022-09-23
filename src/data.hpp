#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>

#include <rte_rwlock.h>

std::string ipv4_to_string(uint32_t ip)
{
    return std::to_string((ip >> 24) & 0xff) + "." + std::to_string((ip >> 16) & 0xff) + "." +
           std::to_string((ip >> 8) & 0xff) + "." + std::to_string(ip & 0xff);
}

class NetStats
{
public:
    NetStats() : num_arp(0), num_ipv4(0), num_ipv6(0), num_multicast(0) {}

    uint32_t num_arp;
    uint32_t num_ipv4;
    uint32_t num_ipv6;
    uint32_t num_multicast;
    std::unordered_map<uint32_t, uint32_t> arp_stats;
};

class RWLock
{
public:
    RWLock() { rte_rwlock_init(&lock); }
    RWLock(const RWLock &) = delete;

    void read_lock() { rte_rwlock_read_lock(&lock); }
    void read_unlock() { rte_rwlock_read_unlock(&lock); }

    void write_lock() { rte_rwlock_write_lock(&lock); }
    void write_unlock() { rte_rwlock_write_unlock(&lock); }

private:
    rte_rwlock_t lock;
};

class ReadLockGuard
{
public:
    ReadLockGuard(RWLock &lock) : l(&lock) { l->read_lock(); }
    ReadLockGuard(const ReadLockGuard &) = delete;

    ~ReadLockGuard() { l->read_unlock(); }

private:
    RWLock *l;
};

class WriteLockGuard
{
public:
    WriteLockGuard(RWLock &lock) : l(&lock) { l->write_lock(); }
    WriteLockGuard(const WriteLockGuard &) = delete;

    ~WriteLockGuard() { l->write_unlock(); }

private:
    RWLock *l;
};

extern std::list<NetStats> net_stats_list;
extern RWLock lock;
