//this code is piece of shit, I know.

#include <iostream>
#include <cstring>
#include <string>
#include "echelonheaders.h"

int main(int argc, char* argv[]) {

  // --help

  if(argc < 2) {
    std::cout << "\n----server----\n";
    printServerHelp(argc, argv);
    std::cout << "\n----client----\n";
    printClientHelp(argc, argv);
    return 1;
  }

  else if(strcmp(argv[1], "client") == 0 &&
      (argc == 2 || flagFinder(argc, argv, "--help") || flagFinder(argc, argv, "-h"))) {
    printClientHelp(argc, argv);
    return 0;
  }

  else if(strcmp(argv[1], "server") == 0 && 
      (argc == 2 ||flagFinder(argc, argv, "--help") || flagFinder(argc, argv, "-h"))) {
    printServerHelp(argc, argv);
    return 0;
  }



  // server

  if(argc >= 3 && strcmp(argv[1], "server") == 0){
    bool continuous = false;
    double speed = 0;
    std::string passkey = "";

    if(flagFinder(argc, argv, "--always") || flagFinder(argc, argv, "-a")) {
      continuous = true;
    }
    
    double tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    }

    std::string tempPasskey = findFlagValueStr(argc, argv, "--passkey=");
    if(tempPasskey != "" && strlen(tempPasskey.c_str()) > 0) {
      passkey = tempPasskey;
    }

    if(strcmp(argv[2], "recv") == 0) {
      while(continuous){
        serverRecv(continuous, speed, passkey.c_str());
      }
      return serverRecv(continuous, speed, passkey.c_str());
    }

    else if(strcmp(argv[2], "send") == 0 && argc >= 4) {
      return serverSend(argc, argv, speed, passkey.c_str());
    }
  }



  //client

  else if(argc >= 4 && strcmp(argv[1], "client") == 0) {
    bool continuous = false;
    double speed = 0;
    std::string passkey = "";

    if(flagFinder(argc, argv, "--always") || flagFinder(argc, argv, "-a")) {
      continuous = true;
    }
    
    double tempSpeed = findFlagValue(argc, argv, "--speed=");
    if(tempSpeed > 0) {
      speed = tempSpeed; 
    }

    std::string tempPasskey = findFlagValueStr(argc, argv, "--passkey=");
    if(tempPasskey != "" && strlen(tempPasskey.c_str()) > 0) {
      passkey = tempPasskey;
    }

    if(strcmp(argv[2], "send") == 0 && argc >= 5) {
      return clientSend(argc, argv, speed, passkey.c_str());
    }
    else if(strcmp(argv[2], "recv") == 0) {
      while(continuous){
        clientRecv(argc, argv, continuous, speed, passkey.c_str());
      }
      return clientRecv(argc, argv, continuous, speed, passkey.c_str());
    }
  }

  return 0;
}
