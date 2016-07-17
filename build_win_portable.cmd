@echo off

set src_path=%cd%

set src_root=%~dp0

set dst_path=%src_root%..\qImgSort

set archive_name=qImgSort-win.zip

set variant=release

cd "%src_root%"

lupdate -no-obsolete qImgSort.pro
lrelease qImgSort.pro

qmake -config %variant% qImgSort.pro
mingw32-make -j4

if exist "%dst_path%" rmdir /s /q "%dst_path%"
mkdir "%dst_path%"

xcopy /y "%variant%\qImgSort.exe" "%dst_path%\"

mkdir "%dst_path%\translations"
xcopy /y *.qm "%dst_path%\translations\"

mingw32-make distclean

cd "%dst_path%"

set deploy_flags=--libdir . --plugindir "plugins" --no-system-d3d-compiler --no-opengl-sw

windeployqt %deploy_flags% "qImgSort.exe"

cd "%dst_path%\.."
if exist "%archive_name%" del /f /q "%archive_name%"

7z a "%archive_name%" "%dst_path%"

cd "%src_path%"
