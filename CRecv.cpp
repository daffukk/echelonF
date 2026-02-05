#include <fstream>
#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "echelonheaders.h"

int clientRecv(Config cfg) {
  double speed = cfg.speed;
  const char* passkey = cfg.passkey.c_str();


  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET; //IPV4
  serverAddress.sin_port = htons(PORT); // set in echelonheaders.h file

  struct hostent* host = gethostbyname(cfg.ip.c_str()); // argv[3] is ip or domain
  serverAddress.sin_addr.s_addr = *((unsigned long*)host->h_addr); // converts domain to ip
 

  while(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) >= 0) {
    
    if(passkey != nullptr && strlen(passkey) > 0) {
      int passkeyLength = strlen(passkey);
      send(clientSocket, &passkeyLength, sizeof(int), 0);

      send(clientSocket, passkey, passkeyLength, 0);
    }

    int filenameSize;
    recv(clientSocket, &filenameSize, sizeof(int), 0);

    char filename[256];
    recv(clientSocket, filename, filenameSize, 0);
    filename[filenameSize] = '\0';
  
    std::streampos fileSize;
    recv(clientSocket, &fileSize, sizeof(int), 0);
  
    std::ofstream file(filename, std::ios::binary);
    
    char buffer[BUFFER_SIZE]; // set in echelonheaders.h file
    int bytes_recv;
    int sleepDuration;
    double MB = 0; // counter
    double fileSizeMB = (double)fileSize / 1000000;


    std::cout << "Recieving file: " << filename << std::endl;
    if(speed > 0) {
      sleepDuration = calculateSpeed(speed);
    }

    std::thread speedometer(BytesPerSecond, std::ref(bytesCounter), std::ref(speedBps), std::ref(running));

    while((bytes_recv = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
      updateReceiveProgress(file, buffer, bytes_recv, MB, fileSizeMB, bytesCounter, speedBps);

      if (speed > 0){
        std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
      }
    }

    running = false;
    speedometer.join();

    std::cout << std::endl;
    file.close();
  }


  close(clientSocket);
  return 0;
}
