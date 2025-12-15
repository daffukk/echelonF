#pragma once
#include <cstring>

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

const int PORT = 7777;
const int BUFFER_SIZE = 4096;
