#include <time.h>
#include <iostream>
#include <queue>
#include <random>
#include "mineField.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

enum class Color {
  BLACK = 0X0000,
  DARK_BLUE = 0X0010,
  DARK_GREEN = 0X0020,
  DARK_CYAN = DARK_BLUE | DARK_GREEN,
  DARK_RED = 0X0040,
  DARK_MAGENTA = DARK_BLUE | DARK_RED,
  DARK_YELLOW = DARK_GREEN | DARK_RED,
  DARK_WHITE = DARK_BLUE | DARK_GREEN | DARK_RED,
  GRAY = 0X0080 | BLACK,
  BLUE = 0X0080 | DARK_BLUE,
  GREEN = 0X0080 | DARK_GREEN,
  CYAN = 0X0080 | DARK_CYAN,
  RED = 0X0080 | DARK_RED,
  MAGENTA = 0X0080 | DARK_MAGENTA,
  YELLOW = 0X0080 | DARK_YELLOW,
  WHITE = 0X0080 | DARK_WHITE,
};

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
Color colors[] = {
    Color::WHITE,       // 0
    Color::GREEN,       // 1
    Color::DARK_GREEN,  // 2
    Color::BLUE,        // 3
    Color::RED,         // 4
    Color::RED,         // 5
    Color::RED,         // 6
    Color::RED,         // 7
    Color::RED,         // 8
};

template <typename T>
T** allocT(int row, int col) {
  T** arr = new T*[row];
  for (int i = 0; i < row; ++i) {
    arr[i] = new T[col];
  }
  return arr;
}

template <typename T>
void freeT(T** arr, int row, int col) {
  for (int i = 0; i < row; ++i) {
    delete[] arr[i];
  }
  delete[] arr;
}

mineField::mineField(int theRow, int theCol, double the_density_of_boom)
    : m_row(theRow),
      m_col(theCol),
      m_density(the_density_of_boom),
      m_inner_map(allocT<int>(theRow, theCol)),
      m_outer_map(allocT<char>(theRow, theCol)),
      m_mine_map(allocT<int>(theRow, theCol)),
      m_zero_flag(allocT<bool>(theRow, theCol)) {
  srand(time(NULL));

  init_outer_map();
  init_mine_map();
  init_inner_map();
  init_zero_flag();
}

mineField::~mineField() {
  freeT(m_inner_map, m_row, m_col);
  freeT(m_outer_map, m_row, m_col);
  freeT(m_mine_map, m_row, m_col);
  freeT(m_zero_flag, m_row, m_col);
}

void mineField::init_outer_map() {
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_col; ++j) {
      m_outer_map[i][j] = '*';
    }
  }
}

void mineField::init_mine_map() {
  for (int i = 0; i < m_row; i++) {
    for (int j = 0; j < m_col; ++j) {
      int rand1 = ((rand() % 100) > int(m_density * 100)) ? 0 : 1;
      m_mine_map[i][j] = rand1;  // 是地雷的地方为1
    }
  }
}

void mineField::init_inner_map() {
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_col; ++j) {
      m_inner_map[i][j] = getNumOfMineAround(i, j);  // 个数
    }
  }
}

void mineField::init_zero_flag() {
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_col; ++j) {
      m_zero_flag[i][j] = false;
    }
  }
}

int mineField::getNumOfMineAround(int theRow, int theCol) const {
  int count = 0;
  int left, right, up, down;
  get_rect(theRow, theCol, left, right, up, down);
  for (int i = left; i <= right; ++i) {
    for (int j = up; j <= down; ++j) {
      count += m_mine_map[i][j];
    }
  }
  return count;
}

bool mineField::update(int theRow, int theCol) {
  if (!address_legal(theRow, theCol)) {
    return true;
  }
  if (m_mine_map[theRow][theCol] == 1) {
    std::cout << "Boom! I am sorry that you failed!" << std::endl;
    return false;
  } else {
    m_outer_map[theRow][theCol] = m_inner_map[theRow][theCol] + '0';
    return true;
  }
}

bool mineField::other_update(int theRow, int theCol) {
  if (!address_legal(theRow, theCol)) {
    return true;
  }
  if (m_mine_map[theRow][theCol] == 1) {
    std::cout << "BOOM! I am sorry that you failed!" << std::endl;
    return false;
  } else {
    m_outer_map[theRow][theCol] = m_inner_map[theRow][theCol] + '0';
    if (m_inner_map[theRow][theCol] == 0) {
      // for (int i = left; i <= right; ++i) {
      //  for (int j = up; j <= down; ++j) {
      //    m_outer_map[i][j] = m_inner_map[i][j] + '0';
      //  }
      //}
    }
    return true;
  }
}

