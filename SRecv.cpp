#include <fstream>
#include <iomanip>
#include <ios>
#include <cmath>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "echelonheaders.h"

int serverRecv(bool continuous) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  int filenameSize;
  recv(clientSocket, &filenameSize, sizeof(int), 0);

  char filename[256];
  recv(clientSocket, filename, filenameSize, 0);
  filename[filenameSize] = '\0';

  std::streampos fileSize;
  recv(clientSocket, &fileSize, sizeof(int), 0);
  
  std::ofstream file(filename, std::ios::binary);

  char buffer[BUFFER_SIZE] = {0};
  int bytes_recieved;
  float MB = 0;

  std::cout << "Recieving file: " << filename << std::endl;

  while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    file.write(buffer, bytes_recieved);
    MB += (float)bytes_recieved / 1000000;

    std::cout << "\rRecieved: " << std::fixed << std::setprecision(1) << MB << " MB " << (int)(( MB / ((float)fileSize / 1000000)) * 100) << "% "<< std::flush;
  }

  std::cout << std::endl;
  file.close();
  if(continuous) {
    close(clientSocket);
    close(serverSocket);
  }
  else{
    close(serverSocket);
  }

  return 0;
}
