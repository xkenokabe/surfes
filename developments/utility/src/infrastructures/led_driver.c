﻿// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "led_driver.h"
#include "led_driver_private.h"

#include <stddef.h>
#include <stdlib.h>

#include "instance_helper.h"

enum LedNumber {
  kFirstLed = 1,
  kLastLed = 8,
};

static bool Validate(uint8_t* io_address, ledDecoder decoder) {
  return io_address && decoder;
}

static LedDriver NewInstance(uint8_t* io_address, ledDecoder decoder) {
  LedDriver self = (LedDriver)InstanceHelper_New(sizeof(LedDriverStruct));
  if (self) {
    self->io_address = io_address;
    self->decoder = decoder;
  }
  return self;
}

LedDriver LedDriver_Create(uint8_t* io_address, ledDecoder decoder) {
  if (!Validate(io_address, decoder)) return NULL;

  LedDriver self = NewInstance(io_address, decoder);
  if (self) LedDriver_TurnAllOff(self);
  return self;
}

void LedDriver_Destroy(LedDriver* self) {
  if (!self || !*self) return;

  LedDriver_TurnAllOff(*self);
  InstanceHelper_Delete(self);
}

static bool IsValid(int led_number) {
  return (led_number >= kFirstLed) && (led_number <= kLastLed);
}

static void SetLedImageBit(LedDriver self, int led_number) {
  *self->io_address |= self->decoder(led_number);
}

void LedDriver_TurnOn(LedDriver self, int led_number) {
  if (!self || !IsValid(led_number)) return;

  SetLedImageBit(self, led_number);
}

static void ClearLedImageBit(LedDriver self, int led_number) {
  *self->io_address &= ~self->decoder(led_number);
}

void LedDriver_TurnOff(LedDriver self, int led_number) {
  if (!self || !IsValid(led_number)) return;

  ClearLedImageBit(self, led_number);
}

static void SetAllLedImageBits(LedDriver self) {
  for (int i = kFirstLed; i <= kLastLed; i++)
    *self->io_address |= self->decoder(i);
}

void LedDriver_TurnAllOn(LedDriver self) {
  if (!self) return;

  SetAllLedImageBits(self);
}

static void ClearAllLedImageBits(LedDriver self) {
  for (int i = kFirstLed; i <= kLastLed; i++)
    *self->io_address &= ~self->decoder(i);
}

void LedDriver_TurnAllOff(LedDriver self) {
  if (!self) return;

  ClearAllLedImageBits(self);
}

static bool IsLedImageBitOn(LedDriver self, int led_number) {
  return *self->io_address & self->decoder(led_number);
}

bool LedDriver_IsOn(LedDriver self, int led_number) {
  if (!self || !IsValid(led_number)) return false;

  return IsLedImageBitOn(self, led_number);
}

bool LedDriver_IsOff(LedDriver self, int led_number) {
  return !LedDriver_IsOn(self, led_number);
}
