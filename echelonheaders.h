#pragma once
#include <algorithm>
#include <cstdint>
#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>


struct Config {
  int port = 7777;
  int bufSize = 4096;

  std::string ip;
  std::string file;

  bool continuous = false;
  double speed = 0.0;

  int attemptAmount = 5;
  int attemptInterval = 2;

  std::string passkey = "";
  std::string mode;
  std::string action;
};



int clientSend(Config cfg);
int clientRecv(Config cfg);
int serverSend(Config cfg);
int serverRecv(Config cfg);
void updateReceiveProgress(std::ofstream& file, char* buffer, int bytes_recieved, double& MB, double fileSizeMB, std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps);
void updateSendProgress(int clientSocket, char* buffer, int bytes_read, double& MB, double fileSizeMB, std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps);
void BytesPerSecond(std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps, std::atomic<bool>& running);

extern std::atomic<uint64_t> bytesCounter;
extern std::atomic<uint64_t> speedBps;
extern std::atomic<bool> running;
extern int PORT;
extern int BUFFER_SIZE;

// Text coloring
namespace color {  
  constexpr const char* red = "\033[31m";
  constexpr const char* green = "\033[32m";
  constexpr const char* yellow = "\033[33m";
  constexpr const char* magneta = "\033[35m";
  constexpr const char* cyan = "\033[36m";  
  constexpr const char* redbg = "\033[41m";
  constexpr const char* greenbg = "\033[42m";
  constexpr const char* yellowbg = "\033[43m";
  constexpr const char* magnetabg = "\033[45m";
  constexpr const char* cyanbg = "\033[46m";
  constexpr const char* reset = "\033[0m";
}

// get terminal width in symbols using ioctl lib
inline int getTerminalWidth() {
  struct winsize w;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) return w.ws_col;
  return 80;
}

// Calculate progressbar length
inline int getBarWidth() { 
  return std::clamp(getTerminalWidth() - 50, 10, 50);
}

// Actually calculates the sleep duration for a custom speed
inline int calculateSpeed(double speed) {
  double bytesPerSecond = speed * 1024 * 1024;
  
  double chunksPerSecond = bytesPerSecond / BUFFER_SIZE;
  double secondsPerChunk = 1 / chunksPerSecond;
  double sleepDuration = secondsPerChunk * 1000000;
  return (int)sleepDuration;
}

inline void printClientHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << " client <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " client send video.mp4 dwpo.xyz --passkey abc12345\n"
    << "\t" << argv[0] << " client recv dwpo.xyz --passkey abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\tsend <FILE> <IP/DOMAIN> \t Send files\n"
    << "\trecv <IP/DOMAIN> \t Receive files\n"
    << "\n"
    << "Options:\n"
    << "Flags can be used in --port 1234 or --port=1234 format\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t-n \t Custom amount of attempts(for binding or connecting).\n"
    << "\t--speed \t Specify transfer speed. MB/S\n"
    << "\t--passkey \t Set a passkey to an application, very useful flag if you are not in local network.\n"
    << "\t--port \t Set a custom port, port must be between 1 and 65535.\n"
    << "\t--buffer \t Set a custom buffer size, must be atleast 250 bytes.\n";
}

inline void printServerHelp(int argc, char* argv[]) {
  std::cout 
    << "Usage: " << argv[0] << "server <MODE> <OPTIONS>\n"
    << "\n"
    << "Examples:\n"
    << "\t" << argv[0] << " server recv --passkey abc12345\n"
    << "\t" << argv[0] << " server send video.mp4 --passkey abc12345\n"
    << "\n"
    << "Modes:\n"
    << "\trecv \t Receive files\n"
    << "\tsend <FILE> \t Send files\n"
    << "\n"
    << "Options:\n"
    << "Flags can be used in --port 1234 or --port=1234 format\n"
    << "\t-h, --help \t Show this text\n"
    << "\t-a, --always \t Receive files without stopping the application, application will not stop after receiving single file. Only avaible in receiving mode.\n"
    << "\t-n \t Custom amount of attempts(for binding or connecting).\n"
    << "\t--speed \t Specify transfer speed. MB/S\n"
    << "\t--passkey \t Set a passkey to an application, very useful flag if you are not in local network.\n"
    << "\t--port \t Set a custom port, port must be between 1 and 65535.\n"
    << "\t--buffer \t Set a custom buffer size, must be atleast 250 bytes.\n";
}


