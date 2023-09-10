set debug=false

if not exist release md release
if "%debug%"=="false" set window=-mwindows 
gcc -Wall src\main.c src/network.c src\level.c src\step.c src\draw.c src\window.c src\ai.c src\missiles.c src\player.c -x c -O2 -o release\main-gcc.exe %window%-L lib\gcc64 -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32 -lws2_32
cd release
main-gcc.exe
pause
