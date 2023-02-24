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
#pragma once
#include <stratosphere.hpp>

// HACK: put this elsewhere?
namespace ams::fssrv::impl {

    const char *GetWorkingDirectoryPath();

}

namespace ams::fatal::srv::font {

    // HACK: put this elsewhere?
    inline bool CreateFilePath(const char **out, const char *argument) {
        /* Allocate buffer for path. */
        const auto dst_size = std::strlen(argument) + fs::EntryNameLengthMax + 1;
        char *dst = static_cast<char *>(std::malloc(dst_size));
        ON_SCOPE_EXIT { std::free(dst); };
        if (dst == nullptr) {
            printf("Error: failed to allocate memory to hold path (%s)\n", argument);
            return false;
        }

        /* Print to path. */
        if (fs::IsPathAbsolute(argument)) {
            util::SNPrintf(dst, dst_size, "%s", argument);
        } else {
            util::SNPrintf(dst, dst_size, "%s%s", fssrv::impl::GetWorkingDirectoryPath(), argument);
        }

        /* Normalize path. */
        char *normalized = static_cast<char *>(std::malloc(dst_size));
        if (normalized == nullptr) {
            printf("Error: failed to allocate memory to hold path (%s)\n", argument);
            return false;
        }

        /* Check that the path is valid. */
        fs::PathFlags flags;
        flags.AllowWindowsPath();
        if (const auto res = fs::PathFormatter::Normalize(normalized, dst_size, dst, std::strlen(dst) + 1, flags); R_FAILED(res)) {
            printf("Error: failed to normalize path (%s): 2%03d-%04d\n", dst, res.GetModule(), res.GetDescription());
            return false;
        }

        /* Set output. */
        *out = normalized;
        return true;
    }

    Result InitializeSharedFont();
    void ConfigureFontFramebuffer(u16 *fb, u32 (*unswizzle_func)(u32, u32));
    void SetHeapMemory(void *memory, size_t memory_size);

    void SetFontColor(u16 color);
    void SetPosition(u32 x, u32 y);
    u32 GetX();
    u32 GetY();
    void SetFontSize(float fsz);
    void AddSpacingLines(float num_lines);
    void PrintLine(const char *str);
    void PrintFormatLine(const char *format, ...);
    void Print(const char *str);
    void PrintFormat(const char *format, ...);
    void PrintMonospaceU64(u64 x);
    void PrintMonospaceU32(u32 x);
    void PrintMonospaceBlank(u32 width);

}
