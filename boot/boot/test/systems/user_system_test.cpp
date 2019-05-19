﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "modular_sum.h"
#include "sif_header.h"
#include "systems/user_system.h"
}

namespace {
bool was_ran;
int Execute(int unused) {
  was_ran = true;
  return 0;
}
SifHeaderStruct dummy_file = {
    {0x7F, 'S', 'I', 'F', kSc64, kSd2Lsb, kSvCurrent},
    kStExe,
    kSmRx,
    0,
    sizeof(SifHeaderStruct),
    {0},
    0x0000000100000000,
    sizeof(SifHeaderStruct),
    0,
    reinterpret_cast<uintptr_t>(&dummy_file),
    0,
    0,
    reinterpret_cast<uintptr_t>(&Execute),
    "This is a dummy core file.",
};
}  // namespace

class UserSystemTest : public ::testing::Test {
 protected:
  ISystemExecutable instance;

  static void SetUpTestCase() {
    dummy_file.checksum = ModularSum_Calculate(
        reinterpret_cast<uint32_t*>(&dummy_file), dummy_file.file_size);
    UserSystem_Change(dummy_file.file_address);
  }

  virtual void SetUp() {
    was_ran = false;
    instance = UserSystem_getInstance();
  }
};

TEST_F(UserSystemTest, GetInstance) {
  EXPECT_EQ(instance, UserSystem_getInstance());
}

TEST_F(UserSystemTest, Execute) {
  EXPECT_FALSE(was_ran);
  EXPECT_EQ(0, instance->Execute());
  EXPECT_TRUE(was_ran);
}
