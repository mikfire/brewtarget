#android build
# copied/inspired from gcompris project (http://gcompris.net/index-en.html),
# who, in turn, cite
#  copied/inspired from stellarium project (http://www.stellarium.org/wiki/index.php/Building_for_Android)

# Brewtarget has no java sources, but I have an idea I will need to specify
# the default database here.
#LIST(APPEND JAVA_SRCS "src/com/android/vending/billing/IInAppBillingService.aidl" "src/net/gcompris/GComprisActivity.java")

#FOREACH(JavaSource ${JAVA_SRCS})
#  ADD_CUSTOM_COMMAND(
#    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${JavaSource}
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${JavaSource} ${CMAKE_CURRENT_BINARY_DIR}/${JavaSource}
#    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${JavaSource}
#    )
#  LIST(APPEND TARGET_JAVA_FILES ${CMAKE_CURRENT_BINARY_DIR}/${JavaSource})
#ENDFOREACH()

#ADD_CUSTOM_TARGET (java_src DEPENDS ${TARGET_JAVA_FILES})
MESSAGE( STATUS "Parsing android/cmakelists.txt")
SET( DOCSRC "${ROOTDIR}/doc" )
SET( brewtarget_ASSETDIR "${CMAKE_BINARY_DIR}/android/assets" )
SET( brewtarget_DATADIR "${brewtarget_ASSETDIR}/data" )
SET( brewtarget_DOCDIR "${brewtarget_ASSETDIR}/docs" )
SET( brewtarget_RES_DB "${brewtarget_DATADIR}/default_db.sqlite" )

SET( TARGET "${CMAKE_BINARY_DIR}/lib/x86/libbrewtarget.so" )

ADD_CUSTOM_COMMAND(
   OUTPUT ${brewtarget_RES_DB}
   COMMAND cmake -E make_directory ${brewtarget_DATADIR}
   COMMAND cmake -E copy ${DATADIR}/default_db.sqlite ${brewtarget_RES_DB}
)

ADD_CUSTOM_COMMAND(
   OUTPUT "${brewtarget_DOCDIR}/brewtarget-manual.html"
   COMMAND cmake -E make_directory ${brewtarget_DOCDIR}
   COMMAND cmake -E copy "${DOCSRC}/brewtarget-manual.html" ${brewtarget_DOCDIR}
   COMMAND cmake -E make_directory "${brewtarget_DOCDIR}/images"
   COMMAND cmake -E copy_directory "${DOCSRC}/images" "${brewtarget_DOCDIR}/images"
)

# All the assets we need go here.
ADD_CUSTOM_TARGET(
   brewtarget_assets
   DEPENDS
   ${brewtarget_RES_DB}
   "${brewtarget_DOCDIR}/brewtarget-manual.html"
)

find_program(ANDROID_DEPLOY_QT NAMES androiddeployqt PATHS ${_qt5Core_install_prefix}/bin/ )

# Look for androiddeployqt program
if (NOT ANDROID_DEPLOY_QT)
  message(FATAL_ERROR "Could not find androiddeployqt. Make sure you set all the Qt5_* paths to the Qt for android compiled version.")
endif(NOT ANDROID_DEPLOY_QT)

# Set version
set(ANDROID_VERSION_NAME ${brewtarget_VERSION})

# set android package source for androiddeployqt json file
set(PACKAGE_SOURCE_ANDROID ${CMAKE_BINARY_DIR}/tmpAndroid)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml.cmake ${PACKAGE_SOURCE_ANDROID}/AndroidManifest.xml @ONLY)

# create json file parsed by the androiddeployqt
set(ANDROID_SDK $ENV{ANDROID_SDK})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configAndroid.json.cmake ${PACKAGE_SOURCE_ANDROID}/configAndroid.json @ONLY)

