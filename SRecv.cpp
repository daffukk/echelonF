#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(7777);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  int filenameSize;
  recv(clientSocket, &filenameSize, sizeof(int), 0);

  char filename[256];
  recv(clientSocket, filename, filenameSize, 0);
  filename[filenameSize] = '\0';
  
  std::ofstream file(filename, std::ios::binary);

  char buffer[4096] = {0};
  int bytes_recieved;
  
  std::cout << "Recieving file: " << filename << std::endl;

  while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    file.write(buffer, bytes_recieved);
    std::cout << "Recieved " << bytes_recieved << " bytes" << std::endl;
  }
  file.close();
  close(serverSocket);

  return 0;
}
