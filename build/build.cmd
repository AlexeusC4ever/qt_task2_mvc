set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%
set PATH=C:\Qt\Tools\Ninja;%PATH%
set PATH=C:\Qt\Tools\mingw810_64\bin;%PATH%

cmake^
 -G "Ninja"^
 -D CMAKE_PREFIX_PATH="C:\Qt\6.1.3\mingw81_64"^
 -D CMAKE_INSTALL_PREFIX=C:\install\qt_task2_mvc^
 -D UPDATE_TRANSLATIONS=ON^
 ..

pause	
	
cmake --build .

pause

cmake --build .
