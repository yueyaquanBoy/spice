/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2009,2010 Red Hat, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_REDMEMSLOTS
#define _H_REDMEMSLOTS

#include "red_common.h"

#include <spice/qxl_dev.h>

typedef struct MemSlot {
    int generation;
    unsigned long virt_start_addr;
    unsigned long virt_end_addr;
    long address_delta;
} MemSlot;

typedef struct RedMemSlotInfo {
    MemSlot **mem_slots;
    uint32_t num_memslots_groups;
    uint32_t num_memslots;
    uint8_t mem_slot_bits;
    uint8_t generation_bits;
    uint8_t memslot_id_shift;
    uint8_t memslot_gen_shift;
    uint8_t internal_groupslot_id;
    unsigned long memslot_gen_mask;
    unsigned long memslot_clean_virt_mask;
} RedMemSlotInfo;

static inline int get_memslot_id(RedMemSlotInfo *info, unsigned long addr)
{
    return addr >> info->memslot_id_shift;
}

static inline int get_generation(RedMemSlotInfo *info, unsigned long addr)
{
    return (addr >> info->memslot_gen_shift) & info->memslot_gen_mask;
}

unsigned long get_virt_delta(RedMemSlotInfo *info, unsigned long addr, int group_id);
void validate_virt(RedMemSlotInfo *info, unsigned long virt, int slot_id,
                   uint32_t add_size, uint32_t group_id);
unsigned long get_virt(RedMemSlotInfo *info, unsigned long addr, uint32_t add_size,
                       int group_id);

void *validate_chunk (RedMemSlotInfo *info, QXLPHYSICAL data, uint32_t group_id, uint32_t *data_size_out, QXLPHYSICAL *next_out);
void *cb_get_virt(void *opaque, unsigned long addr,
                  uint32_t add_size, uint32_t group_id);
void cb_validate_virt(void *opaque,
                      unsigned long virt, unsigned long from_addr,
                      uint32_t add_size, uint32_t group_id);
void red_memslot_info_init(RedMemSlotInfo *info,
                           uint32_t num_groups, uint32_t num_slots,
                           uint8_t generation_bits,
                           uint8_t id_bits,
                           uint8_t internal_groupslot_id);
void red_memslot_info_add_slot(RedMemSlotInfo *info, uint32_t slot_group_id, uint32_t slot_id,
                               uint64_t addr_delta, unsigned long virt_start, unsigned long virt_end,
                               uint32_t generation);
void red_memslot_info_del_slot(RedMemSlotInfo *info, uint32_t slot_group_id, uint32_t slot_id);
void red_memslot_info_reset(RedMemSlotInfo *info);

#endif
