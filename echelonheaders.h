#pragma once
#include <cstring>
#include <iostream>
#include <string>

int clientSend(int argc, char* argv[], float speed=0, const char* passkey="");
int clientRecv(int argc, char* argv[], bool continuous=false, float speed=0, const char* passkey="");
int serverSend(int argc, char* argv[], float speed=0, const char* passkey="");
int serverRecv(bool continuous=false, float speed=0, const char* passkey="");


constexpr int PORT = 7777;
constexpr int BUFFER_SIZE = 4096;


inline bool flagFinder(int argc, char* argv[], const char* flag) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0) {
      return true;
    }
  }
  return false;
}

inline float findFlagValue(int argc, char* argv[], const char* flag) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
      if(arg.find(flag) == 0) {
        float flagValue = std::stof(arg.substr(strlen(flag)));

        if(flagValue < 0) {
          std::cout << "Invalid argument value: " << arg << std::endl;
          return -1;
        }

        return flagValue;
      }
  }
  return -1;
}

inline std::string findFlagValueStr(int argc, char* argv[], const char* flag) {
  for(int i=1; i < argc; i++) {
    std::string arg = argv[i];
    if(arg.find(flag) == 0) {
      std::string flagValue = arg.substr(strlen(flag));
      return flagValue;
    }
  }
  return "";
}

inline int calculateSpeed(float speed) {
  float oneMegabytePerSec = 1000 / (BUFFER_SIZE / 1024);
  float sleepDuration = oneMegabytePerSec / speed; 
  return (int)sleepDuration;
}

inline void printClientHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << " client <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " client send video.mp4 dwpo.xyz --passkey=abc12345\n"
    << "\t" << argv[0] << " client recv dwpo.xyz --passkey=abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\tsend <FILE> <IP/DOMAIN> \t Send files\n"
    << "\trecv <IP/DOMAIN> \t Receive files\n"
    << "\n"
    << "Options:\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t--speed= \t Specify sending/receiving speed. Accept only int numbers.\n"
    << "\t--passkey= \t Set passkey to an application, very useful flag if you are not in local network.\n";
    
}

inline void printServerHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << "server <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " server recv --passkey=abc12345\n"
    << "\t" << argv[0] << " server send video.mp4 --passkey=abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\trecv \t Receive files\n"
    << "\tsend <FILE> \t Send files\n"
    << "\n"
    << "Options:\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t--speed= \t Specify sending/receiving speed. Accept only int numbers.\n"
    << "\t--passkey= \t Set passkey to an application, very useful flag if you are not in local network.\n";
    
}


