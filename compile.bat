if not exist release md release
gcc src\main.c src\draw.c src\window.c -x c -o release\main.exe -L lib -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32
release\main.exe
pause
