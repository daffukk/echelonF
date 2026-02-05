#include <cstdint>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include "echelonheaders.h"

std::atomic<uint64_t> bytesCounter{0};
std::atomic<uint64_t> speedBps{0};
std::atomic<bool> running{true};

void BytesPerSecond(std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps, std::atomic<bool>& running) {
  namespace ch = std::chrono;

  while(running) {
    auto start = ch::steady_clock::now();
    std::this_thread::sleep_for(ch::milliseconds(100));
    auto end = ch::steady_clock::now();

    double dt = ch::duration<double>(end - start).count();
    speedBps = static_cast<uint64_t>(bytesCounter.exchange(0) / dt);
  }
}


void updateReceiveProgress(std::ofstream& file, char* buffer, int bytes_recieved, double& MB, double fileSizeMB, std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps) {
  file.write(buffer, bytes_recieved);
  MB += (double)bytes_recieved / 1000000; // progress counter
  bytesCounter += bytes_recieved; // is used in BytesPerSecond function
  double MBps = static_cast<double>(speedBps / 1024.0 / 1024.0); 

  int percent = (int)(( MB / fileSizeMB) * 100);
  percent = std::min(percent, 100);
  int filled = percent * getBarWidth() / 100;


  std::string progressBar(getBarWidth(), ' ');
  for(int i=0; i < filled; ++i) progressBar[i] = '=';

  std::cout << "\r" << color::cyan << "Progress: "
    << percent  << "% " << color::reset
    << color::green << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB " 
    << " [" << color::green << progressBar << color::reset << "] " 
    << color::yellow << MBps << " MB/s "
    << color::reset << std::flush;
}

void updateSendProgress(int clientSocket, char* buffer, int bytes_read, double& MB, double fileSizeMB, std::atomic<uint64_t>& bytesCounter, std::atomic<uint64_t>& speedBps) {
  send(clientSocket, buffer, bytes_read, 0);
  MB += (double)bytes_read / 1000000;
  bytesCounter += bytes_read;
  double MBps = static_cast<double>(speedBps / 1024.0 / 1024.0); 

  int percent = (int)(( MB / fileSizeMB) * 100);
  percent = std::min(percent, 100);
  int filled = percent * getBarWidth() / 100;


  std::string progressBar(getBarWidth(), ' ');
  for(int i=0; i < filled; ++i) progressBar[i] = '=';

  std::cout << "\r" << color::cyan << "Progress: " 
    << percent << "% " << color::reset 
    << color::green << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB "
    << " [" << color::green << progressBar << color::reset << "] " 
    << color::yellow << MBps << " MB/s "
    << color::reset << std::flush;
}
