#pragma once
#include <cstring>
#include <iostream>

int clientSend(int argc, char* argv[]);
int clientRecv(int argc, char* argv[], bool continuous=false);
int serverSend(int argc, char* argv[]);
int serverRecv(bool continuous=false);

inline bool flagFinder(int argc, char* argv[], const char* flag) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0) {
      return true;
    }
  }
  return false;
}

inline void printClientHelp(int argc, char* argv[]) {
  std::cout << "Usage: " << argv[0] << " <MODE> <OPTIONS>\n"
    << "Modes:\n"
    << "\tsend <FILE> <IP/DOMAIN> \t Send files\n"
    << "\trecv <IP/DOMAIN> \t Receive files\n"
    << "Options:\n"
    << "\t-h, --help \t show this text"
    << "\t-a, --always \t receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode\n";
    
}

constexpr int PORT = 7777;
constexpr int BUFFER_SIZE = 4096;
