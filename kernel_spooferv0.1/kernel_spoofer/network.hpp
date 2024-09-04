#pragma once
#include <ntddk.h>

extern PDRIVER_DISPATCH NsiControlOriginal;

void SpoofNetwork();
NTSTATUS NICControl(PDEVICE_OBJECT device, PIRP irp);
