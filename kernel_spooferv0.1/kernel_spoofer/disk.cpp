#include "disk.hpp"
#include "utils.hpp"

NTSTATUS PartInfoIoc(PDEVICE_OBJECT device, PIRP irp, PVOID context) {
    if (context) {
        IOC_REQUEST request = *(PIOC_REQUEST)context;
        ExFreePool(context);

        if (request.BufferLength >= sizeof(PARTITION_INFORMATION_EX)) {
            PPARTITION_INFORMATION_EX info = (PPARTITION_INFORMATION_EX)request.Buffer;
            if (PARTITION_STYLE_GPT == info->PartitionStyle) {
                RtlZeroMemory(&info->Gpt.PartitionId, sizeof(GUID));
            }
        }

        if (request.OldRoutine && irp->StackCount > 1) {
            return request.OldRoutine(device, irp, request.OldContext);
        }
    }

    return STATUS_SUCCESS;
}

NTSTATUS PartControl(PDEVICE_OBJECT device, PIRP irp) {
    PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
    switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_DISK_GET_PARTITION_INFO_EX:
        ChangeIoc(ioc, irp, PartInfoIoc);
        break;
    case IOCTL_DISK_GET_DRIVE_LAYOUT_EX:
        ChangeIoc(ioc, irp, PartInfoIoc);
        break;
    }

    return PartControlOriginal(device, irp);
}

VOID SpoofDisks() {
    UNICODE_STRING diskDriverName = RTL_CONSTANT_STRING(L"\\Driver\\partmgr");
    SwapControl(diskDriverName, PartControl, &PartControlOriginal);
    KdPrint(("Disk spoofing enabled\n"));
}
