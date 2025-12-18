#include <iostream>
#include <cstring>
#include "echelonheaders.h"



int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <mode>" << std::endl;
    return 1;
  }

  if(argc >= 2 && strcmp(argv[1], "recv") == 0) {
    bool continuous = false;
    float speed = 0;

    if(flagFinder(argc, argv, "--always") || flagFinder(argc, argv, "-a")) {
      continuous = true;
    }

    float tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    }

    while(continuous) {
      serverRecv(continuous, speed);
    }

    return serverRecv(continuous, speed);
    
  }

  if(strcmp(argv[1], "send") == 0) {
    float speed = 0;

    float tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    } 

    return serverSend(argc, argv, speed);
  }
  
  return 0;
}
