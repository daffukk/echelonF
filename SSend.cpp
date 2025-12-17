#include "echelonheaders.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int serverSend(int argc, char* argv[]) {

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  const char* filename = argv[2];

  std::ifstream file(filename, std::ios::binary);

  if(!file.is_open()) {
    std::cout << "error: can't open file " << filename << std::endl;
    return 1;
  }
  
  int filenameSize = strlen(filename);
  send(clientSocket, &filenameSize, sizeof(int), 0);
  send(clientSocket, filename, filenameSize, 0);

  std::streampos fileSize = file.tellg();
  file.seekg(0, std::ios::end);
  fileSize = file.tellg() - fileSize;
  file.seekg(0, std::ios::beg);

  send(clientSocket, &fileSize, sizeof(int), 0);

  char buffer[BUFFER_SIZE];
  int bytes_read;
  float MB = 0;

  while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
    send(clientSocket, buffer, bytes_read, 0);
    MB += (float)bytes_read / 1000000;

    std::cout << "\rSent: " << std::fixed << std::setprecision(1) << MB << " MB " << (int)(( MB / ((float)fileSize / 1000000)) * 100) << "% "<< std::flush;

  }
  file.close();
  close(clientSocket);
  close(serverSocket);

  return 0;

}
