
EXEC= xtsi

.PHONY: all clean mrproper

all: $(EXEC)
	

xtsi: xtsi.o
	gcc -g -Wall -Iinclude xtsi.o -o xtsi
xtsi.o: xtsi.c
	gcc -g -Wall -Iinclude -c xtsi.c -o xtsi.o



clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)







