#include "efi.hpp"
#include "utils.hpp"

/**** ADVANCED SMBIOS SPOOFING ****/

// Structure for SMBIOS Entry Point
typedef struct _SMBIOS_ENTRY_POINT {
    CHAR AnchorString[4];
    UCHAR EntryPointLength;
    UCHAR MajorVersion;
    UCHAR MinorVersion;
    UCHAR MaxStructureSize[2];
    UCHAR EntryPointRevision;
    UCHAR FormattedArea[5];
    CHAR IntermediateAnchorString[5];
    UCHAR IntermediateChecksum;
    USHORT TableLength;
    ULONG TableAddress;
    USHORT NumberOfStructures;
    UCHAR SMBIOSBCDRevision;
} SMBIOS_ENTRY_POINT, * PSMBIOS_ENTRY_POINT;

// SMBIOS Table structure - header for every table
typedef struct _SMBIOS_STRUCTURE_HEADER {
    UCHAR Type;
    UCHAR Length;
    USHORT Handle;
} SMBIOS_STRUCTURE_HEADER, * PSMBIOS_STRUCTURE_HEADER;

// Types for specific SMBIOS tables
#define SMBIOS_TYPE_BIOS_INFORMATION 0x00
#define SMBIOS_TYPE_SYSTEM_INFORMATION 0x01
#define SMBIOS_TYPE_BASEBOARD_INFORMATION 0x02
#define SMBIOS_TYPE_MEMORY_DEVICE 0x17

// New utility to locate SMBIOS Entry Point
PSMBIOS_ENTRY_POINT GetSMBIOSEntryPoint() {
    PVOID address = GetBaseAddress("ntoskrnl.exe");
    if (!address) {
        KdPrint(("Failed to get ntoskrnl.exe base address\n"));
        return NULL;
    }

    // Pattern search to locate SMBIOS entry point
    PBYTE smbiosEntryPoint = FindPatternImage(address, "_SM_", "xxxx");  // "_SM_" is common signature
    if (smbiosEntryPoint) {
        KdPrint(("SMBIOS entry point located\n"));
        return (PSMBIOS_ENTRY_POINT)smbiosEntryPoint;
    }

    KdPrint(("SMBIOS entry point not found\n"));
    return NULL;
}

// Spoof specific system information in SMBIOS
VOID SpoofSMBIOSTable(UCHAR* table) {
    PSMBIOS_STRUCTURE_HEADER header = (PSMBIOS_STRUCTURE_HEADER)table;

    // Process based on table type
    switch (header->Type) {
    case SMBIOS_TYPE_BIOS_INFORMATION:
        KdPrint(("Spoofing BIOS Information\n"));
        RandomizeSerial((char*)(table + 4), 16);  // Modify BIOS version string
        break;

    case SMBIOS_TYPE_SYSTEM_INFORMATION:
        KdPrint(("Spoofing System Information\n"));
        RandomizeSerial((char*)(table + 8), 16);  // Modify System serial number
        break;

    case SMBIOS_TYPE_BASEBOARD_INFORMATION:
        KdPrint(("Spoofing Baseboard Information\n"));
        RandomizeSerial((char*)(table + 8), 16);  // Modify Baseboard serial number
        break;

    case SMBIOS_TYPE_MEMORY_DEVICE:
        KdPrint(("Spoofing Memory Device Information\n"));
        RandomizeSerial((char*)(table + 12), 16);  // Modify Memory Device serial number
        break;

    default:
        KdPrint(("Skipping unrecognized SMBIOS table type: %d\n", header->Type));
        break;
    }
}

VOID SpoofSMBIOS() {
    PSMBIOS_ENTRY_POINT smbiosEntryPoint = GetSMBIOSEntryPoint();
    if (!smbiosEntryPoint) {
        KdPrint(("Failed to locate SMBIOS entry point, aborting spoof\n"));
        return;
    }

    // Access the SMBIOS table starting at the address found in the entry point
    PVOID smbiosTable = (PVOID)(ULONG_PTR)(smbiosEntryPoint->TableAddress);
    if (!smbiosTable) {
        KdPrint(("Failed to access SMBIOS table\n"));
        return;
    }

    KdPrint(("Processing SMBIOS table\n"));

    // Iterate through the SMBIOS structures to spoof relevant ones
    UCHAR* currentTable = (UCHAR*)smbiosTable;
    for (USHORT i = 0; i < smbiosEntryPoint->NumberOfStructures; i++) {
        SpoofSMBIOSTable(currentTable);

        // Move to the next structure (length of current table + 2 null bytes as the string terminator)
        PSMBIOS_STRUCTURE_HEADER header = (PSMBIOS_STRUCTURE_HEADER)currentTable;
        currentTable += header->Length;

        // Skip strings, each table is followed by string-set terminated with two zero bytes
        while (!(currentTable[0] == 0 && currentTable[1] == 0)) {
            currentTable++;
        }
        currentTable += 2;  // Skip the terminating null bytes
    }

    KdPrint(("Finished SMBIOS spoofing\n"));
}
