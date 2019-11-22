#include <iostream>
#include "mineField.h"
#include "mineGame.h"

int row = 9;
int col = 9;

bool is_failed = false;
bool game_end = false;
int mode;
int start_or_set;
double density_of_boom = 0.12;

int oldMain() {
  int row_x;
  int col_y;
  while (!game_end) {
    std::cout << "0: start" << std::endl << "1: setting" << std::endl;
    std::cin >> start_or_set;
    if (start_or_set == 1) {
      std::cout << "Which mode do you want to choose: " << std::endl
                << "0: easy" << std::endl
                << "1: medium" << std::endl
                << "2: hard" << std::endl;
      std::cin >> mode;
      switch (mode) {
        case 0:
          row = 9;
          col = 9;
          density_of_boom = 0.12;
          break;
        case 1:
          row = 9;
          col = 9;
          density_of_boom = 0.16;
          break;
        case 2:
          row = 9;
          col = 9;
          density_of_boom = 0.20;
          break;
      }
    }
    mineField minefield(row, col, density_of_boom);
    std::cout << density_of_boom << std::endl;
    while (!is_failed) {
      minefield.show();
      std::cout << "please input the address you want to obeserve:"
                << std::endl;
      std::cin >> row_x >> col_y;
      if (!minefield.another_update(row_x, col_y)) {
        break;
      }
      std::cout << "<=========我是分割线=========>" << std::endl << std::endl;
      if (minefield.is_succeed()) {
        std::cout << "congradulations for your success !!!" << std::endl; break;
      }
    }
    char continuing;
    while (true) {
      std::cout << "Do you want to continue?(T/F)";
      std::cin >> continuing;
      if (continuing == 'T') {
        game_end = false;
        density_of_boom = 0.12;
        break;
      } else if (continuing == 'F') {
        game_end = true;

        break;
      } else {
        std::cout << "invalid input" << std::endl;
      }
    }
  }

  system("pause");
  return 0;
}