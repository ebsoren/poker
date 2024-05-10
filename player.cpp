#include "player.hpp"

#include <iostream>

using namespace std;

// sanitizes string decisions (check, bet, fold, call, raise, all in)
void decisionSanitizer(string &decision, vector<string> choices) {
  cin >> ws;
  getline(cin, decision);
  bool decisionIsValid =
      std::find(choices.begin(), choices.end(), decision) != choices.end();
  while (!decisionIsValid) {
    cout << "Please type a valid choice. options are:\n";
    for (const string &choice : choices) {
      cout << choice << endl;
    }
    cout << endl;
    cin >> ws;
    getline(cin, decision);
  }
}

// START OF PLAYER FUNCTIONS

// destructor
Player::~Player() {}

// Human constructor
Human::Human(const std::string &name, const std::string &type, int stack)
    : Player(name, type, stack) {}

// runs through the first round of betting in which one-two players have to bet.
// moves the blinds around the table. Transpot

// runs through one decision for each player; player can either bet/check or do
// some version of fold, call, raise, or all in depending on the situation.
void Human::play(int &currentBet, int &pot, int &num_players, int &prevRaise) {
  // Skip turn if player has folded or is all in
  if ((has_folded_func() || is_all_in_func())|| num_players == 1 ||
      (currentBet != 0 && currentBet == getBet() && getIsBlind() == 0)) {
    return;
  }

  printPlayerData();  // Prints an interface. No values changed.
  bool blindAction = handleBlinds(currentBet, pot, num_players);
  if (blindAction) {
    return;
  }
  string decision;  // will be used to read in all inputs.

  // if nobody has bet, handle either a check or bet option
  if (currentBet == 0) {
    cout << "Do you check or bet?\n\n";
    decisionSanitizer(decision, {"check", "bet"});

    if (decision == "check") {
      cout << getName() << " checked\n\n";
    } else {
      handleBetDecision(currentBet, pot);
    }
    return;
  }

  // since currentBet is greater than 0, decide fold, call, or raise
  if (getStack() < (2 * currentBet - prevRaise)) {  // minimum raise
    if (getStack() <= currentBet - getBet()) {      // chips <= raise;
      cout << "Do you fold or go all in ($" << getStack() << ")?\n\n";
      decisionSanitizer(decision, {"fold", "all in"});
    } else {  // covers but cannot make minimum raise
      cout << "Do you fold, call ($" << currentBet - getBet()
           << "), or go all in ($" << getStack()
           << ")? You do not have enough to raise.\n\n";
      decisionSanitizer(decision, {"fold", "call", "all in"});
    }
  } else {  // covers and can make minimum raise
    cout << "Do you fold, call ($" << currentBet - getBet() << "), raise ($"
         << 2 * currentBet - prevRaise << " minimum), or go all in ($"
         << getStack() << ")?\n\n";
    decisionSanitizer(decision, {"fold", "call", "raise", "all in"});
  }

  // fold block
  if (decision == "fold") {
    cout << getName() << " folded\n\n";
    has_folded_setter();
    --num_players;

    // call block
  } else if (decision == "call") {
    handleCallDecision(currentBet, pot);
  }

  // raise block
  else if (decision == "raise") {
    handleRaiseDecision(currentBet, prevRaise, pot);
  }

  // all in block
  if (decision == "all in") {
    handleAllInDecision(currentBet, pot);
  }

}  // END OF PLAY FUNCTION

// prints hand and stack
void Human::printPlayerData() const {
  cout << getName() << ": You have $" << getStack()
       << " in your stack.\nYour cards are: \n"
       << getHand().first.printCard() << "\n"
       << getHand().second.printCard() << "\n";
}

// reads in integer bet values and calls sanitizer
void Human::readInput(int &readValue, int &raise_val) {
  bool isRaise = getBet() < raise_val;
  int stack_temp = getStack();
  while (true) {
    cin >> readValue;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Please type a number.\n\n";
    } else if (sanitizeBetInput(readValue, stack_temp, raise_val, isRaise)) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    }
  }
}

// ensures valid bet or raise
bool Human::sanitizeBetInput(int &val_in, int &stack_val, int &raise_val,
                             bool isRaise) {
  if (val_in <= 0) {
    cout << "Bet must be greater than zero.\n\n";
  }
  if (val_in > stack_val) {
    cout << "This value is larger than your stack value. Please try again.\n\n";
    return false;
  }
  if (val_in < raise_val && isRaise) {
    cout << "This value is less than the minimum raise value. Please try "
            "again.\n\n";
    return false;
  }
  return true;
}

// ensures valid bet and updates values
void Human::handleBetDecision(int &currentBet, int &pot) {
  cout << "\nHow much?\nTo go all in, bet " << getStack() << ".\n\n";
  int readVal;
  readInput(readVal, currentBet);

  if (readVal == getStack()) {
    cout << getName() << " went all in.\n\n";
    all_in_setter();
  } else {
    cout << getName() << " bet $" << readVal << ".\n\n";
  }
  pot += readVal;
  setValues(readVal);
  currentBet = readVal;
}

// calls and updates values
void Human::handleCallDecision(int &currentBet, int &pot) {
  cout << getName() << " called $" << currentBet - getBet();

  if (currentBet - getBet() == getStack()) {
    cout << " and went all in";
    all_in_setter();
  }
  cout << ".\n\n";

  pot += currentBet - getBet();
  setValues(currentBet - getBet());
}

// Ensures valid raise and updates values
void Human::handleRaiseDecision(int &currentBet, int &prevRaise, int &pot) {
  cout << "How much do you want to raise? $" << 2 * currentBet - prevRaise
       << " minimum.\nYour stack is $" << getStack() << ".\n\n";

  int raise_value;
  int check_value = 2 * currentBet - prevRaise;

  readInput(raise_value, check_value);
  if (raise_value == getStack()) {
    cout << getName() << " went all in.\n\n";
    all_in_setter();

  } else {
    cout << getName() << " raised $" << raise_value << ".\n\n";
  }

  pot += raise_value;
  setValues(raise_value);
  prevRaise = currentBet;
  currentBet = raise_value;
}

// sets player to all in and updates all relevant values
void Human::handleAllInDecision(int &currentBet, int &pot) {
  cout << getName() << " went all in.\n\n";
  all_in_setter();
  pot += getStack();
  setValues(getStack());
  currentBet = getStack();
}

bool Human::handleBlinds(int &currentBet, int &pot, int &num_players) {
  if (getIsBlind() == 1) {
    cout << "you are the small blind and bet $" << getSmallBlind() << ".\n\n";
    pot += getSmallBlind();
    setValues(getSmallBlind());
    currentBet = getSmallBlind();
    setIsBlind(0);
    return true;

  } else if (getIsBlind() == 2) {
    cout << "you are the big blind and bet $" << getBigBlind() << ".\n\n";
    pot += getBigBlind();
    setValues(getBigBlind());
    currentBet = getBigBlind();
    setIsBlind(3);
    return true;

  } else if (getIsBlind() == 3) {
    if (pot == getBigBlind() * num_players) {
      cout << "nobody raised the big blind. Do you check or bet?\n\n";
      string decision;
      decisionSanitizer(decision, {"check", "bet"});

      if (decision == "check") {
        cout << getName() << " checked\n\n";
      } else {
        handleBetDecision(currentBet, pot);
      }
      setIsBlind(0);
      return true;
    }
  }
  setIsBlind(0);
  return false;
}

// play for bot has not been implemented.
void Bot::play(int &currentBet, int &pot, int &num_players, int &prevRaise) {
  cout << "bot folded\n";
}
