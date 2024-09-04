Kernel-Level Hardware Spoofer (Windows Driver)

[ Overview ]

This project is a Windows kernel-mode driver designed to spoof various hardware identifiers, including disk drive serial numbers, SMBIOS serial numbers, motherboard serial numbers, and RAM serial numbers. It operates at the kernel level, intercepting I/O request packets (IRPs) and modifying system responses to provide spoofed hardware data.

[ Features ]

Disk Serial Number Spoofing: Intercepts disk-related IRPs to spoof the disk serial number returned by the system.
SMBIOS Spoofing: Modifies the SMBIOS tables in memory, which are responsible for storing system identifiers like BIOS version, system serial number, and motherboard information.
Motherboard Serial Number Spoofing: Changes the motherboard serial number in the SMBIOS structure.
RAM Serial Number Spoofing: Spoofs RAM module serial numbers, also through the SMBIOS structure.
Anti-Detection: The driver is designed to intercept hardware queries and modify responses dynamically without permanently altering hardware properties.
Clean Unloading: Restores original function handlers to ensure stability when the driver is unloaded.
