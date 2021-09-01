set debug=false

if not exist release md release
if "%debug%"=="false" set window=-mwindows
gcc src\main.c src\step.c src\draw.c src\window.c -x c -o release\main-gcc.exe %window% -L lib\gcc64 -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32
release\main-gcc.exe
pause
