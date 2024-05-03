#include "player.hpp"

#include <iostream>

using namespace std;

Player::~Player() {}

Human::Human(const std::string &name, const std::string &type, int stack)
    : Player(name, type, stack) {}

void Human::playTurn(int &currentBet, int &pot, int &num_players,
                     int &num_raises) {
  string decision;
  if (has_folded_func()) {
    return;
  }

  if (currentBet == 0) {
    cout << getName() << ":\nYour cards are: \n"
         << getHand().first.printCard() << "\n"
         << getHand().second.printCard() << "\nDo you check or bet?\n";
    cin >> decision;

    if (decision == "bet") {
      cout << "How much?\n";
      cin >> currentBet;
      cout << "You bet " << currentBet << " dollars.\n";
      pot += currentBet;
      ++num_raises;
      setBet(currentBet);

    } else if (decision == "check") {
      cout << "you checked\n";
    }
    return;
  }

  cout << getName() << ": The amount needed to call is "
       << currentBet - getBet() << " dollars.\nThe pot size is " << pot
       << ".\nYour cards are: \n"
       << getHand().first.printCard() << "\n"
       << getHand().second.printCard() << "\nDo you fold, call, or raise?\n";
  cin >> decision;
  if (decision == "fold") {
    cout << "you folded\n";
    has_folded_setter();
    --num_players;

    return;
  } else if (decision == "call") {
    cout << "you called " << currentBet << " dollars\n";
    pot += currentBet;
    if (num_raises > 0) {
      --num_raises;
    }

    setBet(currentBet);
    return;
  } else if (decision == "raise") {
    int raiseValue;
    cout << "How much do you want to bet?\n";
    cin >> raiseValue;

    cout << "you raised " << raiseValue << " dollars\n";

    pot += raiseValue;
    setBet(raiseValue);
    currentBet = raiseValue;
    ++num_raises;
    return;
  } else {
    cout << "need an error exception or something here lol\n";
    return;
  }
}
// incomplete
void Bot::playTurn(int &currentBet, int &pot, int &num_players,
                   int &num_raises) {
  cout << "bot folded\n";
}
