all: radio1 radio2 ping_pong1 ping_pong2

radio1: radio1.o rfm69.o
	g++ -g -o radio1 radio1.o rfm69.o -lwiringPi

radio1.o: radio1.cpp
	g++ -g -c radio1.cpp

radio2: radio2.o rfm69.o
	g++ -g -o radio2 radio2.o rfm69.o -lwiringPi

radio2.o: radio2.cpp
	g++ -g -c radio2.cpp

ping_pong1: ping_pong1.o rfm69.o
	g++ -g -o ping_pong1 ping_pong1.o rfm69.o -lwiringPi

ping_pong1.o: ping_pong1.cpp
	g++ -g -c ping_pong1.cpp

ping_pong2: ping_pong2.o rfm69.o
	g++ -g -o ping_pong2 ping_pong2.o rfm69.o -lwiringPi

ping_pong2.o: ping_pong2.cpp
	g++ -g -c ping_pong2.cpp

rfm69.o: rfm69.cpp
	g++ -g -c rfm69.cpp

clean:
	rm *.o
