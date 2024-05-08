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
                    int& previous_raise, bool &blindRaise) = 0;

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
  virtual void resetBet() { bet = 0; }


  // returns player name
  virtual const std::string& getName() const { return name; }

  // returns player's current bet
  virtual const int& getBet() const { return bet; }

  // returns total bet in this round of betting
  // NOT tracked across preflop, flop, river, etc.
  virtual const int& getRoundBet() const { return round_bet; }
  virtual const int& getTotalBet() const { return total_bet; }
  virtual const int& getBlind() const { return blind; }

  virtual const int& getBigBlind() const { return bigBlind; }
  virtual const int& getSmallBlind() const { return smallBlind; }

 



  void setBlind(int b) { blind = b; }

  std::pair<Card, Card> getHand() { return hand; }

  // maintains the correct value of the player's bets and stack. "bet" is the
  // current amount of money going into the pot. "round_bet" is the total money
  // paid this round. stack is the player's total amount of money.
  virtual const void setBets_Stack(int bet_in) {
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
    blind = 0;
  }

 private:
  std::pair<Card, Card> hand = {Card(TWO, CLUBS),
                                Card(TWO, SPADES)};  // player's hand
  int stack = 0;                                     // player's stack
  bool has_folded = false;                           // tracks fold status
  bool is_all_in = false;                            // tracks all in status
  int blind = 0;  // 0 is no blind; 1 is small blind; 2 is big blind

  int smallBlind = 20;
  int bigBlind = 40;

  int bet = 0;        // current bet (tracked in case of reraise)
  int round_bet = 0;  // bet by round, including what was bet before a reraise
  int total_bet = 0;  // total bet over a game
  std::string name;   // player's name
  std::string type;   // human or bot
};

class Bot : public Player {
 public:
  using Player::Player;
  void play(int& currentBet, int& pot, int& num_players,
            int& previous_Raise, bool &blindRaise) override;
};

class Human : public Player {
 public:
  Human(const std::string& name, const std::string& type, int stack);

  // implementation of one turn for a player in any situation or round.
  // currentBet is used to pass in a currentBet round member variable to track
  // it through turns. pot tracks the total pot size for a round. prevRaise is a
  // less important variable which is used to calculate the minimum raise.
  void play(int& currentBet, int& pot, int& num_players,
            int& previous_raise, bool &blindRaise) override;

  // prints interface data for players, including name, stack size, and hand.
  void printPlayerData() const;

  // bet or check human interface implementation. Implements the human interface
  // in the case of this decision and tracks variables that need to be updated
  // depending on the decision made.
  void bet_or_check(std::string& decision, int& pot, int& currentBet);

  // reads in bet/raise values and ensures they are numbers.
  void readInput(int& readValue, int& pVal);

  // checks that the bet input is less than or equal to the player's total stack
  // size and, in the case of a previous player raising, at least the minimum
  // raise allowed by hold'em rules. Called in the readInput function.
  bool checkBetInput(int& val_in, int& stack_val, int& raise_val, bool isRaise);
  bool blinds(int &pot, int &currentBet, int &prevRaise);
};

#endif