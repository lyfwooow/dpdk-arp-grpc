#pragma once

#include <stdint.h>

#include <rte_mempool.h>

#ifdef __cplusplus
extern "C" {
#endif

void port_init(uint16_t port_id, struct rte_mempool *mbuf_pool);
void port_init_all(struct rte_mempool *mbuf_pool);
void port_finalize(uint16_t port_id);
void port_finalize_all();

#ifdef __cplusplus
}
#endif
