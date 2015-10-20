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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "dyn_core.h"


struct recon_conn *recon_init(uint16_t recon_port, char *recon_ip, int recon_interval, char *source,
		                   struct context *ctx){

    rstatus_t status;
    struct recon_conn *cn;

    cn = dn_alloc(sizeof(*cn));
    if (cn == NULL) {
       return NULL;
    }

    cn->port = recon_port;
    cn->interval = recon_interval;
    string_set_raw(&cn->addr, recon_ip);

    cn->recon_ts = (int64_t)time(NULL);

    status = recon_start(cn);
    if (status != DN_OK) {
       goto error;
    }

    return cn;

error:
    recon_conn_destroy(cn);
    return NULL;
}


void
recon_conn_destroy(struct recon_conn *cn)
{
    dn_free(cn);
}

static rstatus_t
recon_start(struct recon_conn *cn)
{
    rstatus_t status;

    THROW_STATUS(recon_listen(cn));

    status = pthread_create(&cn->tid, NULL, recon_loop, cn);
    if (status < 0) {
        log_error("reconciliation thread create failed: %s", strerror(status));
        return DN_ERROR;
    }

    return DN_OK;
}

static void *
recon_loop(void *arg)
{

	/* zipp file */
	/* create response */

}

static void
recon_stop(struct recon_conn *cn)
{
    close(cn->sd);
}

static rstatus_t
recon_listen(struct recon_conn *cn)
{
    rstatus_t status;
    struct sockinfo si;

    status = dn_resolve(&cn->addr, cn->port, &si);
    if (status < 0) {
        return status;
    }

    cn->sd = socket(si.family, SOCK_STREAM, 0);
    if (st->sd < 0) {
        log_error("reconciliation socket failed: %s", strerror(errno));
        return DN_ERROR;
    }

    status = dn_set_reuseaddr(cn->sd);
    if (status < 0) {
        log_error("reconciliation set reuseaddr on m %d failed: %s", cn->sd, strerror(errno));
        return DN_ERROR;
    }

    status = bind(st->sd, (struct sockaddr *)&si.addr, si.addrlen);
    if (status < 0) {
        log_error(" reconciliation bind on m %d to addr '%.*s:%u' failed: %s", cn->sd,
                  cn->addr.len, cn->addr.data, cn->port, strerror(errno));
        return DN_ERROR;
    }

    status = listen(cn->sd, SOMAXCONN);
    if (status < 0) {
        log_error("reconciliation listen on m %d failed: %s", cn->sd, strerror(errno));
        return DN_ERROR;
    }

    log_debug(LOG_NOTICE, "reconciliation m %d listening on '%.*s:%u'", cn->sd,
    		cn->addr.len, cn->addr.data, cn->port);

    return DN_OK;
}


