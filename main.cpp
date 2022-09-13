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
  char board[9] = {'#', '#', '#', '#', '#', '#', '#', '#', '#'};
  std::string in;
  char turn;
};

class ServerData {
public:
  int server_fd, valread;
  int sockets[2];
  int socketTurn;

  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char type[1024] = {0};
  char c_msg[1024] = {0};
  std::string strType;
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
  return 0;
}

int waitForConnection(){
  int socket;
  if ((socket = accept(sd.server_fd, (struct sockaddr *)&sd.address,
                              (socklen_t *)&sd.addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return socket;
}

int read(int socket){
  int err = read(socket, sd.buffer, 1024);
  printf("%s\n", sd.buffer);
  return err;

}

int read(){
  int err = read(sd.sockets[sd.socketTurn], sd.buffer, 1024);
  bool done = false;
  int i = 0;
  while(!done){
    if(sd.buffer[i] == ' '){

      for(int j = i; j < strlen(sd.buffer); j++){
	sd.c_msg[j-i] = sd.buffer[j+1];
      }
      break;
    }
    sd.type[i] = sd.buffer[i];
    sd.strType += sd.buffer[i];
    i++;
  }

  printf("%s\n", sd.buffer);
}

int send(char* message){
  int err = send(sd.sockets[sd.socketTurn], message, strlen(message), 0);
  printf("Hello message sent\n");
  return err;
}


// functions

int switchTurn() {
  if (gd.turn == 'X') {
    gd.turn = 'O';
  } else {
    gd.turn = 'X';
  }
  if (sd.socketTurn == 0) {
    sd.socketTurn = 1;
  } else {
    sd.socketTurn = 0;
  }
  return 0;
}
int makeMove() {
  if (gd.board[std::stoi(sd.c_msg)] != '#') {
    return 1;
  }

  gd.board[std::stoi(sd.c_msg)] = gd.turn;
  return 0;
}

int move() {
  bool invalidMove = true;
  send("[MOVE] do");
  while (invalidMove) {
    read()
    std::cin >> sd.c_msg;
    sd.c_msg = std::to_string(std::stoi(sd.c_msg) - 1);
    int err = makeMove();
    if (err != 0) {
      send("[!MOVE] " + err)
    } else {
      invalidMove = false;
    }
  }

  send("[MOVE] done")
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
  sd.sockets[0] = waitForConnection();
  sd.sockets[1] = waitForConnection();

  bool invalidMove = true;
  gd.done = false;
  gd.turn = 'X';
  sd.socketTurn = 0;
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
    // if(){
    //   gd.done = true;
    //   break;
    // }

    // don't forget to free the string after finished using it
    delete[] writable;

    move();
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
    switchTurn();
  }
  // closing the connected socket
  close(sd.sockets[0]);
  // closing the listening socket
  shutdown(sd.server_fd, SHUT_RDWR);
  return 0;
}
