﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef BOOT_REGISTRY_INCLUDE_REGISTRY_API_H_
#define BOOT_REGISTRY_INCLUDE_REGISTRY_API_H_

#include <stdint.h>

enum RegistryFileApiError {
  kRfaNoError = 0,
  kRfaNoRegistryKeyError,
};

enum IntRegistryKey {
  kIrkDefaultRunlevel = 0,
  kIrkLowestRunlevel,
  kIrkHighestRunlevel,
  kIrkNumOfRunlevels,
  kIrkHaltModeRunlevel,
  kIrkRecoveryModeRunlevel,
  kIrkUpdateModeRunlevel,
  kIrkUserModeRunlevel,
  kIrkDeveloperModeRunlevel,
  kIrkRebootModeRunlevel,
};

enum UintptrRegistryKey {
  kUrkCoreFileAddress = 0,
  kUrkUpdaterFileAddress,
};

typedef struct RegistryApiStruct {
  int (*getIntRegistryValue)(int key);
  uintptr_t (*getUintptrRegistryValue)(int key);
} RegistryApiStruct, *RegistryApi;

#endif  // BOOT_REGISTRY_INCLUDE_REGISTRY_API_H_
