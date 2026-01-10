#include <iomanip>
#include <netdb.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "echelonheaders.h"

int clientSend(int argc, char* argv[], double speed, const char* passkey) {

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  
  struct hostent* host = gethostbyname(argv[4]);
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr);


  connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  const char* filename = argv[3];

  std::ifstream file(filename, std::ios::binary);

  if(!file.is_open()) {
    std::cerr << "Error: can't open file " << filename << std::endl;
    return 1;
  }

  if(passkey != nullptr && strlen(passkey) > 0) {
    int passkeyLength = strlen(passkey);
    send(clientSocket, &passkeyLength, sizeof(int), 0);

    send(clientSocket, passkey, passkeyLength, 0);
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
  double MB = 0;
  std::string progressBar(50, ' ');
  double fileSizeMB = (double)fileSize / 1000000;

  if(speed > 0) {
    int sleepDuration = calculateSpeed(speed);

    while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
      send(clientSocket, buffer, bytes_read, 0);
      MB += (double)bytes_read / 1000000;

      int percent = (int)(( MB / fileSizeMB) * 100);

      if(percent % 2 == 0){
        progressBar[percent / 2] = '=';
      }


      std::cout << "\r[" << progressBar << "] " 
        << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB " 
        << percent << "% "<< std::flush;

      std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
    }
  }
  else {
    while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
      send(clientSocket, buffer, bytes_read, 0);
      MB += (double)bytes_read / 1000000;

      int percent = (int)(( MB / fileSizeMB) * 100);

      if(percent % 2 == 0){
        progressBar[percent / 2] = '=';
      }


      std::cout << "\r[" << progressBar << "] " 
        << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB " 
        << percent << "% "<< std::flush;
    }
  }

  std::cout << std::endl;
  file.close();
  close(clientSocket);

  return 0;
  
}
