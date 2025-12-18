#include <iostream>
#include <cstring>
#include "echelonheaders.h"

int main(int argc, char* argv[]) {

  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " <mode>" << std::endl;
    return 1;
  }

  if(strcmp(argv[1], "send") == 0) {
    float speed = 0;

    float tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    } 

    return clientSend(argc, argv, speed);
  }
  
  if(strcmp(argv[1], "recv") == 0) {
    if(argc < 2) {
      std::cout << "Usage " << argv[0] << " recv <ip/domain>" << std::endl;
      return 1;
    }
  }

  if(argc >= 3 && strcmp(argv[1], "recv") == 0) {
    bool continuous = false;
    float speed = 0;

    if(flagFinder(argc, argv, "--always") || flagFinder(argc, argv, "-a")) {
      continuous = true;
    }
    
    float tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    }
    
    while(continuous){
      clientRecv(argc, argv, continuous, speed);
    }

    return clientRecv(argc, argv, continuous, speed);
  
  }

  return 0;
}
