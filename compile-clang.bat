set debug=true

if not exist release md release
if "%debug%"=="false" set window=-Wl,--subsystem,windows 
clang -Wall src/main.c src/level.c src/step.c src/draw.c src/window.c src/ai.c src/missiles.c src/player.c -o release/main-clang.exe %window%-L lib\vs2019 -lglfw3 -lglfw3dll -lopengl32 -lglu32 -lgdi32 -lmsvcrt -lvcruntime -lshell32 -luser32 -Xlinker /NODEFAULTLIB:libcmt.lib
cd release
main-clang.exe
pause
