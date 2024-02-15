@echo off
setlocal

set BUILD_DIR="%~dp0Build"
set VSWHERE="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
set GETVSVERSION=%VSWHERE% -latest -format value -property catalog_productLineVersion

for /f %%i in ('%GETVSVERSION%') do set VSVERSION=%%i
echo The latest VS version found is %VSVERSION%

IF %ERRORLEVEL% NEQ 0 (
  echo "vswhere.exe not found, unable to locate visual studio, please edit this script to suit your particular set"
  exit /b 1
)

IF "%VSVERSION%" == "2022" set GENERATOR="Visual Studio 17 2022"
IF "%VSVERSION%" == "2019" set GENERATOR="Visual Studio 16 2019"
IF "%VSVERSION%" == "2017" set GENERATOR="Visual Studio 15 2017"

IF "%VSVERSION%" == "" (
	echo "Version of Visual Studio (%VSVERSION%) unsupported, please edit this script to suit your particular setup"
	exit /b 1
) ELSE (
	echo Cmake Generator is %GENERATOR%
)

echo "Generating visual studio solution in Build dir"
IF not exist %BUILD_DIR% (
	mkdir %BUILD_DIR%
	cd %BUILD_DIR%
	cmake -G %GENERATOR% -A x64 ..
) ELSE (
	cd %BUILD_DIR%
	cmake ..
)

pause