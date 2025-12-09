#include <fstream>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

  if(argc > 2){
    std::cout << "Usage: " << argv[0] << " <ip>" << std::endl;
    return 1;
  }

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(7777);

  struct hostent* host = gethostbyname(argv[1]);
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);

  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));


  int filenameSize;
  recv(clientSocket, &filenameSize, sizeof(int), 0);

  char filename[256];
  recv(clientSocket, filename, filenameSize, 0);
  filename[filenameSize] = '\0';

  std::ofstream file(filename, std::ios::binary);
  
  char buffer[4096];
  int bytes_recv;

  std::cout << "Recieving file: " << filename << std::endl;
  while((bytes_recv = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    file.write(buffer, bytes_recv);
    std::cout << "Recieved " << bytes_recv << " bytes\n";
  }
  file.close();
  close(clientSocket);

  return 0;
}
