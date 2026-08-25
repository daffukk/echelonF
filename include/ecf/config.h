#pragma once
#include <string>



struct Config {
  int port = 7777;
  int bufSize = 4096;

  std::string ip;
  std::string file;

  bool continuous = false;
  double speed = 0.0;

  int attemptAmount = 5;
  int attemptInterval = 2000; // millisec

  int fileCounter = 1;

  std::string passkey = "";
  std::string mode;
  std::string action;
};
