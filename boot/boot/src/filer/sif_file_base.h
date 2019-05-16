﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_BOOT_SRC_FILER_SIF_FILE_BASE_H_
#define BOOT_BOOT_SRC_FILER_SIF_FILE_BASE_H_

#include "sif_header.h"

typedef struct SifFileBaseStruct {
  uint64_t version;
  closeFunction Close;
} SifFileBaseStruct, *SifFileBase;

SifFileBase SifFileBase_Open(uintptr_t file_address, int instance_size);
void SifFileBase_Close(SifFileBase* self);
uint64_t SifFileBase_getVersion(SifFileBase self);

#endif  // BOOT_BOOT_SRC_FILER_SIF_FILE_BASE_H_
