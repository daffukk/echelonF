#include <ecf/terminal.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <algorithm>


int getTerminalWidth() {
  struct winsize w;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) return w.ws_col;
  return 80;
}

int getBarWidth() { 
  return std::clamp(getTerminalWidth() - 50, 10, 50);
}

