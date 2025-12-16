**tabd** – minimal userspace driver for drawing tablets.

<img src="docs/screenshot.png" width="300" />

### Developing
Compile using MSVC with 64-bit prompt enabled (32-bit evironment is **not** supported):
```bat
rc /nologo /i src /fo icon.res res\icon.rc
cl /nologo src\tabd.c src\tablet.c src\util.c src\preset.c icon.res /link /subsystem:windows /entry:_start
```

Running in the terminal:
```bat
start /b /wait tabd.exe
```

Delete intermediate files:
```bat
del /q /s /f *.exe *.obj *.zip *.ilk *.res *.pdb *.rdi 1> nul
```

### Configuring presets

Preset settings are set in [`src/preset.c`][preset.c] file:
```c
const Preset g_presets[] = {
    { L"Drawing", { {108, 67.5},    {216, 135},        0 } },
    { L"Osu",     { {80.41049, 86}, {98, 55.098095}, -90 } },
};
```

Currently, they can not be changed at runtime.

They use the same values OpenTabletDriver does except that the first pair of values (`{108, 67.5}`)
is preset center's **XY** coords and the second pair (`{216, 135}`) is the size of the area.

Both pairs of values are specified in millimeters while rotation is in degrees.

Unlike in OTD, display area to which the tablet area is mapped can not be configured.

They first preset in the list is used by default but can be changed by right-clicking on the tray 
icon and selecting another preset under "Presets" submenu.

[preset.c]: #
