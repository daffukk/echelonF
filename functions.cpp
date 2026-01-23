#include <iomanip>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include "echelonheaders.h"


void updateReceiveProgress(std::ofstream& file, char* buffer, int bytes_recieved, double& MB, double fileSizeMB) {
  file.write(buffer, bytes_recieved);
  MB += (double)bytes_recieved / 1000000; // progress counter

  int percent = (int)(( MB / fileSizeMB) * 100);
  percent = std::min(percent, 100);
  int filled = percent * getBarWidth() / 100;


  std::string progressBar(getBarWidth(), ' ');
  for(int i=0; i < filled; ++i) progressBar[i] = '=';

  std::cout << "\r" << color::cyan << "Progress: " << percent  << "%" << color::reset
    << " [" << color::green << progressBar << color::reset << "] " 
    << color::green << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB " 
    << color::reset << std::flush;
}

void updateSendProgress(int clientSocket, char* buffer, int bytes_read, double& MB, double fileSizeMB) {
  send(clientSocket, buffer, bytes_read, 0);
  MB += (double)bytes_read / 1000000;

  int percent = (int)(( MB / fileSizeMB) * 100);
  percent = std::min(percent, 100);
  int filled = percent * getBarWidth() / 100;


  std::string progressBar(getBarWidth(), ' ');
  for(int i=0; i < filled; ++i) progressBar[i] = '=';

  std::cout << "\r" << color::cyan << "Progress: " << percent << "%" << color::reset 
    << " [" << color::green << progressBar << color::reset << "] " 
    << color::green << std::fixed << std::setprecision(1) << MB << "/" << fileSizeMB << " MB " 
    << color::reset << std::flush;
}
