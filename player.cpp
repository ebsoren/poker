#include "player.hpp"

#include <iostream>

using namespace std;

Player::~Player() {}

Human::Human(const std::string& name, const std::string& type, int stack)
    : Player(name, type, stack) {}

void Human::playTurn(int &currentBet, bool is_preflop) {
  string decision;
  if(is_preflop){
    cout << " need design for preflop\n";
    currentBet = 20;
    return;
  }
  cout << "The current bet is " << currentBet << " dollars. \nYour cards are: \n"
       << getHand().first.printCard() << "\n"
       << getHand().second.printCard() << "\nDo you fold, call, or raise?\n";
  cin >> decision;
  if (decision == "fold") {
    // do something
    setBetSize(0);
    cout << "you folded\n";
    return;

  } else if (decision == "call") {
    setBetSize(currentBet);
    cout << "you called " << getBet() << " dollars\n";
    return;

  } else {
    int raiseValue;
    cout << "How much do you want to bet?\n";
    cin >> raiseValue;
    setBetSize(raiseValue);
    cout << "you raised " << getBet() << " dollars\n";
    currentBet = getBet();
    return;
  }
}

//incomplete
void Bot::playTurn(int &currentBet, bool is_preflop){
    setBetSize(0);
    cout << "bot folded\n";
}
