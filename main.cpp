//this code is piece of shit, I know.

#include <iostream>
#include <cstring>
#include <string>
#include "echelonheaders.h"

// Port and bufSize declaration
int PORT = 7777;
int BUFFER_SIZE = 4096;


Config parseArgs(int argc, char** argv) {
  Config cfg;

  if (argc < 3) exit(1);

  cfg.mode = argv[1];
  cfg.action = argv[2];

  int i=3; // initial argc argument 
  

  // Mode and action arguments parsing
  if(cfg.mode == "server" && cfg.action == "recv"){}

  else if(cfg.mode == "server" && cfg.action == "send") {

    if(i >= argc) {
      std::cerr << "Check the usage\n";
      exit(1);
    }

    cfg.file = argv[i++];
  }

  else if(cfg.mode == "client" && cfg.action == "send") {

    if(i + 1 >= argc) {
      std::cerr << "Check the usage\n";
      exit(1);
    }

    cfg.file = argv[i++];
    cfg.ip = argv[i++];
  }

  else if(cfg.mode == "client" && cfg.action == "recv") {

    if(i >= argc) {
      std::cerr << "Check the usage\n";
      exit(1);
    }

    cfg.ip = argv[i++];
  }

  else {
    std::cerr << "Unknown mode/action\n";
    exit(1);
  }



  // Additional flags parsing
  for(; i < argc; i++) {
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

    else if (arg == "--port" && i+1 < argc) {
      cfg.port = std::stoi(argv[++i]);
    }

    else if (arg == "--buffer" && i+1 < argc) {
      cfg.bufSize = std::stoi(argv[++i]);
    }

    else if (arg.find("--speed=") == 0) {
      cfg.speed = std::stod(arg.substr(8));
    } 

    else if (arg.find("--passkey=") == 0) {
      cfg.passkey = arg.substr(10);
    }

    else if (arg.find("--port=") == 0) {
      cfg.port = std::stoi(arg.substr(7));
    }

    else if (arg.find("--buffer=") == 0) {
      cfg.bufSize = std::stoi(arg.substr(9));
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

  PORT = cfg.port;
  BUFFER_SIZE = cfg.bufSize;

  if(cfg.mode == "server") {
    if(cfg.action == "recv") {
      if(cfg.continuous) {
        while(true) {
          serverRecv(cfg);
        }
      }

      return serverRecv(cfg);
    }

    else if(cfg.action == "send") {
      return serverSend(cfg);
    }
  }

  else if(cfg.mode == "client") {
    if(cfg.action == "send") {
      return clientSend(cfg);
    }
    
    else if(cfg.action == "recv") {
      if(cfg.continuous) {
        while(true) {
          clientRecv(cfg);
        }
      }
      return clientRecv(cfg);
    }
  }

  return 0;
}
