//this code is piece of shit, I know.

#include <iostream>
#include <cstring>
#include <string>
#include "echelonheaders.h"



struct Config {
  bool continuous = false;
  double speed = 0.0;

  std::string passkey;
  std::string mode;
  std::string action;
};

Config parseArgs(int argc, char** argv) {
  Config cfg;

  if(argc >= 2) {
     cfg.mode = argv[1];
  }

  if(argc >= 3) {
    cfg.action = argv[2];
  }

  for(int i = 3; i < argc; i++) {
    std::string arg = argv[i];

    if(arg == "--always" || arg == "-a") {
      cfg.continuous = true;
    }

    else if (arg == "--passkey" && i+1 < argc) {
      cfg.passkey = argv[++i];
    }

    else if (arg == "--speed" && i+1 < argc) {
      cfg.speed = std::stod(argv[++i]);
    }
  }

  return cfg;
}



int main(int argc, char* argv[]) {

  // --help

  if(argc < 3) {
    std::cout << "\n----server----\n";
    printServerHelp(argc, argv);
    std::cout << "\n----client----\n";
    printClientHelp(argc, argv);
    return 1;
  }

  Config cfg = parseArgs(argc, argv);

  if(cfg.mode == "server") {
    if(cfg.action == "recv") {
      if(cfg.continuous) {
        while(true) {
          serverRecv(true, cfg.speed, cfg.passkey.c_str());
        }
      }

      return serverRecv(cfg.continuous, cfg.speed, cfg.passkey.c_str());
    }

    else if(cfg.action == "send") {
      return serverSend(argc, argv, cfg.speed, cfg.passkey.c_str());
    }
  }

  else if(cfg.mode == "client") {
    if(cfg.action == "send") {
      return clientSend(argc, argv, cfg.speed, cfg.passkey.c_str());
    }
    
    else if(cfg.action == "recv") {
      if(cfg.continuous) {
        while(true) {
          clientRecv(argc, argv, cfg.continuous, cfg.speed, cfg.passkey.c_str());
        }
      }
      return clientRecv(argc, argv, cfg.continuous, cfg.speed, cfg.passkey.c_str());
    }
  }

  return 0;
}
