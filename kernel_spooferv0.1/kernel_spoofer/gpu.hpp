#pragma once
#include <ntddk.h>

extern PDRIVER_DISPATCH GpuControlOriginal;

void SpoofGPU();
NTSTATUS GpuControl(PDEVICE_OBJECT device, PIRP irp);
