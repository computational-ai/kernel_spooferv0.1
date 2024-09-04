#include "network.hpp"
#include "utils.hpp"
#include <minwindef.h>
#include <wdm.h>
#include <ntddk.h>
#include <Windows.h>

NTSTATUS NICControl(PDEVICE_OBJECT device, PIRP irp) {
    PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
    switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_NDIS_QUERY_GLOBAL_STATS: {
        if (irp->MdlAddress) {
            SpoofBuffer(SEED, (PBYTE)MmGetSystemAddressForMdl(irp->MdlAddress), 6);  // Spoof MAC address
            KdPrint(("Handled NIC MAC spoofing\n"));
        }
        break;
    }
    }

    return NsiControlOriginal(device, irp);
}

VOID SpoofNetwork() {
    UNICODE_STRING networkDriverName = RTL_CONSTANT_STRING(L"\\Driver\\nsiproxy");
    SwapControl(networkDriverName, NICControl, &NsiControlOriginal);
    KdPrint(("Network MAC address spoofed\n"));
}
