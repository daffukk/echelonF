#include <netdb.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {


  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << "<filename>" << std::endl;
    return 1;
  }

  char ip[17];
  std::cout << "Enter server ip: ";
  std::cin >> ip;

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(7777);
  //serverAddress.sin_addr.s_addr = inet_addr(ip);
  
  struct hostent* host = gethostbyname(ip);
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);


  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  const char* filename = argv[1];

  std::ifstream file(filename, std::ios::binary);

  if(!file.is_open()) {
    std::cout << "error: can't open file " << filename << std::endl;
    return 1;
  }

  int filenameSize = strlen(filename);
  send(clientSocket, &filenameSize, sizeof(int), 0);
  send(clientSocket, filename, filenameSize, 0);



  char buffer[1024];
  int bytes_read;

  while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
    send(clientSocket, buffer, bytes_read, 0);
  }

  file.close();
  close(clientSocket);

  return 0;
  
}
