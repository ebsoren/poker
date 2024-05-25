#include <cassert>
#include <iostream>
#include <memory>

#include "player.hpp"
using namespace std;

void testPlayerConstructor() {
  Human* player = new Human("Esben", "Human", 100);
  Card* card1 = new Card(0, 2);
  Card* card2 = new Card(3, 14);
  player->setHandFirst(*card1);
  player->setHandSecond(*card2);
  std::cout << player->getName() << " has the "
            << player->getHand().first.printCard() << " and the "
            << player->getHand().second.printCard() << ". Their stack is "
            << player->getStack() << " dollars.\n";
}

void testPlayerDecision() {
  Human* player = new Human("Esben", "Human", 100);
  Card* card1 = new Card(0, 2);
  Card* card2 = new Card(3, 14);
  player->setHandFirst(*card1);
  player->setHandSecond(*card2);
  int val = 20;
  player->playTurn(val);
}

int main() {
  testPlayerConstructor();
  testPlayerDecision();

  std::cout << "All tests executed successfully." << std::endl;
  return 0;
}
