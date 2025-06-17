cd assets
rcc --project > qml.qrc
grep -v '<file>./qml.qrc</file>' qml.qrc > temp && mv temp qml.qrc
cd ..
rcc --namespace ./assets/qml.qrc > ./include/dtpp-qrc.h
echo [-]rcc binary data generated
xmake f -p cross --toolchain=zig --cross=aarch64-linux-gnu.2.27 -c
xmake build
echo [-]DTPP built successfully!
