#include <iostream>
#include <cstring>
#include "echelonheaders.h"

int main(int argc, char* argv[]) {

  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " <mode>" << std::endl;
    return 1;
  }

  if(strcmp(argv[1], "send") == 0) {
    if(argc != 4) {
      std::cout << "Usage: " << argv[0] << " send <file> <ip/domain>" << std::endl;
      return 1;
    }
    
    return clientSend(argc, argv);
  }
  
  if(strcmp(argv[1], "recv") == 0) {
    if(argc != 3) {
      std::cout << "Usage " << argv[0] << " recv <ip/domain>" << std::endl;
      return 1;
    }

    return clientRecv(argc, argv);
  }

  return 0;
}
