prog: agent_radio2.o rfm69.o
	g++ -o prog agent_radio2.o rfm69.o -lwiringPi

agent_radio2.o: agent_radio2.cpp
	g++ -c agent_radio2.cpp

rfm69.o: rfm69.cpp
	g++ -c rfm69.cpp
