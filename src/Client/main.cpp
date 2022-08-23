#include <iostream>
#include <string>

class GameData{
public:
  bool done;
  char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
  std::string in;
  char turn;
};

int move(GameData gd){
  return 0;
}

int main(int argc, char *argv[]){
  GameData      gd;

  gd.done = false;
  gd.turn = 'X';
  while(!gd.done){
    for(int i=0;i<sizeof(gd.board);i++){
      std::cout << gd.board[i];
      if(i==2||i==5||i==8){
	std::cout << "\n";
      }
    }
    std::cout << "It's your turn " << gd.turn << ": ";
    std::cin >> gd.in;
    int err = move(gd);
    if(err != 0){
      std::cout << "Error while making move: " << err << "\n";
    }
  }
  return 0;
}
