#include <ntddk.h>
#include "disk.hpp"
#include "usb.hpp"
#include "monitor.hpp"
#include "gpu.hpp"
#include "ram.hpp"
#include "motherboard.hpp"
#include "network.hpp"
#include "efi.hpp"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registry_path) {
    UNREFERENCED_PARAMETER(registry_path);
    driver->DriverUnload = DriverUnload;

    KdPrint(("++ Starting hardware spoofer ++\n"));

    SpoofDisks();
    SpoofUSB();
    SpoofMonitor();
    SpoofGPU();
    SpoofRAM();
    SpoofMotherboard();
    SpoofNetwork();
    SpoofEFI();

    KdPrint(("++ Hardware spoofing complete ++\n"));

    return STATUS_SUCCESS;
}

extern "C" VOID DriverUnload(PDRIVER_OBJECT driver) {
    UNREFERENCED_PARAMETER(driver);
    KdPrint(("-- Unloading driver --\n"));
}
