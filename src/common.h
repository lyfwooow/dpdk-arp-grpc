#pragma once

#define IPV4_PRT_FMT "%d.%d.%d.%d"
#define IPV4_BYTES(ip) (((ip) >> 24) & 0xff), (((ip) >> 16) & 0xff), (((ip) >> 8) & 0xff), ((ip)&0xff)
