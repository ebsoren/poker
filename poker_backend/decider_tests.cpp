#include <iostream>
#include <vector>

#include "decider.hpp"  // Assuming decider.hpp defines the Decider class, Card, Hand classes
#include "player.hpp"  // Assuming player.hpp might define the Player and related logic if used

using namespace std;

void deciderTest(const vector<Card>& cards,
                 const vector<pair<Card, Card>>& hands,
                 const string& testname) {
  Decider decider(hands, cards);

  auto winnerData = decider.determineWinner();

  cout << testname << ":\n";
  decider.printWinningHand(winnerData.second.first,
                           winnerData.second.second[winnerData.first.second]);
  cout << "\n\n";
}

int main() {
  vector<string> testnames = {"High Card Test",      "One Pair Test",
                              "Two Pair Test",       "Three of a Kind Test",
                              "Straight Test",       "Flush Test",
                              "Full House Test",     "Four of a Kind Test",
                              "Straight Flush Test", "Royal Flush Test"};
  vector<vector<Card>> all_Cards = {
      // High Card Test
      {Card(ACE, HEARTS), Card(KING, HEARTS), Card(QUEEN, HEARTS),
       Card(JACK, HEARTS), Card(TEN, SPADES)},
    
      {Card(FOUR, HEARTS), Card(FOUR, SPADES), Card(NINE, CLUBS),
       Card(JACK, DIAMONDS), Card(ACE, HEARTS)},
      // Two Pair Test
      {Card(THREE, HEARTS), Card(THREE, DIAMONDS), Card(FIVE, CLUBS),
       Card(FIVE, SPADES), Card(KING, HEARTS)},
      // Three of a Kind Test
      {Card(SIX, HEARTS), Card(SIX, DIAMONDS), Card(SIX, CLUBS),
       Card(TEN, SPADES), Card(QUEEN, HEARTS)},
      // Straight Test
      {Card(NINE, HEARTS), Card(TEN, DIAMONDS), Card(JACK, CLUBS),
       Card(QUEEN, SPADES), Card(KING, HEARTS)},
      // Flush Test
      {Card(TWO, SPADES), Card(FIVE, SPADES), Card(SEVEN, SPADES),
       Card(JACK, SPADES), Card(ACE, SPADES)},
      // Full House Test
      {Card(EIGHT, DIAMONDS), Card(EIGHT, HEARTS), Card(EIGHT, CLUBS),
       Card(KING, SPADES), Card(KING, HEARTS)},
      // Four of a Kind Test
      {Card(QUEEN, DIAMONDS), Card(QUEEN, HEARTS), Card(QUEEN, CLUBS),
       Card(QUEEN, SPADES), Card(ACE, HEARTS)},
      // Straight Flush Test
      {Card(NINE, CLUBS), Card(TEN, CLUBS), Card(JACK, CLUBS),
       Card(QUEEN, CLUBS), Card(KING, CLUBS)},
      // Royal Flush Test
      {Card(TEN, HEARTS), Card(JACK, HEARTS), Card(QUEEN, HEARTS),
       Card(KING, HEARTS), Card(ACE, HEARTS)}};

  vector<vector<pair<Card, Card>>> all_Hands = {

      {{Card(ACE, SPADES), Card(THREE, SPADES)},
       {Card(SIX, HEARTS), Card(EIGHT, DIAMONDS)}},

      {{Card(TEN, CLUBS), Card(SEVEN, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(FIVE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}},

      {{Card(TWO, SPADES), Card(THREE, SPADES)},
       {Card(TWO, SPADES), Card(THREE, SPADES)}}

  };

  for (int i = 0; i < testnames.size(); ++i) {
    deciderTest(all_Cards[i], all_Hands[i], testnames[i]);
  }

  return 0;
}
