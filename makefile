# compiler = gcc -lstdc++
# compiler = gpp
compiler = clang++

libraries = -lpthread

make:
	${compiler} ${libraries} -c configuration.cpp -o configuration.o

	${compiler} ${libraries} -c car.cpp -o car.o

	${compiler} ${libraries} -c Speaker/speaker.cpp -o speaker.o
	${compiler} ${libraries} -c Speaker/speaker_use.cpp -o speaker_use.o

	${compiler} ${libraries} -c Driver_Assist/collision_control.cpp -o collision_control.o
	${compiler} ${libraries} -c Driver_Assist/cruise_control.cpp -o cruise_control.o

	${compiler} ${libraries} -c main.cpp -o main.o
	${compiler} ${libraries} configuration.o car.o speaker.o speaker_use.o collision_control.o cruise_control.o main.o -o final.o

run:
	./final.o

clean:
	rm *.o

all:
	make && make run; make clean && clear