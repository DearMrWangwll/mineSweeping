#pragma once
namespace mine {
class Game {
 public:
  enum class Page { Start, Setting, GameRun, GameEnd, Exit };

 public:
  void run();

 private:
  Page run_start_page();
  Page run_setting_page();
  Page run_game_run_page();
  Page run_game_end_page();

 private:
  Page m_page{Page::Start};

  int m_row{9};
  int m_col{9};
  double m_density{0.12};
};
}  // namespace mine