# Get all po files in po/ to create the values-{locale} in order to have the locales available in android
# eep. I hope this works -- maf
file(GLOB TRANSLATIONS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../po/*.po")
foreach(PoSource ${TRANSLATIONS_FILES})
  # Remove suffix ".po"
  string(REPLACE ".po" "" PoSource ${PoSource})
  # Remove prefix "../po/bt_"
   string(REPLACE "../po/bt_" "" locale ${PoSource})
  # replace _ par -r for locales with country
  string(REPLACE "_" "-r" locale ${locale})
  
  add_custom_command(
    OUTPUT ${locale}
    COMMAND cmake -E make_directory ${TRANSLATIONSDIR}
    COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/android/res/values-${locale}
    COMMAND cmake -E copy ${CMAKE_SOURCE_DIR}/android/strings.xml ${CMAKE_BINARY_DIR}/android/res/values-${locale}/strings.xml
    )
  list(APPEND ALL_LOCALES ${locale})
endforeach()


add_custom_command (
  OUTPUT createApkFromAndroidDeployQtDebug
  DEPENDS ${PACKAGE_SOURCE_ANDROID}/AndroidManifest.xml brewtarget_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${PACKAGE_SOURCE_ANDROID}
  COMMAND ${ANDROID_DEPLOY_QT} --output ${CMAKE_CURRENT_BINARY_DIR}/ --input ${PACKAGE_SOURCE_ANDROID}/configAndroid.json --debug 
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/bin/QtApp-debug.apk ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-debug-${ANDROID_ABI}-${brewtarget_VERSION_STRING}.apk
)

add_custom_command (
  OUTPUT createApkFromAndroidDeployQtRelease
  DEPENDS ${PACKAGE_SOURCE_ANDROID}/AndroidManifest.xml
  Message( FATAL_ERROR "We cannot build a release APK yet")
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${PACKAGE_SOURCE_ANDROID}
  COMMAND ${ANDROID_DEPLOY_QT} --output ${CMAKE_CURRENT_BINARY_DIR}/ --input ${PACKAGE_SOURCE_ANDROID}/configAndroid.json --release
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/bin/QtApp-release-unsigned.apk ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-${ANDROID_ABI}-${brewtarget_VERSION}.apk
)

add_custom_command (
  OUTPUT createApkSignedFromAndroidDeployQtRelease
  Message( FATAL_ERROR "We cannot build a release APK yet")
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-${ANDROID_ABI}-${brewtarget_VERSION}.apk
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-${ANDROID_ABI}-${brewtarget_VERSION}.apk ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-${ANDROID_ABI}-${brewtarget_VERSION}.apk
  COMMAND jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore $ENV{ANDROID_KEYSTORE} -storepass $ENV{ANDROID_KEYSTORE_PASSWD} ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-${ANDROID_ABI}-${brewtarget_VERSION}.apk brewtarget
)

add_custom_command (
  OUTPUT createApkSignedAlignedFromAndroidDeployQtRelease
  Message( FATAL_ERROR "We cannot build a release APK yet")
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-${ANDROID_ABI}-${brewtarget_VERSION}.apk
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-aligned-${ANDROID_ABI}-${brewtarget_VERSION}.apk
  COMMAND ${ANDROID_SDK}/build-tools/21.1.2/zipalign -v 4 ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-${ANDROID_ABI}-${brewtarget_VERSION}.apk ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-aligned-${ANDROID_ABI}-${brewtarget_VERSION}.apk
)

add_custom_command (
  OUTPUT apkInstall
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-aligned-${ANDROID_ABI}-${brewtarget_VERSION}.apk
  COMMAND ${ANDROID_SDK}/platform-tools/adb install -r ${CMAKE_CURRENT_BINARY_DIR}/bin/brewtarget-Android-release-signed-aligned-${ANDROID_ABI}-${brewtarget_VERSION}.apk
)

# Command to create apk from Makefile
add_custom_target (apk_debug
   DEPENDS ${ALL_LOCALES} createApkFromAndroidDeployQtDebug #java_src
)

# Command to create apk from Makefile
add_custom_target (apk_release
   DEPENDS ${ALL_LOCALES} createApkFromAndroidDeployQtRelease #java_src
)

# Command to create signed apk from Makefile
add_custom_target (apk_signed
   DEPENDS ${ALL_LOCALES} createApkSignedFromAndroidDeployQtRelease #java_src
)

# Command to create signed aligned apk from Makefile
add_custom_target (apk_signed_aligned
   DEPENDS ${ALL_LOCALES} createApkSignedAlignedFromAndroidDeployQtRelease #java_src
)

# Command to install the signed aligned apk through adb from Makefile
add_custom_target (apk_install
   DEPENDS apkInstall
)
