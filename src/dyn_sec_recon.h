/*
 * Dynomite - A thin, distributed replication layer for multi non-distributed storages.
 * Copyright (C) 2015 Netflix, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *_stats_pool_set_ts
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* author: Ioannis Papapanagiotou */


#include "dyn_core.h"



struct recon_conn {
    struct context           *ctx;
    uint16_t                  port;           /* reconciliation monitoring port */
    int                       interval;       /* reconciliation aggregation interval */
    struct string             addr;           /* reconciliation monitoring address */

    int64_t                   start_ts;       /* reconciliation timestamp of dynomite */

    pthread_t                 tid;            /* reconciliation aggregator thread */
    int                       sd;             /* reconciliation descriptor */


};



struct recon_conn *recon_init(uint16_t recon_port, char *recon_ip, int recon_interval, char *source,
		                   struct context *ctx);
void recon_conn_destroy(struct recon_conn *cn);
