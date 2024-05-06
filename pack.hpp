#ifndef PACK_HPP
#define PACK_HPP

#include <vector>
#include "card.hpp"



class Pack {
 private:
  std::vector<Card> cards;  // Stores the cards in the pack
  int num_dealed = 0;       // counts how many cards have been dealed

 public:
  // Default constructor to initialize a standard pack of cards
  Pack();

  // Shuffles the cards in the pack
  void shuffle_pack();

  // Deals one card from the top of the pack
  Card dealCard();

  // Burns a card from the top of the pack.
  void burnCard();

  // Resets the pack with a complete set of cards
  void resetPack();

  // Destructor
  ~Pack();
};

#endif  // PACK_HPP
