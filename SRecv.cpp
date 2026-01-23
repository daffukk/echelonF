#include <fstream>
#include <ios>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "echelonheaders.h"




int serverRecv(bool continuous, double speed, const char* passkey) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET; // IPV4
  serverAddress.sin_port = htons(PORT); // set in the echelonheaders.h file
  serverAddress.sin_addr.s_addr = INADDR_ANY; // bind on 0.0.0.0

  bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  if(passkey != nullptr && strlen(passkey) > 0) {
    bool logged = false;
    while(logged != true) {
      int userPasskeyLenght;
      recv(clientSocket, &userPasskeyLenght, sizeof(int), 0);
  
      char userPasskey[256];
      recv(clientSocket, userPasskey, userPasskeyLenght, 0);
      userPasskey[userPasskeyLenght] = '\0';
  
      if(strcmp(userPasskey, passkey) != 0) {
        std::cout << "Wrong passkey." << std::endl;
        close(clientSocket);

        clientSocket = accept(serverSocket, nullptr, nullptr);
        continue;
      }
      else {
        std::cout << "Passkey accepted." << std::endl;
        logged = true;
      }
    }
  }

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
  int sleepDuration;
  double MB = 0;
  double fileSizeMB = (double)fileSize / 1000000;


  std::cout << "Recieving file: " << filename << std::endl;
  if(speed > 0) {
    sleepDuration = calculateSpeed(speed);
  }

  while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    updateReceiveProgress(file, buffer, bytes_recieved, MB, fileSizeMB);

    if(speed > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
    }
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
