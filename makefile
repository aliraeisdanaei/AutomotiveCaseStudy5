# compiler = gcc -lstdc++
# compiler = gpp
compiler = clang++

libraries = -lpthread

make:
	${compiler} ${libraries} -c car.cpp -o car.o
	${compiler} ${libraries} -c speaker.cpp -o speaker.o
	${compiler} ${libraries} -c main.cpp -o main.o
	${compiler} ${libraries} car.o speaker.o main.o -o final.o

run:
	./final.o

clean:
	rm *.o