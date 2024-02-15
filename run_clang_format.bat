@echo off
setlocal
set CLANG_FORMAT_EXE="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin\clang-format.exe"
set BATCHPATH=%~dp0

pushd %BATCHPATH%\Code

FOR /f %%f IN ('dir /s/b *.h') DO (
	echo Formatting file %%f
	%CLANG_FORMAT_EXE% -style=file -i %%f
)

FOR /f %%f IN ('dir /s/b *.cpp') DO (
	echo Formatting file %%f
	%CLANG_FORMAT_EXE% -style=file -i %%f
)

popd
pause