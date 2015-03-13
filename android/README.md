# Prerequisites
The best place to go is http://doc.qt.io/qt-5/androidgs.html and follow their
instructions. This is sort of the highlight.

## Downloads and installs
Do these:
* Download and install the Android NDK
  https://developer.android.com/tools/sdk/ndk/index.html or your OS's
  preferred method.
* Download and install the Android SDK
  https://developer.android.com/sdk/index.html and scroll down until you see
  the SDK tools only. Or use whatever your OS wants to use (emerge, yum,
  aptitude, whatever).
* Make sure any v1.8 and Java v6 or later is installed
* Download the Qt offline installer from http://www.qt.io/download-open-source/#section-2 for Android and your OS + bitness.

##Environment
* You will need export two environmental variables: ANDROID\_NDK and
  ANDROID\_SDK pointing to where ever you just installed the NDK and the SDK,
  respectively.
* Set your PATH=$PATH:$ANDROID\_SDK/tools:$ANDROID\_SDK/platform-tools

## Build tools
* Run sudo android update sdk. Install some SDK tools (v 24.1.2 and v21.1.2
  recommended). 
* Install a few platforms (5.1 and 4.4.2 recommended). Don't install the stuff
  for tv's or wearables. So far, my changes are only known to work for Intel
  x86 Atoms.
* Install the packages and exit the tool as root

## Emulators
* As yourself, run: android avd
* Create a couple of android machines. I recommend a nexus 10 tablet running
  5.1

## Prebuild work
* I've included two simple scripts to do some of the annoying work. If you
  want to use them, fix the DIR variable in android/build.sh. Otherwise, study
  the commands yourself and figure it out. I am going to assume you want to
  use them
* mkdir android-build in the top level
* cp ../android/\*build.sh android-build

# Building

## Build the code
* cd android-build
* ./build.sh
* make && make apk\_debug

## Deploy
* Run: android avd &
* start the virtual device
* adb install -r android/bin/brewtarget-Android-debug-x86-2.2.0.apk

# Run
You should see brewtarget in the emulator. Run it, and be amazed at our
ugly. We've a lot of work to do.

