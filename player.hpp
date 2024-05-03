#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <utility>

#include "card.hpp"

class Player {
 public:
  Player(const std::string& name, const std::string& type, int stack)
      : name(name), type(type), stack(stack) {}
  virtual ~Player();

  virtual const std::pair<Card, Card>& getHand() { return hand; };
  virtual void setHandFirst(Card card) { hand.first = card; }
  virtual void setHandSecond(Card card) { hand.second = card; }

  virtual void playTurn(int& currentBet, int& pot, int& num_players,
                        int& num_raises) = 0;

  virtual int getStack() { return stack; }

  virtual bool has_folded_func() { return has_folded; }
  virtual void has_folded_setter() { has_folded = true; }

  virtual const std::string& getName() { return name; }
  virtual const int& getBet() { return bet; }
  virtual const void setBet(int bet_in) { bet = bet_in; }

 private:
  std::pair<Card, Card> hand;
  int stack = 0;
  bool has_folded = false;
  int bet = 0;
  std::string name;
  std::string type;
};

class Bot : public Player {
 public:
  using Player::Player;
  void playTurn(int& currentBet, int& pot, int& num_players,
                int& num_raises) override;
};

class Human : public Player {
 public:
  Human(const std::string& name, const std::string& type, int stack);
  void playTurn(int& currentBet, int& pot, int& num_players,
                int& num_raises) override;
};

#endif