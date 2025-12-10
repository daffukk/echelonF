#include <iostream>
#include <cstring>
#include "echelonheaders.h"

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <mode>" << std::endl;
    return 1;
  }
  if(argc == 2 && strcmp(argv[1], "recv") == 0) {
    return serverRecv();
  }

  if(strcmp(argv[1], "send") == 0) {
    if(argc != 3) {
      std::cout << "Usage: " << argv[0] << " send <file>" << std::endl;
      return 1;
    }
    
    return serverSend(argc, argv);
  }
  
  return 0;
}
