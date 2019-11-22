#include <iostream>
#include "mineField.h"
#include "mineGame.h"

using std::cin;
using std::cout;
using std::endl;

void mine::Game::run() {
  while (m_page != Page::Exit) {
    Page next_page;
    if (m_page == Page::Start) {
      next_page = run_start_page();
    } else if (m_page == Page::Setting) {
      next_page = run_setting_page();
    } else if (m_page == Page::GameRun) {
      next_page = run_game_run_page();
    } else if (m_page == Page::GameEnd) {
      next_page = run_game_end_page();
    }
    m_page = next_page;
  }
}

mine::Game::Page mine::Game::run_start_page() {
  cout << "0: start\n"
          "1: setting\n"
          "2: exit\n";

  int action;
  cin >> action;

  if (action == 0) return Page::GameRun;
  if (action == 1) return Page::Setting;
  if (action == 2) return Page::Exit;

  return Page::Exit;
}

mine::Game::Page mine::Game::run_setting_page() {
  cout << "Which mode do you want to choose: \n"
          "0: easy\n"
          "1: medium\n"
          "2: hard\n";

  int mode;
  cin >> mode;

  // do some setting
  switch (mode) {
    case 0:
      m_row = 9;
      m_col = 9;
      m_density = 0.12;
      break;
    case 1:
      m_row = 9;
      m_col = 9;
      m_density = 0.16;
      break;
    case 2:
      m_row = 9;
      m_col = 9;
      m_density = 0.20;
      break;
    default:
      cout << "invalid mode, just using easy mode\n";
      m_row = 9;
      m_col = 9;
      m_density = 0.12;
  }

  return Page::Start;
}

mine::Game::Page mine::Game::run_game_run_page() {
  mineField minefield(m_row, m_col, m_density);
  while (true) {
    minefield.show();
    cout << "please input the address you want to obeserve:" << endl;
    int row;
    int col;
    cin >> row >> col;
    if (!minefield.another_update(row, col)) {
      return Page::GameEnd;
    }

    cout << "<=========我是分割线=========>" << endl << endl;

    if (minefield.is_succeed()) {
      cout << "congradulations for your success!!!" << endl;
      return Page::GameEnd;
    }
  }
}

mine::Game::Page mine::Game::run_game_end_page() {
  while (true) {
    cout << "Do you want to continue?(Y/N)";

    char receive_flag;
    cin >> receive_flag;

    if (receive_flag == 'Y') return Page::Start;
    if (receive_flag == 'N') return Page::Exit;

    cout << "invalid input" << endl;
  }
}
