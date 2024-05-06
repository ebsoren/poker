#ifndef ROUND_HPP
#define ROUND_HPP

#include <iostream>
#include <vector>

#include "card.hpp"
#include "pack.hpp"
#include "player.hpp"

class Round {
 public:
  // idk why its making me define this
  Round();
  // actual constructor
  Round(std::vector<std::unique_ptr<Player>>& players_in);

  // destructor
  ~Round();

  // deals two cards to each player.
  void setup();

  // runs the preflop and checks for a winner. mostly taken care of by the
  // playAll function.
  void preflop();

  // runs the flop, turn, or river. Identical for each except for the string
  // printed to user (the current round) and the number of cards printed.
  // Deals 3 cards on the flop.
  void postflop(const std::string current_round);

  // checks whether anybody has won the round because everybody else has folded.
  // NOT used to determine who wins based on actual hands.
  void checkWinner();

  // used to determine whether it is time to move rounds (i.e from the flop to
  // the turn). Determines whether only one player remains, or if all players
  // still in the game have bet the same amount.
  // EX: p1 bets -> p2 raises -> p3 calls -> p4 folds -> p1 calls -> move on

  bool checkIfContinue();

  // runs through each player's turn until checkIfContinue evaluates to true, as
  // which point we move to the next round of betting.
  void playAll();

  // prints out players' cards after the final round of betting.
  bool show_cards();

  // resets each player's current bet, total bet, and the pot size.
  void resetBets();

  // adds a card to the community cards.
  void addCommunityCard();

  std::vector<std::pair<Card, Card>> getHands();

  std::vector<Card> getCommunityCards(){ return community_cards;}

  int getPot(){return pot;}


  void moveChips();

 private:
  int pot = 0;             // pot size
  int num_players = 0;     // number of players
  int currentBet = 0;      // current bet. used for calculations
  int previous_raise = 0;  // previous raise, used for calculations

  std::vector<Card> community_cards;              // flop, turn, river cards
  std::vector<std::unique_ptr<Player>>& players;  // player list
  Pack pack;                                      // pack used during the game
};

#endif