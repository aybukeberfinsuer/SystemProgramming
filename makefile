tum: derle calistir

derle:
	gcc -I./include/ -c ./lib/libfdr/fields.c -o ./lib/fields.o
	gcc -I./include/ -I./lib/libfdr/ -o ./lib/Module.o -c ./src/Module.c
	gcc -I./include/ -o ./bin/Main ./lib/Module.o ./src/Main.c ./lib/fields.o

calistir:
	./bin/Main
