![License](https://img.shields.io/badge/License-GPLv2-blue.svg)

fatal_renderer is a tool to preview atmosphere's fatal screen (to test changes to it for use in ams.fatal)

Usage
=====
```
fatal_renderer
```

To convert the raw bins, do

```
ffmpeg -f rawvideo -pixel_format rgb565 -video_size 1280x720 -i aarch64.bin aarch64.png
ffmpeg -f rawvideo -pixel_format rgb565 -video_size 1280x720 -i aarch32.bin aarch32.png
```

Licensing
=====

This software is licensed under the terms of the GPLv2, with exemptions for specific projects noted below.

You can find a copy of the license in the [LICENSE file](LICENSE).

Exemptions:
* The [yuzu Nintendo Switch emulator](https://github.com/yuzu-emu/yuzu) and the [Ryujinx Team and Contributors](https://github.com/orgs/Ryujinx) are exempt from GPLv2 licensing. They are permitted, each at their individual discretion, to instead license any source code authored for the romfs_diag project as either GPLv2 or later or the [MIT license](https://github.com/Atmosphere-NX/Atmosphere/blob/master/docs/licensing_exemptions/MIT_LICENSE). In doing so, they may alter, supplement, or entirely remove the copyright notice for each file they choose to relicense. Neither the Atmosphère project nor its individual contributors shall assert their moral rights against any of the aforementioned projects.
* [Nintendo](https://github.com/Nintendo) is exempt from GPLv2 licensing and may (at its option) instead license any source code authored for the romfs_diag project under the Zero-Clause BSD license.
