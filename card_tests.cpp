#include <cassert>
#include <iostream>

#include "card.hpp"

void testCardConstructor() {
  Card card(0, 2);
  assert(card.getSuit() == 0 && card.getRank() == 2);
  std::cout << "Constructor test passed." << std::endl;
}

void testCardCopyConstructor() {
  Card original(1, 14);
  Card copy = original;
  assert(copy.getSuit() == 1 && copy.getRank() == 14);
  std::cout << "Copy constructor test passed." << std::endl;
}

void testCardPrint() {
  Card ace_clubs(0, 14);
  Card two_hearts(3, 2);
  Card seven_diamonds(2, 7);
  Card jack_spades(1, 11);

  std::cout << "Expected card list:\nACE of CLUBS\nTWO of HEARTS\nSEVEN of "
               "DIAMONDS\nJACK of SPADES\n\nActual card list:\n"
            << ace_clubs.printCard() << "\n" << two_hearts.printCard()
            <<  "\n" << seven_diamonds.printCard() <<  "\n" << jack_spades.printCard() << "\n\n";
}

int main() {
  testCardConstructor();
  testCardCopyConstructor();
  testCardPrint();
  // Add more tests as necessary

  std::cout << "All tests executed successfully." << std::endl;
  return 0;
}
