// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "fake_memorypool.h"

#include "instance_helper.h"

enum {
  kLowestId = 0,
  kHighestId = CFN_MAX_MPLID - 1,
  kNonsenseId = ~0,
};

typedef struct {
  ATR mplatr;
  SZ mplsz;
  SZ used_size;
} MailboxControlBlockStruct;

static MailboxControlBlockStruct memorypool_control_blocks[CFN_MAX_MPLID];

static void InitBlock(ID mplid) {
  memorypool_control_blocks[mplid].mplatr = ~0;
  memorypool_control_blocks[mplid].mplsz = 0;
  memorypool_control_blocks[mplid].used_size = 0;
}

void fake_memorypool_init(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i) InitBlock(i);
}

ATR fake_memorypool_getAttribute(ID mplid) {
  return memorypool_control_blocks[mplid].mplatr;
}

SZ fake_memorypool_getSize(ID mplid) {
  return memorypool_control_blocks[mplid].mplsz;
}

SZ fake_memorypool_getUsedSize(ID mplid) {
  return memorypool_control_blocks[mplid].used_size;
}

bool fake_memorypool_isCreated(ID mplid) {
  return memorypool_control_blocks[mplid].mplsz != 0;
}

static ID RetrieveUnusedId(void) {
  for (ID i = kLowestId; i <= kHighestId; ++i)
    if (!memorypool_control_blocks[i].mplsz) return i;
  return kNonsenseId;
}

ID tk_cre_mpl(CONST T_CMPL *pk_cmpl) {
  if (!pk_cmpl || (pk_cmpl->mplsz <= 0)) return E_PAR;

  ID mplid = RetrieveUnusedId();
  if (mplid == kNonsenseId) return E_LIMIT;

  memorypool_control_blocks[mplid].mplatr = pk_cmpl->mplatr;
  memorypool_control_blocks[mplid].mplsz = pk_cmpl->mplsz;
  return mplid;
}

ER tk_del_mpl(ID mplid) {
  if (mplid < kLowestId || mplid > kHighestId) return E_ID;
  if (!memorypool_control_blocks[mplid].mplsz) return E_NOEXS;

  InitBlock(mplid);
  return E_OK;
}

ER tk_get_mpl(ID mplid, SZ blksz, void **p_blk, TMO tmout) {
  if (mplid < kLowestId || mplid > kHighestId) return E_ID;
  if (!memorypool_control_blocks[mplid].mplsz) return E_NOEXS;
  if (tmout < TMO_FEVR) return E_PAR;
  if ((memorypool_control_blocks[mplid].used_size + blksz) >
      memorypool_control_blocks[mplid].mplsz)
    return E_TMOUT;

  void *block = InstanceHelper_New(blksz + sizeof(W));
  W *block_size = (W *)block;
  *block_size = blksz;
  *p_blk = (void *)(block + sizeof(W));
  memorypool_control_blocks[mplid].used_size += blksz;
  return E_OK;
}

ER tk_rel_mpl(ID mplid, void *blk) {
  if (mplid < kLowestId || mplid > kHighestId) return E_ID;
  if (!memorypool_control_blocks[mplid].mplsz) return E_NOEXS;
  if (!blk) return E_PAR;

  void *block = (void *)(blk - sizeof(W));
  W *block_size = (W *)block;
  memorypool_control_blocks[mplid].used_size -= *block_size;
  InstanceHelper_Delete(&block);
  return E_OK;
}