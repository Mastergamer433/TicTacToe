#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
// classes

class GameData {
public:
  bool done;
  char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  std::string in;
  char turn;
};

class ServerData {
public:
  int server_fd, valread;
  int sockets[2];

  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello = "Hello from server";
};

ServerData sd;
GameData gd;


int server() {

  // Creating socket file descriptor
  if ((sd.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(sd.server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &sd.opt,
                 sizeof(sd.opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  sd.address.sin_family = AF_INET;
  sd.address.sin_addr.s_addr = INADDR_ANY;
  sd.address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(sd.server_fd, (struct sockaddr *)&sd.address, sizeof(sd.address)) <
      0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(sd.server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

int waitForConnection(){
  if ((sd.sockets[0] = accept(sd.server_fd, (struct sockaddr *)&sd.address,
                              (socklen_t *)&sd.addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
}

int read(){
  sd.valread = read(sd.sockets[0], sd.buffer, 1024);
  printf("%s\n", sd.buffer);
}

int send(char* message){
  send(sd.sockets[0], message, strlen(message), 0);
  printf("Hello message sent\n");
}


// functions

int switchTurn() {
  if (gd.turn == 'X') {
    gd.turn = 'O';
  } else {
    gd.turn = 'X';
  }
  return 0;
}
int makeMove() {
  if (gd.board[std::stoi(gd.in)] != ' ') {
    return 1;
  }

  gd.board[std::stoi(gd.in)] = gd.turn;
  return 0;
}

int move() {
  bool invalidMove = true;
  send("[MOVE]");
  // while (invalidMove) {
    // std::cout << "It's your turn " << gd.turn << ": ";
    // std::cin >> gd.in;
    // gd.in = (std::to_string(((std::stoi(gd.in)) - 1)));
    // int err = makeMove();
    // if (err != 0) {
    //   std::cout << "Error while making move: " << err << "\n";
    //   switch (err) {
    //   case 1:
    //     std::cout << "You made an invalid move... Try again.\n";
    //   }
    // } else {
    //   invalidMove = false;
    // }
  // }

  gd.in = (std::to_string(((std::stoi(gd.in)) - 1)));
  return 0;
}

int checkWin() {
  if (gd.board[0] == gd.turn && gd.board[1] == gd.turn &&
          gd.board[2] == gd.turn ||
      gd.board[3] == gd.turn && gd.board[4] == gd.turn &&
          gd.board[5] == gd.turn ||
      gd.board[6] == gd.turn && gd.board[7] == gd.turn &&
          gd.board[8] == gd.turn ||
      gd.board[0] == gd.turn && gd.board[3] == gd.turn &&
          gd.board[6] == gd.turn ||
      gd.board[1] == gd.turn && gd.board[4] == gd.turn &&
          gd.board[7] == gd.turn ||
      gd.board[2] == gd.turn && gd.board[5] == gd.turn &&
          gd.board[8] == gd.turn ||
      gd.board[0] == gd.turn && gd.board[4] == gd.turn &&
          gd.board[8] == gd.turn ||
      gd.board[2] == gd.turn && gd.board[4] == gd.turn &&
          gd.board[6] == gd.turn) {
    return 0;
  }
  return 1;
}

int checkTie() {
  int isTie = 0;
  for (int i = 0; i < sizeof(gd.board); i++) {
    if (gd.board[i] != ' ') {
      isTie++;
    }
  }
  if (isTie == 9) {
    return 0;
  }
  return 1;
}

int won() {
  std::cout << gd.turn << " won!\n";
  return 0;
}

int tie() {
  std::cout << "It's a tie!\n";
  return 0;
}

int printBoard() {
  for (int i = 0; i < sizeof(gd.board); i++) {
    std::cout << gd.board[i];
    if (i == 2 || i == 5 || i == 8) {
      std::cout << "\n";
    }
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  /// server
  server();

  waitForConnection();
  // waitForConnection();

  bool invalidMove = true;
  gd.done = false;
  gd.turn = 'X';
  while (!gd.done) {
    printBoard();
    std::string tmp;
    for (int i = 0; i < sizeof(gd.board); i++) {
      tmp += gd.board[i];
    }
    std::cout << tmp;

    tmp = "[BOARD] " + tmp;
    
    std::string str;
    char *writable = new char[tmp.size() + 1];
    std::copy(tmp.begin(), tmp.end(), writable);
    writable[tmp.size()] = '\0'; // don't forget the terminating 0

    send(writable);

    // don't forget to free the string after finished using it
    delete[] writable;

    gd.done = true;

    // move();
    // int err = checkWin();
    // if (err == 0) {
    //   won();
    //   return 0;
    // }
    // err = checkTie();
    // if (err == 0) {
    //   tie();
    //   return 0;
    // }
    // switchTurn();
  }
  // closing the connected socket
  close(sd.sockets[0]);
  // closing the listening socket
  shutdown(sd.server_fd, SHUT_RDWR);
  return 0;
}
