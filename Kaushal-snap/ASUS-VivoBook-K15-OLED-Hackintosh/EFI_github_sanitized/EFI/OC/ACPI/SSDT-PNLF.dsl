/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20200925 (64-bit version)
 * Copyright (c) 2000 - 2020 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of /Volumes/1401/EFI/OC/ACPI/SSDT-PNLF.aml, Fri Nov  6 03:47:54 2026
 *
 * Original Table Header:
 *     Signature        "SSDT"
 *     Length           0x0000008D (141)
 *     Revision         0x02
 *     Checksum         0x88
 *     OEM ID           "ZPSS"
 *     OEM Table ID     "PNLF"
 *     OEM Revision     0x00000000 (0)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20241212 (539234834)
 */
DefinitionBlock ("", "SSDT", 2, "ZPSS", "PNLF", 0x00000000)
{
    External (_SB_.PC00.GFX0, DeviceObj)

    Device (\_SB.PC00.GFX0.PNLF)
    {
        Name (_HID, EisaId ("APP0002"))  // _HID: Hardware ID
        Name (_CID, "backlight")  // _CID: Compatible ID
        Name (_UID, 0x13)  // _UID: Unique ID
        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            If (_OSI ("Darwin"))
            {
                Return (0x0B)
            }
            Else
            {
                Return (Zero)
            }
        }
    }
}

