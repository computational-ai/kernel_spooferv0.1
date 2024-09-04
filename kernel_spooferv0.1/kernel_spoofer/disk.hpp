#pragma once
#include <ntddk.h>

extern PDRIVER_DISPATCH PartControlOriginal;

void SpoofDisks();
NTSTATUS PartInfoIoc(PDEVICE_OBJECT device, PIRP irp, PVOID context);
NTSTATUS PartControl(PDEVICE_OBJECT device, PIRP irp);
