bccsh: bccsh.c bccsh.h parser.o util.h
	gcc bccsh.c parser.o -o bccsh -I `pkg-config --cflags --libs readline`
	rm *.o

parser.o: parser.c parser.h util.h
	gcc parser.c -c
