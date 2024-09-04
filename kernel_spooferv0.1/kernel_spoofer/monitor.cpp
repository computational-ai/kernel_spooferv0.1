#include "monitor.hpp"
#include "utils.hpp"

VOID SpoofMonitor() {
    PVOID base = GetBaseAddress("dxgkrnl.sys");
    if (!base) {
        KdPrint(("Failed to get dxgkrnl.sys\n"));
        return;
    }

    PBYTE edidData = FindPatternImage(base, "\x00\xFF\xFF\xFF\xFF\xFF\xFF\x00", "xxxxxxxx");
    if (edidData) {
        RandomizeSerial((char*)edidData + 10, 8);  // Change serial number in EDID
        KdPrint(("Monitor EDID spoofed\n"));
    }
    else {
        KdPrint(("EDID pattern not found!\n"));
    }
}
