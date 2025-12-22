#pragma once
#include <cstring>
#include <iostream>
#include <string>

int clientSend(int argc, char* argv[], float speed=0, const char* passkey="");
int clientRecv(int argc, char* argv[], bool continuous=false, float speed=0);
int serverSend(int argc, char* argv[], float speed=0);
int serverRecv(bool continuous=false, float speed=0, const char* passkey="");

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
  float oneMegabytePerSec = 250; // EQUALS 250 ONLY WHEN BUFFER_SIZE EQUALS 4096!!!! Would be fixed
  float sleepDuration = oneMegabytePerSec / speed; 
  return (int)sleepDuration;
}

inline void printClientHelp(int argc, char* argv[]) {
  std::cout << "Usage: " << argv[0] << " <MODE> <OPTIONS>\n"
    << "Modes:\n"
    << "\tsend <FILE> <IP/DOMAIN> \t Send files\n"
    << "\trecv <IP/DOMAIN> \t Receive files\n"
    << "Options:\n"
    << "\t-h, --help \t Show this text"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t--speed= \t Specify sending/receiving speed. Accept only int numbers."
    << "\t--passkey= \t Set passkey to an application, very useful flag if you are not in local network.";
    
}

constexpr int PORT = 7777;
constexpr int BUFFER_SIZE = 4096; // IF YOU WILL CHANGE THAT, YOU WILL HAVE TO MODIFY calculateSpeed() FUNCTION!!!! Would be fixed
