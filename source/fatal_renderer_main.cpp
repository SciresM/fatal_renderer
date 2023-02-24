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
#include "fatal_screen.hpp"
#include "fatal_font.hpp"

namespace ams {

    namespace {

        /* Screen definitions. */
        constexpr u32 FatalScreenWidth = 1280;
        constexpr u32 FatalScreenHeight = 720;
        constexpr u32 FatalScreenBpp = 2;

        constexpr u32 FatalScreenWidthAlignedBytes = util::AlignUp(FatalScreenWidth * FatalScreenBpp, 64);
        constexpr u32 FatalScreenWidthAligned = FatalScreenWidthAlignedBytes / FatalScreenBpp;

        Result SaveData(const char *fn, const void *data, size_t size) {
            fs::CreateFile(fn, size);

            fs::FileHandle file;
            R_TRY(fs::OpenFile(std::addressof(file), fn, fs::OpenMode_ReadWrite));
            ON_SCOPE_EXIT { fs::CloseFile(file); };

            R_TRY(fs::SetFileSize(file, size));

            R_RETURN(fs::WriteFile(file, 0, data, size, fs::WriteOption::Flush));
        }

    }

    void Main() {
        /* Disable auto-abort when performing file operations. */
        fs::SetEnabledAutoAbort(false);

        const auto argc = os::GetHostArgc();
        const auto argv = os::GetHostArgv();

        if (argc != 1) {
            printf("Usage: %s", argv[0]);
            return;
        }

        printf("Setting up font\n");

        if (const Result res = fatal::srv::font::InitializeSharedFont(); R_FAILED(res)) {
            fprintf(stderr, "Failed to initialize shared font: 2%03d-%04d\n", res.GetModule(), res.GetDescription());
            return;
        }

        printf("Making paths\n");
        const char *path64 = nullptr;
        const char *path32 = nullptr;
        AMS_ABORT_UNLESS(fatal::srv::font::CreateFilePath(std::addressof(path64), "aarch64.bin"));
        AMS_ABORT_UNLESS(fatal::srv::font::CreateFilePath(std::addressof(path32), "aarch32.bin"));
        ON_SCOPE_EXIT { std::free(const_cast<char *>(path64)); std::free(const_cast<char *>(path32)); };
        printf("Made paths\n");

        SaveData(path64, fatal::srv::RenderFatal(false), FatalScreenWidthAlignedBytes * FatalScreenHeight);
        printf("Saved aarch64 to aarch64.bin\n");
        SaveData(path32, fatal::srv::RenderFatal(true), FatalScreenWidthAlignedBytes * FatalScreenHeight);
        printf("Saved aarch32 to aarch32.bin\n");

        printf("Done!\n");
    }

}