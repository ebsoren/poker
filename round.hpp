#ifndef ROUND_HPP
#define ROUND_HPP

#include <iostream>
#include <vector>

#include "card.hpp"
#include "pack.hpp"
#include "player.hpp"

class Round {
 public:
  Round();
  Round(std::vector<std::unique_ptr<Player>>& players_in);

  void setup();

  void preflop();
  void flop();
  void turn_or_river();  // turn and river basically follow the same process.
  void checkWinner();
  void playTurnAll();
  int get_num_raises();
  void show_cards();


  void resetBets();

  ~Round();

 private:
  int pot = 0;
  int num_players = 0;
  int currentBet = 0;
  int num_raises = 0;
  std::vector<Card> community_cards;
  std::vector<std::unique_ptr<Player>>& players;
  Pack pack;
  bool river = false;
};

#endif