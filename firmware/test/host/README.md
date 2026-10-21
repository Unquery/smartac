# AC-control host tests

These tests compile the production `AcControl.cpp` against the installed
IRremoteESP8266 headers. Link-time stand-ins capture library calls without
accessing GPIO or transmitting IR. They check request validation, settings
translation, initialization, and independence from previous requests.

They do not verify library encoding, IR waveforms, or physical AC behavior.

First build the firmware with PlatformIO to install its existing dependencies.
Then, from an x64 Native Tools Command Prompt for Visual Studio, in `firmware/`:

```bat
if not exist .pio\host-tests mkdir .pio\host-tests
cl /nologo /std:c++14 /EHsc /W4 /Itest\host /Iinclude /external:I.pio\libdeps\esp32-s3-devkitc-1\IRremoteESP8266\src /external:W0 src\AcControl.cpp test\host\AcControlTest.cpp /Fo.pio\host-tests\ /Fe.pio\host-tests\AcControlTest.exe
.pio\host-tests\AcControlTest.exe
```

Compile without `NDEBUG` so that assertions remain enabled. A C++11-or-newer
host compiler can also be used with the same include paths and source files.
