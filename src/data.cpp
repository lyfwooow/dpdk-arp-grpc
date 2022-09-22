#include <list>

#include "data.hpp"

std::list<NetStats> net_stats_list;
RWLock lock;
