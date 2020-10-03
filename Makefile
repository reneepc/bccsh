bccsh: bccsh.c bccsh.h parser.o util.h doc/bccsh.pdf
	gcc bccsh.c parser.o -o bccsh -I `pkg-config --cflags --libs readline`
	gcc -pthread ep1.c -o ep1 
	rm *.o

doc/bccsh.pdf: doc/bccsh.tex
	pdflatex -output-directory=./doc doc/bccsh.tex
	rm ./doc/*.aux ./doc/*.log

parser.o: parser.c parser.h util.h
	gcc parser.c -c

