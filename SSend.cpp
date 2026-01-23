#include "echelonheaders.h"
#include <cstring>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int serverSend(int argc, char* argv[], double speed, const char* passkey) {

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET; // IPV4
  serverAddress.sin_port = htons(PORT); // set in the echelonheaders.h file
  serverAddress.sin_addr.s_addr = INADDR_ANY; // bind on 0.0.0.0

  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  const char* filename = argv[3]; // argv[3] is a file //im tired to repeat this fucking comments

  std::ifstream file(filename, std::ios::binary);

  if(!file.is_open()) {
    std::cerr << "error: can't open file " << filename << std::endl;
    return 1;
  }

  if(passkey != nullptr && strlen(passkey) > 0) {
    bool logged = false;
    while(logged != true) {
      int userPasskeyLength;
      recv(clientSocket, &userPasskeyLength, sizeof(int), 0);
  
      char userPasskey[256];
      recv(clientSocket, userPasskey, userPasskeyLength, 0);
      userPasskey[userPasskeyLength] = '\0';
  
      if(strcmp(userPasskey, passkey) != 0) {
        std::cout << "Wrong passkey." << std::endl;
        
        close(clientSocket);
        close(serverSocket);
        return 1;
      }
      else {
        std::cout << "Passkey accepted." << std::endl;
        logged = true;
      }
    }
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
  int sleepDuration;
  double MB = 0;
  std::string progressBar(50, ' ');
  double fileSizeMB = (double)fileSize / 1000000;

  if(speed > 0) {
    sleepDuration = calculateSpeed(speed);
  }

  while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
    updateSendProgress(clientSocket, buffer, bytes_read, MB, fileSizeMB);

    if(speed > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
    }

  }

  file.close();
  close(clientSocket);
  close(serverSocket);

  return 0;

}
