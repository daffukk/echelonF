#include <ecf/config.h>
#include <ecf/utils.h>



int calculateSpeed(double speed, const Config& cfg) {
  double bytesPerSecond = speed * 1024 * 1024;
  
  double chunksPerSecond = bytesPerSecond / cfg.bufSize;
  double secondsPerChunk = 1 / chunksPerSecond;
  double sleepDuration = secondsPerChunk * 1000000;
  return (int)sleepDuration;
}
