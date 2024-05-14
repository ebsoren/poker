#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <utility>

#include "card.hpp"

class Player {
 public:
  // player constructor. sets name, type, and stack, but not hand
  Player(const std::string& name, const std::string& type, int stack)
      : name(name), type(type), stack(stack) {}

  // destructor
  virtual ~Player();

  // sets first card. separate functions because cards are dealt separately.
  virtual void setHandFirst(Card card) { hand.first = card; }
  // sets second card. separate functions because cards are dealt separately.
  virtual void setHandSecond(Card card) { hand.second = card; }

  // returns player hand
  virtual const std::pair<Card, Card>& getHand() const { return hand; };

  // virtual function. Implemented separately for different types of player.
  virtual void play(int& currentBet, int& pot, int& num_players,
                    int& previous_raise, std::pair<bool, int>& bigBlindRaised,
                    std::string& player_in) = 0;

  virtual void addStack(int val) { stack += val; }

  // returns player stack.
  virtual int getStack() const { return stack; }

  // returns whether player has folded
  virtual bool has_folded_func() { return has_folded; }

  // returns whether player is all in
  virtual bool is_all_in_func() { return is_all_in; }

  // sets fold status
  virtual void has_folded_setter() { has_folded = true; }

  // sets all in status
  virtual void all_in_setter() { is_all_in = true; }

  // returns player name
  virtual const std::string& getName() const { return name; }

  // returns player's current bet
  virtual const int& getBet() const { return bet; }
  virtual void setBet(int in) { bet = in; }

  // returns total bet in this round of betting
  // NOT tracked across preflop, flop, river, etc.
  virtual const int& getRoundBet() const { return round_bet; }

  // returns total bet tracked ACROSS preflop, river, etc.
  virtual const int& getTotalBet() const { return total_bet; }

  virtual const int& getBigBlind() const { return bigBlind; }
  virtual const int& getSmallBlind() const { return smallBlind; }

  virtual const int& getIsBlind() const { return isBlind; }
  virtual void setIsBlind(int val_in) { isBlind = val_in; }

  virtual void setPrevPlayer(std::string player_in) { prev_player = player_in; }
  virtual const std::string& getPrevPlayer() const { return prev_player; }

  std::pair<Card, Card> getHand() { return hand; }

  // maintains the correct value of the player's bets and stack. "bet" is the
  // current amount of money going into the pot. "round_bet" is the total money
  // paid this round. stack is the player's total amount of money.
  virtual const void setValues(int bet_in) {
    bet = bet_in;
    stack -= bet_in;
    round_bet += bet_in;
    if (bet_in == 0) {
      round_bet = 0;  // when new round of betting has started, reset round_bet
    }
    total_bet += bet_in;
  }

  virtual void reset() {
    has_folded = false;
    is_all_in = false;
    bet = 0;
    round_bet = 0;
    total_bet = 0;
  }

 private:
  std::pair<Card, Card> hand = {Card(TWO, CLUBS),
                                Card(TWO, SPADES)};  // player's hand
  int stack = 0;                                     // player's stack
  bool has_folded = false;                           // tracks fold status
  bool is_all_in = false;                            // tracks all in status

  int smallBlind = 20;
  int bigBlind = 40;
  int isBlind = 0;  // 0 if no blind; 1 if small blind; 2 if big blind; 3 is big
                    // blind already bet; used to check if big blind player has
                    // the right to check/raise if nobody raised the big blind.

  int bet = 0;        // current bet (tracked in case of reraise)
  int round_bet = 0;  // bet by round, including what was bet before a reraise
  int total_bet = 0;  // total bet over a game
  std::string name;   // player's name
  std::string type;   // human or bot
  std::string prev_player;  // previous player's name
};

class Bot : public Player {
 public:
  using Player::Player;
  void play(int& currentBet, int& pot, int& num_players, int& previous_Raise,
            std::pair<bool, int>& bigBlindRaised,
            std::string &player_in) override;
};

class Human : public Player {
 public:
  Human(const std::string& name, const std::string& type, int stack);

  // runs through the blinds round of betting.
  bool handleBlinds(int& currentBet, int& pot, int& num_players,
                    std::pair<bool, int>& bigBlindRaised);

  // implementation of one turn for a player in any situation or round.
  // currentBet is used to pass in a currentBet round member variable to track
  // it through turns. pot tracks the total pot size for a round. prevRaise is a
  // less important variable which is used to calculate the minimum raise.
  void play(int& currentBet, int& pot, int& num_players, int& previous_raise,
            std::pair<bool, int>& bigBlindRaised,
            std::string& player_in) override;

  // prints interface data for players, including name, stack size, and hand.
  void printPlayerData() const;

  void handleAllInDecision(int& currentBet, int& pot);
  void handleBetDecision(int& currentBet, int& pot);
  void handleCallDecision(int& currentBet, int& pot);
  void handleRaiseDecision(int& currentBet, int& raise_value, int& pot);

  // reads in bet/raise values and ensures they are numbers.
  void readInput(int& readValue, int& pVal);

  // checks that the bet input is less than or equal to the player's total stack
  // size and, in the case of a previous player raising, at least the minimum
  // raise allowed by hold'em rules. Called in the readInput function.
  bool sanitizeBetInput(int& val_in, int& stack_val, int& raise_val,
                        bool isRaise);
};

#endif