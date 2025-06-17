# DTPP
A simple and opensource project for modifying your dictpen

## Note
您使用DTPP造成的一切后果均由本人承担，本项目开发者不承担任何设备损坏责任

All consequences resulting from your use of DTPP shall be borne by yourself. The project developer assumes no responsibility for your device.

## Features
 - Add a Deepseek-R1 chat for your YDP032 DictPen
 - Show your battery current
 - Switch system schedule to `performance`
 - Bring OCR result into Inputmethod
 - Opensource for study

## Build && Install
### Archlinux
- TODO:This instructions need to be fixed!!!
- You need to check whether you have access to root on your device
- You need to get your qt resources [qrc extract tool](https://github.com/pgaskin/qrc)
- DTPP now only support YDP03x Device
- Your system version must be from 2.7.0 to 2.8.0!!!
Add your API KEY [llmchat.cpp](src/llmchat.cpp)
```bash
# sudo pacman -S zig qt5-base cmake xmake base-devel git
# git clone https://github.com/DTPP0x19/DTPP
# cd DTPP
# sh build.sh
# adb shell mkdir /userdisk/dtpp
# adb push build/cross/arm64/release/libdtppl.so /userdisk/dtpp/
# adb push build/cross/arm64/release/libdtpp.so /userdisk/dtpp/
# adb push lib/Dobby
...
#Add "LD_PRELOAD=/userdisk/dtpp/libdtppl.so" To your runDictPen file
```


## Credits
[LSPosed's fork of Dobby](https://github.com/LSPosed/Dobby/)
[pymumu/tinylog](https://github.com/pymumu/tinylog)
[nlohmann/json](https://github.com/nlohmann/json)
[kubo/injector](https://github.com/kubo/injector)
[atisawd/boxicons](https://github.com/atisawd/boxicons)
[pgaskin/qrc](https://github.com/pgaskin/qrc)
[mrexodia/zig-cross](https://github.com/mrexodia/zig-cross)

## License
files in `src` and `loader` is under AGPLv3 license

files in `lib` and `include` is under each library's own license

Icons are under MIT license