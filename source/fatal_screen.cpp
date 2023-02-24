/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "fatal_font.hpp"

namespace ams::fatal {

    namespace aarch64 {

        enum RegisterName {
            RegisterName_X0     =  0,
            RegisterName_X1     =  1,
            RegisterName_X2     =  2,
            RegisterName_X3     =  3,
            RegisterName_X4     =  4,
            RegisterName_X5     =  5,
            RegisterName_X6     =  6,
            RegisterName_X7     =  7,
            RegisterName_X8     =  8,
            RegisterName_X9     =  9,
            RegisterName_X10    = 10,
            RegisterName_X11    = 11,
            RegisterName_X12    = 12,
            RegisterName_X13    = 13,
            RegisterName_X14    = 14,
            RegisterName_X15    = 15,
            RegisterName_X16    = 16,
            RegisterName_X17    = 17,
            RegisterName_X18    = 18,
            RegisterName_X19    = 19,
            RegisterName_X20    = 20,
            RegisterName_X21    = 21,
            RegisterName_X22    = 22,
            RegisterName_X23    = 23,
            RegisterName_X24    = 24,
            RegisterName_X25    = 25,
            RegisterName_X26    = 26,
            RegisterName_X27    = 27,
            RegisterName_X28    = 28,
            RegisterName_FP     = 29,
            RegisterName_LR     = 30,

            RegisterName_SP     = 31,
            RegisterName_PC     = 32,

            RegisterName_GeneralPurposeCount,

            RegisterName_PState = 33,
            RegisterName_Afsr0  = 34,
            RegisterName_Afsr1  = 35,
            RegisterName_Esr    = 36,
            RegisterName_Far    = 37,

            RegisterName_Count,
        };

        struct CpuContext {
            using RegisterType = u64;
            static constexpr size_t MaxStackTraceDepth = 0x20;

            static constexpr const char *RegisterNameStrings[RegisterName_Count] = {
                "X0",
                "X1",
                "X2",
                "X3",
                "X4",
                "X5",
                "X6",
                "X7",
                "X8",
                "X9",
                "X10",
                "X11",
                "X12",
                "X13",
                "X14",
                "X15",
                "X16",
                "X17",
                "X18",
                "X19",
                "X20",
                "X21",
                "X22",
                "X23",
                "X24",
                "X25",
                "X26",
                "X27",
                "X28",
                "FP",
                "LR",
                "SP",
                "PC",
                "PState",
                "Afsr0",
                "Afsr1",
                "Esr",
                "Far",
            };

            /* Registers, exception context. N left names for these fields in fatal .rodata. */
            union {
                struct {
                    union {
                        RegisterType x[RegisterName_GeneralPurposeCount];
                        struct {
                            RegisterType _x[RegisterName_FP];
                            RegisterType fp;
                            RegisterType lr;
                            RegisterType sp;
                            RegisterType pc;
                        };
                    };
                    RegisterType pstate;
                    RegisterType afsr0;
                    RegisterType afsr1;
                    RegisterType esr;
                    RegisterType far;
                };
                RegisterType registers[RegisterName_Count];
            };

            /* Misc. */
            RegisterType stack_trace[MaxStackTraceDepth];
            RegisterType base_address;
            RegisterType register_set_flags;
            u32 stack_trace_size;

            void ClearState() {
                std::memset(this, 0, sizeof(*this));
            }

            void SetProgramIdForAtmosphere(ncm::ProgramId program_id) {
                /* Right now, we mux program ID in through afsr when creport. */
                /* TODO: Better way to do this? */
                this->afsr0 = static_cast<RegisterType>(program_id.value);
            }

            ncm::ProgramId GetProgramIdForAtmosphere() const {
                return ncm::ProgramId{this->afsr0};
            }

            void SetRegisterValue(RegisterName name, RegisterType value) {
                this->registers[name] = value;
                this->register_set_flags |= (RegisterType(1) << name);
            }

            bool HasRegisterValue(RegisterName name) const {
                return this->register_set_flags & (RegisterType(1) << name);
            }

            void SetBaseAddress(RegisterType base_addr) {
                this->base_address = base_addr;
            }
        };

    }

    namespace aarch32 {

        enum RegisterName {
            RegisterName_R0     =  0,
            RegisterName_R1     =  1,
            RegisterName_R2     =  2,
            RegisterName_R3     =  3,
            RegisterName_R4     =  4,
            RegisterName_R5     =  5,
            RegisterName_R6     =  6,
            RegisterName_R7     =  7,
            RegisterName_R8     =  8,
            RegisterName_R9     =  9,
            RegisterName_R10    = 10,
            RegisterName_FP     = 11,
            RegisterName_IP     = 12,
            RegisterName_LR     = 13,
            RegisterName_SP     = 14,
            RegisterName_PC     = 15,

