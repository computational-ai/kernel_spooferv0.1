#include "gpu.hpp"
#include "utils.hpp"

NTSTATUS GpuControl(PDEVICE_OBJECT device, PIRP irp) {
    PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
    switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_NVIDIA_SMIL: {
        NTSTATUS ret = GpuControlOriginal(device, irp);

        PCHAR buffer = (PCHAR)irp->UserBuffer;  // Cast to PCHAR
        if (buffer) {
            for (DWORD i = 0; i < IOCTL_NVIDIA_SMIL_MAX - 4; ++i) {
                if (0 == memcmp(buffer + i, "GPU-", 4)) {
                    RtlZeroMemory(buffer + i, 4);  // Clear the GPU identifier
                    KdPrint(("Handled GPU serial spoofing\n"));
                    break;
                }
            }
        }

        return ret;
    }
    }

    return GpuControlOriginal(device, irp);
}

VOID SpoofGPU() {
    UNICODE_STRING gpuDriverName = RTL_CONSTANT_STRING(L"\\Driver\\nvlddmkm");
    SwapControl(gpuDriverName, GpuControl, &GpuControlOriginal);
    KdPrint(("NVIDIA GPU spoofed\n"));
}
