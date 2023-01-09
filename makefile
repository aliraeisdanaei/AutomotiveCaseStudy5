make:
	gcc -lstdc++ -c car.cpp -o car.o
	gcc -lstdc++ -c main.cpp -o main.o
	gcc -lstdc++ car.o main.o -o final.o

run:
	./final.o

clean:
	rm *.o