            RegisterName_GeneralPurposeCount,

            RegisterName_PState = 16,
            RegisterName_Afsr0  = 17,
            RegisterName_Afsr1  = 18,
            RegisterName_Esr    = 29,
            RegisterName_Far    = 20,

            RegisterName_Count,
        };

        struct CpuContext {
            using RegisterType = u32;
            static constexpr size_t MaxStackTraceDepth = 0x20;

            static constexpr const char *RegisterNameStrings[RegisterName_Count] = {
                "R0",
                "R1",
                "R2",
                "R3",
                "R4",
                "R5",
                "R6",
                "R7",
                "R8",
                "R9",
                "R10",
                "FP",
                "IP",
                "LR",
                "SP",
                "PC",
                "PState",
                "Afsr0",
                "Afsr1",
                "Esr",
                "Far",
            };

            /* Registers, exception context. N left names for these fields in fatal .rodata. */
            union {
                struct {
                    union {
                        RegisterType r[RegisterName_GeneralPurposeCount];
                        struct {
                            RegisterType _x[RegisterName_FP];
                            RegisterType fp;
                            RegisterType ip;
                            RegisterType lr;
                            RegisterType sp;
                            RegisterType pc;
                        };
                    };
                    RegisterType pstate;
                    RegisterType afsr0;
                    RegisterType afsr1;
                    RegisterType esr;
                    RegisterType far;
                };
                RegisterType registers[RegisterName_Count];
            };

            /* Misc. Yes, stack_trace_size is really laid out differently than aarch64... */
            RegisterType stack_trace[MaxStackTraceDepth];
            u32 stack_trace_size;
            RegisterType base_address;
            RegisterType register_set_flags;

            void ClearState() {
                std::memset(this, 0, sizeof(*this));
            }

            void SetProgramIdForAtmosphere(ncm::ProgramId program_id) {
                /* Right now, we mux program ID in through afsr when creport. */
                /* TODO: Better way to do this? */
                this->afsr0 = static_cast<RegisterType>(static_cast<u64>(program_id.value) >> 0);
                this->afsr1 = static_cast<RegisterType>(static_cast<u64>(program_id.value) >> 32);
            }

            ncm::ProgramId GetProgramIdForAtmosphere() const {
                return ncm::ProgramId{(static_cast<u64>(this->afsr1) << 32ul) | (static_cast<u64>(this->afsr0) << 0ul)};
            }

            void SetRegisterValue(RegisterName name, RegisterType value) {
                this->registers[name] = value;
                this->register_set_flags |= (RegisterType(1) << name);
            }

            bool HasRegisterValue(RegisterName name) const {
                return this->register_set_flags & (RegisterType(1) << name);
            }

            void SetBaseAddress(RegisterType base_addr) {
                this->base_address = base_addr;
            }
        };

    }

}

namespace ams::fatal::srv {

    namespace {

        #include "fatal_ams_logo.inc"

    }

    namespace {

        /* Screen definitions. */
        constexpr u32 FatalScreenWidth = 1280;
        constexpr u32 FatalScreenHeight = 720;
        constexpr u32 FatalScreenBpp = 2;
        constexpr u32 FatalLayerZ = 100;

        constexpr u32 FatalScreenWidthAlignedBytes = (FatalScreenWidth * FatalScreenBpp + 63) & ~63;
        constexpr u32 FatalScreenWidthAligned = FatalScreenWidthAlignedBytes / FatalScreenBpp;

        constexpr u32 GetPixelOffset(u32 x, u32 y) {
            return y * FatalScreenWidthAligned + x;
        }

    }

