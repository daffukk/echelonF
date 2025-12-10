#pragma once

int clientSend(int argc, char* argv[]);
int clientRecv(int argc, char* argv[]);
int serverSend(int argc, char* argv[]);
int serverRecv();

const int PORT = 7777;
const int BUFFER_SIZE = 4096;
