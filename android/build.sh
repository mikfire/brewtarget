
export DIR=/home/mik/Qt5.4.1/5.4/android_x86/lib/cmake

cmake --debug-output -DCMAKE_TOOLCHAIN_FILE=AndroidToolchain.cmake \
-DQt5_DIR=$DIR/Qt5 -DQt5Core_DIR=$DIR/Qt5Core -DQt5Multimedia_DIR=$DIR/Qt5Multimedia \
-DQt5PrintSupport_DIR=$DIR/Qt5PrintSupport -DQt5Qml_DIR=$DIR/Qt5Qml \
-DQt5Sql_DIR=$DIR/Qt5Sql -DQt5Xml_DIR=$DIR/Qt5Xml -DQt5Widgets_DIR=$DIR/Qt5Widgets \
-DQt5Network_DIR=$DIR/Qt5Network -DQt5Gui_DIR=$DIR/Qt5Gui \
../CMakeLists.txt

