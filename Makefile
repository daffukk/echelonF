flags = -Wall -Wextra

all: EClient EServer
	mkdir ofiles
	mv EServer.o SSend.o SRecv.o EClient.o CRecv.o CSend.o ofiles 


EClient: CSend.o CRecv.o EClient.o
	@echo "Linking final server application"
	g++ $(flags) CSend.o CRecv.o EClient.o -o EClient
	chmod +x EClient
	@echo "Done. Do not pay attention to warnings up there."

EClient.o: EClient.cpp
	@echo "Compiling main client application"
	g++ $(flags) -c EClient.cpp -o EClient.o

CSend.o: CSend.cpp
	@echo "Compiling send client file"
	g++ $(flags) -c CSend.cpp -o CSend.o

CRecv.o: CRecv.cpp
	@echo "Compiling receive client file"
	g++ $(flags) -c CRecv.cpp -o CRecv.o

SRecv.o: SRecv.cpp
	@echo "Compiling receive server file"
	g++ $(flags) -c SRecv.cpp -o SRecv.o 

SSend.o: SSend.cpp
	@echo "Compiling send server file"
	g++ $(flags) -c SSend.cpp -o SSend.o 

EServer.o: EServer.cpp
	@echo "Compiling main server application"
	g++ $(flags) -c EServer.cpp -o EServer.o 

EServer: SSend.o SRecv.o EServer.o 
	@echo "Linking final server application"
	g++ $(flags) SSend.o SRecv.o EServer.o -o EServer
	chmod +x EServer
	@echo "Done. Do not pay attention to warnings up there."



clean:
	@echo "cleaning up all application files"
	rm -rf ofiles/ EClient EServer