    void *RenderFatal(bool is_aarch32) {
        void *buffer = std::malloc(FatalScreenWidthAlignedBytes * FatalScreenHeight);
        std::memset(buffer, 0, FatalScreenWidthAlignedBytes * FatalScreenHeight);
        u16 *frame_buffer = static_cast<u16 *>(buffer);

        /* Let the font manager know about our framebuffer. */
        font::ConfigureFontFramebuffer(frame_buffer, GetPixelOffset);
        font::SetFontColor(0xFFFF);

        /* Draw a background. */
        for (size_t i = 0; i < FatalScreenWidthAligned * FatalScreenHeight; i++) {
            frame_buffer[i] = 0x39C9;
        }

        /* Draw the atmosphere logo in the upper right corner. */
        const u32 start_x = 32, start_y = 64;
        for (size_t y = 0; y < AtmosphereLogoHeight; y++) {
            for (size_t x = 0; x < AtmosphereLogoWidth; x++) {
                frame_buffer[GetPixelOffset(FatalScreenWidth - AtmosphereLogoWidth - start_x + x, start_x + y)] = AtmosphereLogoData[y * AtmosphereLogoWidth + x];
            }
        }
        printf("0\n");

        /* Draw error message and firmware. */
        font::SetPosition(start_x, start_y);
        font::SetFontSize(16.0f);
        font::PrintFormat((const char *)u8"Error Code: 2%03d-%04d (0x%x)\n", 2, 2, 0x202);
        font::AddSpacingLines(0.5f);
        font::PrintFormatLine(  "Program:  %016llX", 0xCCCCCCCCCCCCCCCCull);
        font::AddSpacingLines(0.5f);

        font::PrintFormatLine((const char *)u8"Firmware: %s (Atmosphere %u.%u.%u-%s)", "16.0.0", ATMOSPHERE_RELEASE_VERSION, ams::GetGitRevision());
        font::AddSpacingLines(1.5f);
        font::Print((const char *)u8"An error has occured.\n\n"
                                 u8"Please press the POWER Button to restart the console normally, or a VOL button\n"
                                 u8"to reboot to a payload (or RCM, if none is present). If you are unable to\n"
                                 u8"restart the console, hold the POWER Button for 12 seconds to turn the console off.\n\n"
                                 u8"If the problem persists, refer to the Nintendo Support Website.\n"
                                 u8"support.nintendo.com/switch/error\n");

        /* Add a line. */
        for (size_t x = start_x; x < FatalScreenWidth - start_x; x++) {
            frame_buffer[GetPixelOffset(x, font::GetY())] = 0xFFFF;
        }

        font::AddSpacingLines(1.5f);

        u32 backtrace_y = font::GetY();
        u32 backtrace_x = 0;
        u32 pc_x = 0;

        /* Print GPRs. */
        font::SetFontSize(14.0f);
        font::Print("General Purpose Registers      ");
        font::PrintLine("");
        font::SetPosition(start_x, font::GetY());
        font::AddSpacingLines(0.5f);
        if (is_aarch32) {
            for (size_t i = 0; i < (aarch32::RegisterName_GeneralPurposeCount / 2); i++) {
                u32 x = font::GetX();
                font::PrintFormat("%s:", aarch32::CpuContext::RegisterNameStrings[i]);
                font::SetPosition(x + 47, font::GetY());
                if (true) {
                    font::PrintMonospaceU32(i * 0x01010101u);
                    font::PrintMonospaceBlank(8);
                } else {
                    font::PrintMonospaceBlank(16);
                }
                font::Print("  ");
                pc_x = font::GetX();
                font::PrintFormat("%s:", aarch32::CpuContext::RegisterNameStrings[i + (aarch32::RegisterName_GeneralPurposeCount / 2)]);
                font::SetPosition(pc_x + 47, font::GetY());
                if (true) {
                    font::PrintMonospaceU32((i + (aarch32::RegisterName_GeneralPurposeCount / 2)) * 0x01010101u);
                    font::PrintMonospaceBlank(8);
                } else {
                    font::PrintMonospaceBlank(16);
                }

                if (i == (aarch32::RegisterName_GeneralPurposeCount / 2) - 1) {
                    font::Print("    ");
                    backtrace_x = font::GetX();
                }

                font::PrintLine("");
                font::SetPosition(start_x, font::GetY());
            }
        } else {
            for (size_t i = 0; i < aarch64::RegisterName_GeneralPurposeCount / 2; i++) {
                u32 x = font::GetX();
                font::PrintFormat("%s:", aarch64::CpuContext::RegisterNameStrings[i]);
                font::SetPosition(x + 47, font::GetY());
                if (true) {
                    font::PrintMonospaceU64(i * 0x0101010101010101ull);
                } else {
                    font::PrintMonospaceBlank(16);
                }
                font::Print("  ");
                pc_x = font::GetX();
                font::PrintFormat("%s:", aarch64::CpuContext::RegisterNameStrings[i + (aarch64::RegisterName_GeneralPurposeCount / 2)]);
                font::SetPosition(pc_x + 47, font::GetY());
                if (true) {
                    font::PrintMonospaceU64((i + (aarch64::RegisterName_GeneralPurposeCount / 2)) * 0x0101010101010101ull);
                } else {
                    font::PrintMonospaceBlank(16);
                }

                if (i == (aarch64::RegisterName_GeneralPurposeCount / 2) - 1) {
                    font::Print("    ");
                    backtrace_x = font::GetX();
                }

                font::PrintLine("");
                font::SetPosition(start_x, font::GetY());
            }
        }

        /* Print PC. */
        {
            font::SetPosition(pc_x, backtrace_y);
            const u32 x = font::GetX();
            font::Print("PC: ");
            font::SetPosition(x + 47, font::GetY());
        }
        if (is_aarch32) {
            font::PrintMonospaceU32(0xAAAAAAAAu);
        } else {
            font::PrintMonospaceU64(0xAAAAAAAAAAAAAAAAull);
        }

        /* Print Backtrace. */
        u32 bt_size = is_aarch32 ? aarch32::CpuContext::MaxStackTraceDepth : aarch64::CpuContext::MaxStackTraceDepth;

        font::SetPosition(backtrace_x, backtrace_y);
        if (bt_size == 0) {
            if (is_aarch32) {
                font::Print("Start Address: ");
                font::PrintMonospaceU32(0xFFFFF000u);
                font::PrintLine("");
            } else {
                font::Print("Start Address: ");
                font::PrintMonospaceU64(0xFFFFFFFFFFFFF000ull);
                font::PrintLine("");
            }
        } else {
            if (is_aarch32) {
                font::Print("Backtrace - Start Address: ");
                font::PrintMonospaceU32(0xFFFFF000u);
                font::PrintLine("");
                font::AddSpacingLines(0.5f);
                for (u32 i = 0; i < aarch32::CpuContext::MaxStackTraceDepth / 2; i++) {
                    u32 bt_cur = 0, bt_next = 0;
                    if (i < bt_size) {
                        bt_cur = 0xFFFFF000u + 0x10 * i;
                    }
                    if (i + aarch32::CpuContext::MaxStackTraceDepth / 2 < bt_size) {
                        bt_next = 0xFFFFF000u + 0x10 * (i + aarch32::CpuContext::MaxStackTraceDepth / 2);
                    }

                    if (i < bt_size) {
                        u32 x = font::GetX();
                        font::PrintFormat("BT[%02d]: ", i);
                        font::SetPosition(x + 72, font::GetY());
                        font::PrintMonospaceU32(bt_cur);
                        font::PrintMonospaceBlank(8);
                        font::Print("  ");
                    }

                    if (i + aarch32::CpuContext::MaxStackTraceDepth / 2 < bt_size) {
                        u32 x = font::GetX();
                        font::PrintFormat("BT[%02d]: ", i + aarch32::CpuContext::MaxStackTraceDepth / 2);
                        font::SetPosition(x + 72, font::GetY());
                        font::PrintMonospaceU32(bt_next);
                        font::PrintMonospaceBlank(8);
                    }

                    font::PrintLine("");
                    font::SetPosition(backtrace_x, font::GetY());
                }
            } else {
                font::Print("Backtrace - Start Address: ");
                font::PrintMonospaceU64(0xFFFFFFFFFFFFF000ull);
                font::PrintLine("");
                font::AddSpacingLines(0.5f);
                for (u32 i = 0; i < aarch64::CpuContext::MaxStackTraceDepth / 2; i++) {
                    u64 bt_cur = 0, bt_next = 0;
                    if (i < bt_size) {
                        bt_cur = 0xFFFFFFFFFFFFF000ull + 0x10 * i;
                    }
                    if (i + aarch64::CpuContext::MaxStackTraceDepth / 2 < bt_size) {
                        bt_next = 0xFFFFFFFFFFFFF000ull + 0x10 * (i + aarch64::CpuContext::MaxStackTraceDepth / 2);
                    }

                    if (i < bt_size) {
                        u32 x = font::GetX();
                        font::PrintFormat("BT[%02d]: ", i);
                        font::SetPosition(x + 72, font::GetY());
                        font::PrintMonospaceU64(bt_cur);
                        font::Print("  ");
                    }

                    if (i + aarch64::CpuContext::MaxStackTraceDepth / 2 < bt_size) {
                        u32 x = font::GetX();
                        font::PrintFormat("BT[%02d]: ", i + aarch64::CpuContext::MaxStackTraceDepth / 2);
                        font::SetPosition(x + 72, font::GetY());
                        font::PrintMonospaceU64(bt_next);
                    }

                    font::PrintLine("");
                    font::SetPosition(backtrace_x, font::GetY());
                }
            }
        }

        return buffer;
    }

}
