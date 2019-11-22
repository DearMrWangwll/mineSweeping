#pragma once

class mineField {
 public:
  // ctor & dtor
  mineField(int theRow, int theCol, double the_density_of_boom);
  ~mineField();

 private:
  // initialize map
  void init_outer_map();
  void init_mine_map();
  void init_inner_map();
  void init_zero_flag();

 public:
  // accessor
  int getRow() const { return m_row; }
  int getCol() const { return m_col; }
  int getNumOfMineAround(int theRow, int theCol) const;

  // updater(so cute)
  bool update(int theRow, int theCol);
  bool other_update(int theRow, int theCol);
  bool another_update(int theRow, int theCol);

  // show
  void show() const;
  void show_after_boom(int theRow, int theCol) const;

  // judge
  bool is_succeed() const;

 private:
  bool address_legal(int theRow, int theCol);

  void get_rect(int theRow, int theCol, int &left, int &right, int &up,
                int &down) const;

 private:
  int m_row;
  int m_col;
  double m_density;

  int **m_inner_map;
  char **m_outer_map;
  int **m_mine_map;

  bool **m_zero_flag;
};