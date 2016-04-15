# Prerequisites
The best place to go is http://doc.qt.io/qt-5/androidgs.html and follow their
instructions. This is sort of the highlight.

## Downloads and installs
Do these:
* Download and install the Android NDK
  https://developer.android.com/tools/sdk/ndk/index.html or use your OS's
  preferred method for installing software (emerge, yum, aptitude, whatever).
* Download and install the Android SDK
  https://developer.android.com/sdk/index.html and scroll down until you see
  the SDK tools only. Or use your OS native package manager.
* Make sure ant v1.8 and Java v6 or later is installed. I'm using ant v1.9.2
  and icedtea v7.2 with no issues.
* Download the Qt offline installer from
  http://www.qt.io/download-open-source/#section-2 for Android and your OS +
  bitness.  
* Run the offline installer and install somewhere that makes sense (eg: ~/Qt5.4.1). 
  I recommend only installing the android stuff (arm and atom), not the
  desktop stuff. But it's your hard drive.

##Environment
* You will need to set two environmental variables: ANDROID\_NDK and
  ANDROID\_SDK pointing to where ever you just installed the NDK and the SDK,
  respectively. setenv for csh, export for the rest.
* Set your PATH=$PATH:$ANDROID\_SDK/tools:$ANDROID\_SDK/platform-tools

## Build tools
* Run sudo android update sdk. Install some SDK tools (v 24.1.2 and v21.1.2
  recommended). You may not need to do this as root, but I did since my OS
  installed the SDK and NDK into /opt
* Install a few platforms (5.1 is recommended). Don't install the stuff
  for tv's or wearables. So far, my changes are known to work for armv7. It
  will compile for x86 Atoms, but I need to figure out how to cross compile
  the libpq.so
* Install the packages and exit the tool as root

## Emulators
* As yourself, run: android avd
* Create a couple of android machines. I recommend a nexus 10 tablet running
  5.1

## Prebuild work
* I've included two simple scripts to do some of the annoying work. If you
  want to use them, fix the DIR variable in android/build.sh. To fix it, set
  it whatever directory you installed the QT binaries into (~/Qt5.4.1 for example)
* Otherwise, study the commands yourself and figure it out. I am going to
  assume you want to use them
* mkdir android-build in the top level
* cp android/\*build.sh android-build

# Building

## Build the code
* cd android-build
* ./build.sh
* make && make apk\_debug

## Deploy
* Run: android avd &
* start the virtual device
* adb install -r android/bin/brewtarget-Android-debug-x86-2.4.0.apk

# Run
You should see brewtarget in the emulator. Run it, and be amazed at our
ugly. We've a lot of work to do.

# Cleanup
The unbuild.sh script will simply clean all of the temp files out of the
android-build directory. I wanted to start from a clean slate enough times to
make a script.
