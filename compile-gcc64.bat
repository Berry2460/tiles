set debug=false

if not exist release md release
if "%debug%"=="false" set window=-mwindows 
gcc -Wall -pthread src\*.c -x c -O2 -o release\main-gcc.exe %window%-L lib -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32 -lws2_32 -llibwinpthread-1
cd release
main-gcc.exe
pause
