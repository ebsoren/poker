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

  virtual void playTurn(int &currentBet, bool is_preflop) = 0;

  virtual void setBetSize(int inBet) { bet = inBet; }
  virtual int getBet() { return bet; }
  virtual int getStack() { return stack; }

  virtual const std::string& getName() { return name; }

 private:
  std::pair<Card, Card> hand;
  int stack = 0;
  int bet = 0;

  std::string name;
  std::string type;
};

class Bot : public Player {
 public:
  using Player::Player;                    
  void playTurn(int &currentBet, bool is_preflop) override;  
};

class Human : public Player {
 public:
  Human(const std::string& name, const std::string& type, int stack);
  void playTurn(int &currentBet, bool is_preflop) override;
};

#endif