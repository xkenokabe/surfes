﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_REGISTRY_FIXED_API_H_
#define BOOT_REGISTRY_INCLUDE_REGISTRY_FIXED_API_H_

enum BootFileMetadata {
  kBootFileVersion = 0x0000000100000000,
  kBootFileAddress = 0xFFFFE000,
  kRegistryFileAddress = 0xFFFFC000,
};

#endif  // BOOT_REGISTRY_INCLUDE_REGISTRY_FIXED_API_H_
