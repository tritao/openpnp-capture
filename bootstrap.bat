@ECHO OFF
ECHO Please choose build system:
ECHO 1. Visual Studio with NMake
ECHO 2. Visual Studio with Ninja Build
ECHO 3. Visual Studio 2017 (x86)
ECHO 4. Visual Studio 2019 (x86)
ECHO 5. Exit
ECHO .

CHOICE /C 12345 /M "Enter your choice:"

IF ERRORLEVEL 5 GOTO End
IF ERRORLEVEL 4 GOTO VS_2019
IF ERRORLEVEL 3 GOTO VS_2017
IF ERRORLEVEL 2 GOTO NinjaBuild
IF ERRORLEVEL 1 GOTO VS

:VS
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ..
cd ..
GOTO End

:NinjaBuild
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" ..
cd .. 
GOTO End

:VS_2017
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 15 2017" ..
cd ..
GOTO End

:VS_2019
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2019" ..
cd ..
GOTO End

:End
