#include <chrono>
#include <cerrno>
#include <cstring>
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




int serverRecv(Config cfg) {
  namespace ch = std::chrono;

  bool continuous = cfg.continuous;
  double speed = cfg.speed;
  const char* passkey = cfg.passkey.c_str();

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET; // IPV4
  serverAddress.sin_port = htons(PORT); // set in the echelonheaders.h file
  serverAddress.sin_addr.s_addr = INADDR_ANY; // bind on 0.0.0.0
  
  int i;
  for(i=0; i < cfg.attemptAmount; i++) {
    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == 0) {
      break;
    }
    
    std::cerr << "Failed binding (attempt " << (i+1) << "/" << cfg.attemptAmount << ").\n";
    std::this_thread::sleep_for(ch::seconds(cfg.attemptInterval));
  }

  if(i == cfg.attemptAmount) {
    std::cout << "Failed to bind.\n" << std::strerror(errno) << "\n";
    return 1;
  }


  listen(serverSocket, 5);
  
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  // poorly written passkey system
  if(passkey != nullptr && strlen(passkey) > 0) {
    bool logged = false;
    while(logged != true) {
      int userPasskeyLength;
      recv(clientSocket, &userPasskeyLength, sizeof(int), 0);

      if(userPasskeyLength <= 0 || userPasskeyLength > 1024) {
        std::cerr << "Invalid passkey lenght\n";
        close(clientSocket);

        clientSocket = accept(serverSocket, nullptr, nullptr);
        continue;
      }

      std::string userPasskey(userPasskeyLength, '\0');
      recv(clientSocket, userPasskey.data(), userPasskeyLength, 0);

      if(strcmp(userPasskey.c_str(), passkey) != 0) {
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

  if(filenameSize <= 0 || filenameSize > 512) {
    std::cerr << "Invalid filename lenght\n";
    close(clientSocket);
  }

  std::string filename(filenameSize, '\0');
  recv(clientSocket, filename.data(), filenameSize, 0);

  std::streampos fileSize;
  recv(clientSocket, &fileSize, sizeof(int), 0);
  
  std::ofstream file(filename, std::ios::binary);

  char buffer[BUFFER_SIZE];
  int bytes_recieved;
  int sleepDuration;
  double MB = 0;
  double fileSizeMB = (double)fileSize / 1000000;


  std::cout << "Recieving file: " << filename << std::endl;
  if(speed > 0) {
    sleepDuration = calculateSpeed(speed);
  }

  // For transfer speed meter in progressBar
  std::thread speedometer(BytesPerSecond, std::ref(bytesCounter), std::ref(speedBps), std::ref(running));

  while((bytes_recieved = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    updateReceiveProgress(file, buffer, bytes_recieved, MB, fileSizeMB, bytesCounter, speedBps);


    if(speed > 0) {
      std::this_thread::sleep_for(ch::microseconds(sleepDuration));
    }
  }

  running = false; // is used in speedometer thread
  speedometer.join();
  
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
