#include "player.hpp"

#include <iostream>

using namespace std;

Player::~Player() {}

Human::Human(const std::string &name, const std::string &type, int stack)
    : Player(name, type, stack) {}

void Human::play(int &currentBet, int &pot, int &num_players, int &prevRaise, bool &blindRaise) {
  // Skip turn if player has folded or is all in
  if (has_folded_func() || is_all_in_func()) {
   
    return;
  }

  string decision;  // will be used to read in all inputs.

  if (currentBet != 0 && currentBet == getBet()) {

    if (getBlind() == 2) {
     

      cout << getName()
           << ", everyone called the big blind. Do you check or raise?\n";
      getline(cin, decision);
      while (decision != "raise" && decision != "check") {
        cout << "Please type 'check' or 'raise'. \n";
        getline(cin, decision);
      }

      if (decision == "raise") {
        cout << "How much do you want to raise? $" << 2 * currentBet - prevRaise
             << " minimum.\nTo go all in, bet $" << getStack() << ".\n";

        int raise_value;
        int check_value = 2 * currentBet - prevRaise;
        blindRaise = true;

        readInput(raise_value, check_value);
        if (raise_value == getStack()) {
          cout << getName() << " went all in.\n";
          all_in_setter();

        } else {
          cout << getName() << " raised $" << raise_value << ".\n";
          pot += raise_value;

          setBets_Stack(raise_value);
          currentBet = raise_value;
          setBlind(0);

              return;
        }

      } else {
        cout << getName() << " checked.\n";
      }
      setBlind(0);
      return;
    }
    return;
  }

  printPlayerData();  // simply prints an interface. No values changed.

  bool is_blind = blinds(pot, currentBet, prevRaise);
  if (is_blind) {
    return;
   
  }

  // if nobody has bet, call the bet_or_check decider function and end.
  if (currentBet == 0) {
 
    bet_or_check(decision, pot, currentBet);
    return;
  }

  // since currentBet is greater than 0, decide fold, call, or raise
if(blindRaise){
  resetBet();
}
  if (getStack() < (2 * currentBet - prevRaise)) {
    if (getStack() <= currentBet - getBet()) {
      cout << "Do you fold or go all in ($" << getStack() << ")?\n";
    } else {
      cout << "Do you fold, call ($" << currentBet - getBet()
           << "), or go all in ($" << getStack()
           << ")? You do not have enough to raise.\n";
    }
  } else {
    cout << "Do you fold, call ($" << currentBet - getBet() << "), raise ($"
         << 2 * currentBet - prevRaise << " minimum), or go all in ($"
         << getStack() << ")?\n";
  }

  getline(cin, decision);  // read in decision

  // ensures decision is a valid input
  while (decision != "raise" && decision != "fold" && decision != "call" &&
         decision != "all in") {
    cout << "Please type 'fold', 'call', 'raise', or 'all in'. \n";
    getline(cin, decision);
  }

  // fold block
  if (decision == "fold") {
    cout << getName() << " folded\n\n";

    has_folded_setter();
    --num_players;

    return;

    // call block
  } else if (decision == "call") {
    cout << getName() << " called $" << currentBet - getBet();

    if (currentBet - getBet() == getStack()) {
      cout << " and went all in";
      all_in_setter();
    }
    cout << ".\n";

    pot += currentBet - getBet();
    setBets_Stack(currentBet - getBet());
 

    return;
  }

  // raise block
  else if (decision == "raise") {
    cout << "How much do you want to raise? $" << 2 * currentBet - prevRaise
         << " minimum.\nTo go all in, bet $" << getStack() << ".\n";

    int raise_value;
    int check_value = 2 * currentBet - prevRaise;

    readInput(raise_value, check_value);
    if (raise_value == getStack()) {
      cout << getName() << " went all in.\n";
      all_in_setter();

    } else {
      cout << getName() << " raised $" << raise_value << ".\n";
    }

    pot += raise_value;

    setBets_Stack(raise_value);
    prevRaise = currentBet;
    currentBet = raise_value;

    return;
  }

  cout << getName() << " went all in.\n";
  pot += getStack();

  prevRaise = currentBet;
  currentBet = getStack();
  setBets_Stack(getStack());
  all_in_setter();

}  // END OF PLAY FUNCTION

bool Human::blinds(int &pot, int &currentBet, int &prevRaise) {
  if (getBlind() == 1) {
    cout << "You are the small blind and bet $" << getSmallBlind() << ".\n";
    if (getStack() == getSmallBlind()) {
      all_in_setter();
      cout << getName() << " is all in.\n";
    }
    pot += getSmallBlind();
    setBets_Stack(getSmallBlind());
    currentBet = getSmallBlind();
    setBlind(0);
    return true;
  }
  if (getBlind() == 2) {
    cout << "You are the big blind and bet $" << getBigBlind() << ".\n";
    if (getStack() == getBigBlind()) {
      cout << getName() << " is all in.\n";
      all_in_setter();
    }
    pot += getBigBlind();
    setBets_Stack(getBigBlind());
    prevRaise = currentBet;
    currentBet = getBigBlind();

    return true;
  }
  return false;
}

void Human::printPlayerData() const {
  cout << getName() << ": You have $" << getStack()
       << " in your stack.\nYour cards are: \n"
       << getHand().first.printCard() << "\n"
       << getHand().second.printCard() << "\n";
}

void Human::bet_or_check(string &decision, int &pot, int &currentBet) {
  cout << "Do you check or bet?\n";
  cin >> decision;

  while (decision != "check" && decision != "bet") {
    cout << "Please type 'check' or 'bet'. \n";
    cin >> decision;
  }

  if (decision == "bet") {
    cout << "\nHow much?\nTo go all in, bet " << getStack() << ".\n";
    int readVal;
    readInput(readVal, currentBet);

    if (readVal == getStack()) {
      cout << getName() << " went all in.\n";
      all_in_setter();
    } else {
      cout << getName() << " bet $" << readVal << ".\n\n";
    }
    pot += readVal;
    setBets_Stack(readVal);
    currentBet = readVal;
  } else if (decision == "check") {
    cout << getName() << " checked\n\n";
  }
}

void Human::readInput(int &readValue, int &raise_val) {
  bool isRaise = getBet() < raise_val;
  int stack_temp = getStack();
  while (true) {
    cin >> readValue;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Please type a number.\n";
    } else if (checkBetInput(readValue, stack_temp, raise_val, isRaise)) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }
}

bool Human::checkBetInput(int &val_in, int &stack_val, int &raise_val,
                          bool isRaise) {
  if (val_in > stack_val) {
    cout << "This value is larger than your stack value. Please try again.\n";
    return false;
  }
  if (val_in < raise_val && isRaise) {
    cout << "This value is less than the minimum raise value. Please try "
            "again.\n";
    return false;
  }
  return true;
}






// play for bot has not been implemented.
void Bot::play(int &currentBet, int &pot, int &num_players, int &prevRaise, bool &blindRaise) {
  cout << "bot folded\n";
}
