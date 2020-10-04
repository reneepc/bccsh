bccsh: bccsh.c bccsh.h parser.o util.h doc/apresentacao.pdf gerador_de_processos/gerador.c ep1.c
	gcc bccsh.c parser.o -o bccsh -I `pkg-config --cflags --libs readline`
	gcc -pthread ep1.c -o ep1 
	gcc gerador_de_processos/gerador.c -o gerador_de_processos/gerador
	./gerador_de_processos/gerador 5
	./gerador_de_processos/gerador 50
	./gerador_de_processos/gerador 500
	rm *.o

doc/apresentacao.pdf: doc/apresentacao.tex
	pdflatex -output-directory=./doc doc/apresentacao.tex
	xdg-open doc/apresentacao.pdf
	rm ./doc/*.aux ./doc/*.log ./doc/*.nav ./doc/*toc ./doc/*.snm ./doc/*.out

parser.o: parser.c parser.h util.h
	gcc parser.c -c

