#include <iostream>
#include <string>

class GameData{
public:
  bool done;
  char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
  std::string in;
};

int main(int argc, char *argv[]){
  GameData      gd;

  gd.done = false;
  while(!gd.done){
    for(int i=0;i<sizeof(gd.board);i++){
      std::cout << gd.board[i];
      if(i==2||i==5||i==8){
	std::cout << "\n";
      }
    }
    std::cin >> gd.in;
  }
  return 0;
}
