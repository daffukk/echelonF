#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>


struct Config {
  int port = 7777;
  int bufSize = 4096;

  std::string ip;
  std::string file;

  bool continuous = false;
  double speed = 0.0;

  std::string passkey = "";
  std::string mode;
  std::string action;
};



int clientSend(Config cfg);
int clientRecv(Config cfg);
int serverSend(Config cfg);
int serverRecv(Config cfg);
void updateReceiveProgress(std::ofstream& file, char* buffer, int bytes_recieved, double& MB, double fileSizeMB);
void updateSendProgress(int clientSocket, char* buffer, int bytes_read, double& MB, double fileSizeMB);


extern int PORT;
extern int BUFFER_SIZE;

// Text coloring
namespace color {  
  constexpr const char* red = "\033[31m";
  constexpr const char* green = "\033[32m";
  constexpr const char* yellow = "\033[33m";
  constexpr const char* magneta = "\033[35m";
  constexpr const char* cyan = "\033[36m";  
  constexpr const char* redbg = "\033[41m";
  constexpr const char* greenbg = "\033[42m";
  constexpr const char* yellowbg = "\033[43m";
  constexpr const char* magnetabg = "\033[45m";
  constexpr const char* cyanbg = "\033[46m";
  constexpr const char* reset = "\033[0m";
}


inline int getTerminalWidth() {
  struct winsize w;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) return w.ws_col;
  return 80;
}

// Calculate progressbar length
inline int getBarWidth() { 
  return std::clamp(getTerminalWidth() - 40, 10, 50);
}

// Find flags like --always
inline bool flagFinder(int argc, char* argv[], const char* flag) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0) {
      return true;
    }
  }
  return false;
}

// Find flags with double value like `--speed=0.1` <- this flag returns 0.1
inline double findFlagValue(int argc, char* argv[], const char* flag) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
      if(arg.find(flag) == 0) {
        double flagValue = std::stof(arg.substr(strlen(flag)));

        if(flagValue < 0) {
          std::cout << "Invalid argument value: " << arg << std::endl;
          return -1;
        }

        return flagValue;
      }
  }
  return -1;
}

// Find flags with string value like `--passkey=qwerty123` <- this flag returns qwerty123
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

// Actually calculates the sleep duration for a custom speed
inline int calculateSpeed(double speed) {
  double oneMegabytePerSec = 1000 / ((double)BUFFER_SIZE / 1024);
  double sleepDuration = oneMegabytePerSec / speed; 
  return (int)sleepDuration;
}

inline void printClientHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << " client <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " client send video.mp4 dwpo.xyz --passkey abc12345\n"
    << "\t" << argv[0] << " client recv dwpo.xyz --passkey abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\tsend <FILE> <IP/DOMAIN> \t Send files\n"
    << "\trecv <IP/DOMAIN> \t Receive files\n"
    << "\n"
    << "Options:\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t--speed \t Specify sending/receiving speed. Accept only int numbers.\n"
    << "\t--passkey \t Set passkey to an application, very useful flag if you are not in local network.\n";
    
}

inline void printServerHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << "server <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " server recv --passkey abc12345\n"
    << "\t" << argv[0] << " server send video.mp4 --passkey abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\trecv \t Receive files\n"
    << "\tsend <FILE> \t Send files\n"
    << "\n"
    << "Options:\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t--speed \t Specify sending/receiving speed. Accept only int numbers.\n"
    << "\t--passkey \t Set passkey to an application, very useful flag if you are not in local network.\n";
    
}


