#pragma once
#include <atomic>
#include <fstream>

struct TransferStats {
  std::atomic<uint64_t> bytesCounter{0};
  std::atomic<uint64_t> speedBps{0};
  std::atomic<bool> running{true};
};



void updateReceiveProgress(
    std::ofstream& file, 
    char* buffer, 
    int bytes_recieved, 
    double& MB, 
    double fileSizeMB, 
    TransferStats& tstats
);

void updateSendProgress(
    int clientSocket, 
    char* buffer, 
    int bytes_read, 
    double& MB, 
    double fileSizeMB, 
    TransferStats& tstats
);

void BytesPerSecond(TransferStats& tstats);

