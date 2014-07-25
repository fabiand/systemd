/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

#pragma once

/***
  This file is part of systemd.

  Copyright 2014 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <sys/types.h>

#include "hashmap.h"
#include "prioq.h"
#include "time-util.h"
#include "list.h"

typedef struct DnsCacheItem DnsCacheItem;

typedef struct DnsCache {
        Hashmap *by_key;
        Prioq *by_expiry;
} DnsCache;

#include "resolved-dns-rr.h"
#include "resolved-dns-question.h"
#include "resolved-dns-answer.h"

typedef enum DnsCacheItemType {
        DNS_CACHE_POSITIVE,
        DNS_CACHE_NODATA,
        DNS_CACHE_NXDOMAIN,
} DnsCacheItemType;

typedef struct DnsCacheItem {
        DnsResourceKey *key;
        DnsResourceRecord *rr;
        usec_t until;
        DnsCacheItemType type;
        unsigned prioq_idx;
        LIST_FIELDS(DnsCacheItem, by_key);
} DnsCacheItem;

void dns_cache_flush(DnsCache *c);
void dns_cache_prune(DnsCache *c);

int dns_cache_put(DnsCache *c, DnsQuestion *q, int rcode, DnsAnswer *answer, usec_t timestamp);
int dns_cache_lookup(DnsCache *c, DnsQuestion *q, int *rcode, DnsAnswer **answer);