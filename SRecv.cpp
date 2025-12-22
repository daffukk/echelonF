#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "echelonheaders.h"

int serverRecv(bool continuous, float speed, const char* passkey) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

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
  float MB = 0;
  
  std::cout << "Recieving file: " << filename << std::endl;


  if(speed > 0) {
    int sleepDuration = calculateSpeed(speed);

    while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      file.write(buffer, bytes_recieved);
      MB += (float)bytes_recieved / 1000000;
  
      std::cout << "\rRecieved: " << std::fixed << std::setprecision(1) << MB << " MB " << (int)(( MB / ((float)fileSize / 1000000)) * 100) << "% "<< std::flush;

      std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
    }

  } 
  else {
    while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      file.write(buffer, bytes_recieved);
      MB += (float)bytes_recieved / 1000000;
      
      std::cout << "\rRecieved: " << std::fixed << std::setprecision(1) << MB << " MB " << (int)(( MB / ((float)fileSize / 1000000)) * 100) << "% "<< std::flush;
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
