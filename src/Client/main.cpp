#include <iostream>
#include <stdio.h>
#include <string>


class GameData{
public:
  bool done;
  char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
  std::string in;
  char turn;
};


GameData      gd;

int move(){
  if(gd.board[std::stoi(gd.in)] != ' '){
    return 1;
  }

  gd.board[std::stoi(gd.in)] = gd.turn;
  return 0;
}

int main(int argc, char *argv[]){

  bool invalidMove = true;
  gd.done = false;
  gd.turn = 'X';
  while(!gd.done){
    for(int i=0;i<sizeof(gd.board);i++){
      std::cout << gd.board[i];
      if(i==2||i==5||i==8){
	std::cout << "\n";
      }
    }
    invalidMove = true;
    while(invalidMove){
    std::cout << "It's your turn " << gd.turn << ": ";
    std::cin >> gd.in;
    gd.in = (std::to_string(((std::stoi(gd.in))-1)));
    int err = move();
    if(err != 0){
      std::cout << "Error while making move: " << err << "\n";
      switch (err) {
      case 1:
	std::cout << "You made an invalid move... Try again.\n";
      }
    }else{
      invalidMove = false;
    }
    }
  }
  return 0;
}
