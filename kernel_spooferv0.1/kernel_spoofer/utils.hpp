#pragma once
#include <ntddk.h>

void RandomizeSerial(char* buffer, size_t length);
PBYTE FindPatternImage(PVOID base, const char* pattern, const char* mask);
PVOID GetBaseAddress(const char* driverName);
