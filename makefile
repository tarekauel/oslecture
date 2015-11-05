all:
	gcc -Wall -o Task1 Task1.c
	gcc -Wall -o Task2 Task2.c
	gcc -Wall -o Task3 Task3.c -lpthread
	gcc -Wall -o Task4 Task4.c -lrt
	gcc -Wall -o Task5 Task5.c -lrt

clean:
	-rm Task1
	-rm Task2
	-rm Task3
	-rm Task4
	-rm Task5
