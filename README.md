**Tiny Tablet Driver** – minimal user space driver for Wacom CTL-672 drawing tablet written in C99 
using only Windows API. Based on [OpenTabletDriver][otd].

Status:
- find, open and read raw data from HID device with VID=1386 & PID=891 (aka CTL-672)
- switch tablet from "generic mouse" mode into raw mode (but not restore it)
- parse tablet report

Build requirements:
- [Visual Studio Build Tools][vstools] (MSVC, Windows SDK)
- [64-Bit MSVC toolset][64tools] command prompt being active

Build and run:
```bat
project build run
```

Package for distribution:
```bat
project dist
```

Delete intermediate files:
```bat
project clean
```

Check dependencies:
```bat
dumpbin /imports out\TinyTabletDriver.exe
```

[otd]: https://github.com/OpenTabletDriver/OpenTabletDriver
[vstools]: https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2026
[64tools]: https://learn.microsoft.com/en-us/cpp/build/how-to-enable-a-64-bit-visual-cpp-toolset-on-the-command-line?view=msvc-170
