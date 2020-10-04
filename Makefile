bccsh: bccsh.c bccsh.h parser.o util.h doc/bccsh.pdf gerador_de_processos/gerador.c
	gcc bccsh.c parser.o -o bccsh -I `pkg-config --cflags --libs readline`
	gcc -pthread ep1.c -o ep1 
	gcc gerador_de_processos/gerador.c -o gerador_de_processos/gerador
	./gerador_de_processos/gerador 5
	./gerador_de_processos/gerador 50
	./gerador_de_processos/gerador 500
	rm *.o

doc/bccsh.pdf: doc/bccsh.tex
	pdflatex -output-directory=./doc doc/bccsh.tex
	rm ./doc/*.aux ./doc/*.log

parser.o: parser.c parser.h util.h
	gcc parser.c -c