bool mineField::another_update(int theRow, int theCol) {
  if (!address_legal(theRow, theCol)) {
    return true;
  }

  if (m_mine_map[theRow][theCol] == 1) {
    std::cout << "BOOM! I am sorry that you failed!" << std::endl;
    show_after_boom(theRow, theCol);
    return false;
  }

  m_outer_map[theRow][theCol] = m_inner_map[theRow][theCol] + '0';

  // 遇到0就往外一圈
  if (m_inner_map[theRow][theCol] == 0) {
    std::queue<std::pair<int, int>> address_queue;
    // address_queue.push(std::pair<int, int>(theRow, theCol));
    address_queue.emplace(theRow, theCol);

    while (!address_queue.empty()) {
      auto p = address_queue.front();
      int x_row = p.first;
      int y_col = p.second;
      address_queue.pop();
      m_zero_flag[x_row][y_col] = true;

      int left, right, up, down;
      get_rect(x_row, y_col, left, right, up, down);
      for (int i = left; i <= right; ++i) {
        for (int j = up; j <= down; ++j) {
          m_outer_map[i][j] = m_inner_map[i][j] + '0';
          if (m_inner_map[i][j] == 0 && !m_zero_flag[i][j]) {
            // address_queue.push(std::pair<int, int>(i, j));
            address_queue.emplace(i, j);
          }
        }
      }
    }
  }
  return true;
}

void mineField::show() const {
  std::cout << '+' << ' ';
  for (int j = 0; j < m_col; ++j) {
    std::cout << j << ' ';
  }
  std::cout << std::endl;
  for (int i = 0; i < m_row; ++i) {
    std::cout << i << ' ';
    for (int j = 0; j < m_col; ++j) {
      if (m_outer_map[i][j] == '*') {
        SetConsoleTextAttribute(handle, static_cast<WORD>(colors[0]) >> 4);
      } else {
        int x = m_inner_map[i][j] % 8 + 1;
        SetConsoleTextAttribute(handle, static_cast<WORD>(colors[x]) >> 4);
      }
      std::cout << m_outer_map[i][j] << ' ';
    }
    std::cout << std::endl;
    SetConsoleTextAttribute(handle, static_cast<WORD>(colors[0]) >> 4);
  }
  /*for (int i = 0; i < row; ++i) {
          for (int j = 0; j < col; ++j) {
                  std::cout << mine_map[i][j] << '\t';
          }
          std::cout << std::endl;
  }*/
}

void mineField::show_after_boom(int theRow, int theCol) const {
  std::cout << '+' << ' ';
  for (int j = 0; j < m_col; ++j) {
    std::cout << j << ' ';
  }
  std::cout << std::endl;
  for (int i = 0; i < m_row; ++i) {
    std::cout << i << ' ';
    for (int j = 0; j < m_col; ++j) {
      if (m_outer_map[i][j] == '*' && (i != theRow || j != theCol)) {
        SetConsoleTextAttribute(handle, static_cast<WORD>(colors[0]) >> 4);
        std::cout << m_outer_map[i][j] << ' ';
      } else if (i != theRow || j != theCol) {
        int x = m_inner_map[i][j] % 8 + 1;
        SetConsoleTextAttribute(handle, static_cast<WORD>(colors[x]) >> 4);
        std::cout << m_outer_map[i][j] << ' ';
      } else if (i == theRow && j == theCol) {
        SetConsoleTextAttribute(handle, static_cast<WORD>(colors[8]) >> 4);
        std::cout << '@' << ' ';
      }
    }
    std::cout << std::endl;
    SetConsoleTextAttribute(handle, static_cast<WORD>(colors[0]) >> 4);
  }
}

bool mineField::is_succeed() const {
  int count = 0;
  for (int i = 0; i < m_row; ++i) {
    for (int j = 0; j < m_col; ++j) {
      if (m_outer_map[i][j] == '*' && m_mine_map[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

bool mineField::address_legal(int theRow, int theCol) {
  if (theRow > m_row - 1 || theCol > m_col - 1) {
    std::cout << "you can not choose this because it is out of range!"
              << std::endl;
    return false;
  }
  if (m_outer_map[theRow][theCol] != '*') {
    std::cout << "you can not choose this because it is has been choosen!"
              << std::endl;
    return false;
  }
  return true;
}

void mineField::get_rect(int theRow, int theCol, int& left, int& right, int& up,
                         int& down) const {
  left = (theRow > 0) ? theRow - 1 : theRow;
  right = (theRow < m_row - 1) ? theRow + 1 : theRow;
  up = (theCol > 0) ? theCol - 1 : theCol;
  down = (theCol < m_col - 1) ? theCol + 1 : theCol;
}
