#include "motherboard.hpp"
#include "utils.hpp"

VOID SpoofMotherboard() {
    PVOID base = GetBaseAddress("hal.dll");
    if (!base) {
        KdPrint(("Failed to get hal.dll\n"));
        return;
    }

    PBYTE moboInfo = FindPatternImage(base, "\x48\x8B\xC4\x48\x89", "xxxxx");  // Motherboard serial pattern
    if (moboInfo) {
        RandomizeSerial((char*)moboInfo, 16);  // Change motherboard serial
        KdPrint(("Motherboard spoofed\n"));
    }
    else {
        KdPrint(("Motherboard pattern not found!\n"));
    }
}
