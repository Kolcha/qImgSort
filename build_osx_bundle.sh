#!/bin/bash

QT_ROOT="$HOME/Qt/5.7/clang_64"

$QT_ROOT/bin/lupdate -no-obsolete qImgSort.pro
$QT_ROOT/bin/lrelease qImgSort.pro

$QT_ROOT/bin/qmake QMAKE_MAC_SDK=macosx10.9 -config release qImgSort.pro
make -j4
[[ $? == 0 ]] || exit 1

# copy resources
mkdir qImgSort.app/Contents/Resources/translations
mv lng/*.qm qImgSort.app/Contents/Resources/translations/

# generate qt.conf
echo "[Paths]" > qImgSort.app/Contents/Resources/qt.conf
echo "Plugins = PlugIns" >> qImgSort.app/Contents/Resources/qt.conf
echo "Translations = Resources/translations" >> qImgSort.app/Contents/Resources/qt.conf

# deploy Qt for app and create .dmg file
$QT_ROOT/bin/macdeployqt qImgSort.app -dmg
rm -f ../qImgSort.dmg
mv qImgSort.dmg ../

# cleanup
make distclean
