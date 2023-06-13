CC=em++
C_FLAGS=-Wall -O3 -s WASM=1 -s EXPORT_ALL=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0
L_FLAGS=--bind
TARGET=bin/wasm.js
FILES=objects/maze.o
WEB_FILES=web/index.html web/style.css web/main.js

${TARGET}: ${FILES} web
	${CC} -o ${TARGET} ${FILES} ${L_FLAGS}

objects/maze.o: maze.cpp ${CLIENT_FILES}
	${CC} -o objects/maze.o -c maze.cpp ${C_FLAGS}
	cp ${CLIENT_FILES} bin/