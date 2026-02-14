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

int clientSend(Config cfg) {
  namespace ch = std::chrono;

  double speed = cfg.speed;
  const char* passkey = cfg.passkey.c_str();
  const char* filename = cfg.file.c_str(); 

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET; //IPV4
  serverAddress.sin_port = htons(PORT); // set in the echelonheaders.h file
  
  struct hostent* host = gethostbyname(cfg.ip.c_str());  
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr); // converts a domain to ip


  int i;
  for(i=0; i<5; i++) {
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == 0) {
      break;
    }

    std::cerr << "Failed to connect (attempt " << (i+1) << "/5).\n";
    if(i<4) {
      std::this_thread::sleep_for(ch::seconds(2));
    }
  }

  if(i == 5) {
    std::cerr << "Failed to connect.\n" << std::strerror(errno) << "\n";
    close(clientSocket);
    return 1;
  }


  std::ifstream file(filename, std::ios::binary);

  if(!file.is_open()) {
    std::cerr << "Error: can't open file " << filename << std::endl;
    return 1;
  }

  if(passkey != nullptr && strlen(passkey) > 0) {
    int passkeyLength = strlen(passkey);
    if(passkeyLength <= 0 || passkeyLength > 1024) {
      std::cerr << "Invalid passkey lenght\n";
      close(clientSocket);
      return 1;
    }

    send(clientSocket, &passkeyLength, sizeof(int), 0);

    send(clientSocket, passkey, passkeyLength, 0);
  }

  int filenameSize = strlen(filename);
  if(filenameSize <= 0 || filenameSize > 512) {
    std::cerr << "Invalid filename lenght\n";
    close(clientSocket);
    return 1;
  }

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

  std::thread speedometer(BytesPerSecond, std::ref(bytesCounter), std::ref(speedBps), std::ref(running));
  
  while((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
    updateSendProgress(clientSocket, buffer, bytes_read, MB, fileSizeMB, bytesCounter, speedBps);

    if(speed > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
    }

  }

  running = false;
  speedometer.join();

  std::cout << std::endl;
  file.close();
  close(clientSocket);

  return 0;
  
}
