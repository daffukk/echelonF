#include <fstream>
#include <iomanip>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "echelonheaders.h"

int clientRecv(int argc, char* argv[], bool continuous, double speed, const char* passkey) {

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  struct hostent* host = gethostbyname(argv[3]);
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);
 

  while(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) >= 0) {
    
    if(passkey != nullptr && strlen(passkey) > 0) {
      int passkeyLength = strlen(passkey);
      send(clientSocket, &passkeyLength, sizeof(int), 0);

      send(clientSocket, passkey, passkeyLength, 0);
    }

    int filenameSize;
    recv(clientSocket, &filenameSize, sizeof(int), 0);

    char filename[256];
    recv(clientSocket, filename, filenameSize, 0);
    filename[filenameSize] = '\0';
  
    std::streampos fileSize;
    recv(clientSocket, &fileSize, sizeof(int), 0);
  
    std::ofstream file(filename, std::ios::binary);
    
    char buffer[BUFFER_SIZE];
    int bytes_recv;
    int sleepDuration;
    double MB = 0;
    double fileSizeMB = (double)fileSize / 1000000;


    std::cout << "Recieving file: " << filename << std::endl;
    if(speed > 0) {
      sleepDuration = calculateSpeed(speed);
    }

    while((bytes_recv = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      updateReceiveProgress(file, buffer, bytes_recv, MB, fileSizeMB);

      if (speed > 0){
        std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
      }
    }
    std::cout << std::endl;
    file.close();
  }


  close(clientSocket);
  return 0;
}
