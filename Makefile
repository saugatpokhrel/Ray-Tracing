all:
	g++ -I src/include -L src/lib -o main raytracing.c -lmingw32 -lSDL3
	.\main.exe
