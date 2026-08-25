#pragma once
#include "config.h"
#include <iostream>

int calculateSpeed(double speed, const Config& cfg);

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

