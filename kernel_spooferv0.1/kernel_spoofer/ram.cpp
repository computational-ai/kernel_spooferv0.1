#include "ram.hpp"
#include "utils.hpp"
#include <minwindef.h>

VOID SpoofRAM() {
    PVOID base = GetBaseAddress("ntoskrnl.exe");
    if (!base) {
        KdPrint(("Failed to get ntoskrnl.exe\n"));
        return;
    }

    PBYTE ramInfo = FindPatternImage(base, "\x48\x8B\x00\x24\x10\x48\x89", "xx?xxxx");  // RAM pattern
    if (ramInfo) {
        RandomizeSerial((char*)ramInfo, 16);  // Change RAM serial
        KdPrint(("RAM spoofed\n"));
    }
    else {
        KdPrint(("RAM pattern not found!\n"));
    }
}
