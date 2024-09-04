#include "usb.hpp"
#include "utils.hpp"

VOID SpoofUSB() {
    PVOID base = GetBaseAddress("usbport.sys");
    if (!base) {
        KdPrint(("Failed to get usbport.sys\n"));
        return;
    }

    PBYTE USBDeviceDescriptor = FindPatternImage(base, "\x12\x01", "xx");  // Locate USB descriptor
    if (USBDeviceDescriptor) {
        RandomizeSerial((char*)USBDeviceDescriptor + 8, 4);  // Vendor ID
        RandomizeSerial((char*)USBDeviceDescriptor + 10, 4);  // Product ID
        KdPrint(("USB device spoofed\n"));
    }
    else {
        KdPrint(("USB device not found!\n"));
    }
}
