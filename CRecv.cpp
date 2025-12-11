#include <fstream>
#include <iomanip>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cmath>
#include "echelonheaders.h"

int clientRecv(int argc, char* argv[]) {

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  struct hostent* host = gethostbyname(argv[2]);
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);

  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));


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
  float MB = 0;

  std::cout << "Recieving file: " << filename << std::endl;
  while((bytes_recv = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    file.write(buffer, bytes_recv);
    MB += (float)bytes_recv / 1000000;

    std::cout << "\rRecieved: " << std::fixed << std::setprecision(1) << MB << " MB " << (int)(( MB / ((float)fileSize / 1000000)) * 100) << "% "<< std::flush;
  }
  file.close();
  close(clientSocket);

  return 0;
}
