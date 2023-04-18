#include <list>

#include "data.hpp"

std::list<NetStats> net_stats_list;
std::list<NetStats> net_stats_list_2;
RWLock lock;
