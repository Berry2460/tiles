set debug=false

if not exist release md release
if "%debug%"=="false" set window=-mwindows 
gcc -Wall -pthread src\main.c src/network.c src\step.c src\level.c src\draw.c src\window.c src\ai.c src\missiles.c src\player.c -x c -O2 -o release\main-gcc32.exe %window%-L lib\gcc32 -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32 -lws2_32
cd release
main-gcc32.exe
pause
