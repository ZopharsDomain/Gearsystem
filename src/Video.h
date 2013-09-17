/*
 * Gearsystem - Sega Master System / Game Gear Emulator
 * Copyright (C) 2013  Ignacio Sanchez

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/ 
 * 
 */

#ifndef VIDEO_H
#define	VIDEO_H

#include "definitions.h"

#define VDP_READ_VRAM_OPERATION 0x00
#define VDP_WRITE_VRAM_OPERATION 0x01
#define VDP_WRITE_REG_OPERATION 0x02
#define VDP_WRITE_CRAM_OPERATION 0x03

class Memory;
class Processor;

class Video
{
public:
    Video(Memory* pMemory, Processor* pProcessor);
    ~Video();
    void Init();
    void Reset(bool bGameGear, bool bPAL);
    bool Tick(unsigned int &clockCycles, GS_Color* pColorFrameBuffer);
    u8 GetVCounter();
    u8 GetHCounter();
    u8 GetDataPort();
    u8 GetStatusFlags();
    void WriteData(u8 data);
    void WriteControl(u8 control);

private:
    void ScanLine(int line);
    void RenderBG(int line);
    void RenderSprites(int line);
    GS_Color ConvertTo8BitColor(int palette_color);

private:
    Memory* m_pMemory;
    Processor* m_pProcessor;
    u8* m_pInfoBuffer;
    GS_Color* m_pColorFrameBuffer;
    u8* m_pVdpVRAM;
    u8* m_pVdpCRAM;
    bool m_bFirstByteInSequence;
    u8 m_VdpLatch;
    u8 m_VdpRegister[16];
    u8 m_VdpCode;
    u8 m_VdpBuffer;
    u16 m_VdpAddress;
    int m_iVCounter;
    int m_iHCounter;
    int m_iCycleCounter;
    int m_iCycleAdjustment;
    int m_iHBlankCycles;
    u8 m_VdpStatus;
    bool m_bHBlankInterrupt;
    u8 m_HBlankCounter;
    u8 m_ScrollV;
    bool m_bGameGear;
    int m_iCyclesPerLine;
    int m_iCyclesAdjustmentLine;
    int m_iCyclesPerLineLeft;
    int m_iLinesPerFrame;
    bool m_bHBlank;
};

inline GS_Color Video::ConvertTo8BitColor(int palette_color)
{
    int r, g, b, highest_value;
    if (m_bGameGear)
    {
        int palette_color_2 = palette_color << 1;
        r = m_pVdpCRAM[palette_color_2] & 0x0F;
        g = (m_pVdpCRAM[palette_color_2] >> 4) & 0x0F;
        b = (m_pVdpCRAM[(palette_color_2) + 1]) & 0x0F;
        highest_value = 15;
    }
    else
    {
        r = m_pVdpCRAM[palette_color] & 0x03;
        g = (m_pVdpCRAM[palette_color] >> 2) & 0x03;
        b = (m_pVdpCRAM[palette_color] >> 4) & 0x03;
        highest_value = 3;
    }

    GS_Color final_color;

    final_color.red = (r * 255) / highest_value;
    final_color.green = (g * 255) / highest_value;
    final_color.blue = (b * 255) / highest_value;
    final_color.alpha = 0xFF;
    return final_color;
}

const u8 kVdpHCounter[0x200] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
    0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

#endif	/* VIDEO_H */

