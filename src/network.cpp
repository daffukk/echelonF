#include <ecf/terminal.h>
#include <ecf/network.h>
#include <sys/socket.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>




void updateReceiveProgress(
    std::ofstream& file, 
    char* buffer, 
    int bytes_recieved, 
    double& MB, 
    double fileSizeMB, 
    TransferStats& tstats
) {
  file.write(buffer, bytes_recieved);
  MB += (double)bytes_recieved / 1000000; // progress counter(MB)
  tstats.bytesCounter += bytes_recieved; // is used in BytesPerSecond function
  double MBps = static_cast<double>(tstats.speedBps / 1024.0 / 1024.0); 

  int percent = static_cast<int>(( MB / fileSizeMB) * 100);
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



void updateSendProgress(
    int clientSocket, 
    char* buffer, 
    int bytes_read, 
    double& MB, 
    double fileSizeMB, 
    TransferStats& tstats
) {
  send(clientSocket, buffer, bytes_read, 0);
  MB += (double)bytes_read / 1000000;
  tstats.bytesCounter += bytes_read;
  double MBps = static_cast<double>(tstats.speedBps / 1024.0 / 1024.0); 

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



void BytesPerSecond(TransferStats& tstats) {
  namespace ch = std::chrono;

  while(tstats.running) {
    auto start = ch::steady_clock::now();
    std::this_thread::sleep_for(ch::milliseconds(100));
    auto end = ch::steady_clock::now();

    double dt = ch::duration<double>(end - start).count();
    tstats.speedBps = static_cast<uint64_t>(tstats.bytesCounter.exchange(0) / dt);  
  }
}

