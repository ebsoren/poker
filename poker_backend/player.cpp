#include "player.hpp"

#include <iostream>

using namespace std;

// sanitizes string decisions (check, bet, fold, call, raise, all in)
void decisionSanitizer(string &decision_in, vector<string> choices) {
  cin >> ws;
  getline(cin, decision_in);
  bool decisionIsValid =
      find(choices.begin(), choices.end(), decision_in) != choices.end();
  while (!decisionIsValid) {
    cout << "Please type a valid choice. options are:\n";
    for (const string &choice : choices) {
      cout << choice << endl;
    }
    cout << endl;
    if (!cin) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin >> ws;
    getline(cin, decision_in);

    decisionIsValid =
        find(choices.begin(), choices.end(), decision_in) != choices.end();
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
pair<Decision, int> Player::decide(int &currentBet, int &num_players,
                                   int &prevRaise,
                                   pair<bool, int> &bigBlindRaised) {
  // Skip turn if player has folded or is all in

  if ((has_folded_func() || is_all_in_func()) || num_players == 1) {
    return {SKIP, 0};
  }

  if (bigBlindRaised.second == num_players - 1) {
    bigBlindRaised.first = false;
  }

  if (bigBlindRaised.first) {
    setBet(0);
    ++bigBlindRaised.second;
  }

  if ((currentBet != 0 && currentBet == getBet() && getIsBlind() == 0) ||
      (getHasActed() && prevRaise == 0 && getIsBlind() == 0 && getBet() == 0 &&
       currentBet == 0)) {
    return {NODECISION, 0};
  }

  setHasActed(true);
  printPlayerData();  // Prints an interface. No values changed.
  pair<int, bool> blindAction = handleBlinds(currentBet, bigBlindRaised);
  if (blindAction.second) {
    return {BLIND, blindAction.first};
  }

  // if nobody has bet, handle either a check or bet option
  if (currentBet == 0) {
    if (!wantsToBet()) {
      cout << getName() << " checked\n\n";
      return {CHECK, 0};
    } else {
      return {BET, handleBetDecision(currentBet)};
    }
  }

  // since currentBet is greater than 0, decide fold, call, or raise

  string decision_in;  // will be used to read in all inputs.
  decideFCRA(currentBet, prevRaise, decision_in);

  Decision decision;

  int bet_value;
  // fold block
  if (decision_in == "fold") {
    cout << getName() << " folded\n\n";
    has_folded_setter();
    decision = FOLD;
    bet_value = 0;

    // call block
  } else if (decision_in == "call") {
    decision = CALL;
    bet_value = handleCallDecision(currentBet);
  }

  // raise block
  else if (decision_in == "raise") {
    decision = RAISE;
    bet_value = handleRaiseDecision(currentBet, prevRaise);
  }

  // all in block
  if (decision_in == "all in") {
    decision = ALLIN;
    bet_value = handleAllInDecision();
  }
  return {decision, bet_value};
}  // END OF DECIDE FUNCTION

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
int Player::handleBetDecision(int &currentBet) {
  cout << "\nHow much?\nTo go all in, bet " << getStack() << ".\n\n";
  int readVal;
  readInput(readVal, currentBet);

  if (readVal == getStack()) {
    cout << getName() << " went all in.\n\n";
    all_in_setter();
  } else {
    cout << getName() << " bet $" << readVal << ".\n\n";
  }

  setValues(readVal);
  return readVal;
}

// calls and updates values
int Player::handleCallDecision(int &currentBet) {
  cout << getName() << " called $" << currentBet - getBet();

  if (currentBet - getBet() == getStack()) {
    cout << " and went all in";
    all_in_setter();
  }
  cout << ".\n\n";
  setValues(currentBet - getBet());
  return currentBet - getBet();
}

// Ensures valid raise and updates values
int Player::handleRaiseDecision(int &currentBet, int &prevRaise) {
  int raise_value = 0;
  decideRaiseValue(currentBet, prevRaise, raise_value);

  if (raise_value == getStack()) {
    cout << getName() << " went all in.\n\n";
    all_in_setter();

  } else {
    cout << getName() << " raised $" << raise_value << ".\n\n";
  }
  setValues(raise_value);
  return getRoundBet();
}

// sets player to all in and updates all relevant values
int Player::handleAllInDecision() {
  cout << getName() << " went all in.\n\n";
  all_in_setter();

  setValues(getStack());
  return getStack();
}

pair<int, bool> Player::handleBlinds(int &currentBet,
                                     pair<bool, int> &bigBlindRaised) {
  if (getIsBlind() == 1) {
    cout << getName() << " is the small blind and bets $" << getSmallBlind()
         << ".\n\n";

    setValues(getSmallBlind());
    currentBet = getSmallBlind();
    setIsBlind(0);
    return {currentBet, true};

  } else if (getIsBlind() == 2) {
    cout << getName() << " is the big blind and bets $" << getBigBlind()
         << ".\n\n";

    setValues(getBigBlind());
    currentBet = getBigBlind();
    setIsBlind(3);
    return {currentBet, true};

  } else if (getIsBlind() == 3) {
    if (currentBet == getBigBlind()) {
      cout << "nobody raised the big blind. ";
      if (!wantsToBet()) {
        cout << getName() << " checked\n\n";
      } else {
        currentBet = handleBetDecision(currentBet);
        bigBlindRaised.first = true;
      }
      setIsBlind(0);
      return {currentBet, true};
    }
  }
  setIsBlind(0);
  return {currentBet, false};
}

bool Human::wantsToBet() {
  string decision_in;
  cout << "Do you check or bet?\n\n";
  decisionSanitizer(decision_in, {"check", "bet"});

  if (decision_in == "bet") {
    return true;
  }
  return false;
}

bool Bot::wantsToBet() { return false; }

void Human::decideRaiseValue(int &currentBet, int &prevRaise,
                             int &raise_value) {
  cout << "How much do you want to raise? $"
       << 2 * currentBet - prevRaise - getBet() << " minimum.\nYour stack is $"
       << getStack() << ".\n\n";

  int check_value = 2 * currentBet - prevRaise - getBet();

  readInput(raise_value, check_value);
}

void Bot::decideRaiseValue(int &currentBet, int &prevRaise, int &raise_value) {
  raise_value = 20;
}

void Human::decideFCRA(int &currentBet, int &prevRaise, string &decision_in) {
  if (getStack() < (2 * currentBet - prevRaise)) {  // minimum raise
    if (getStack() <= currentBet - getBet()) {      // chips <= raise;
      cout << "Do you fold or go all in ($" << getStack() << ")?\n\n";
      decisionSanitizer(decision_in, {"fold", "all in"});
    } else {  // covers but cannot make minimum
      cout << "Do you fold, call ($" << currentBet - getBet()
           << "), or go all in ($" << getStack()
           << ")? You do not have enough to raise.\n\n";
      decisionSanitizer(decision_in, {"fold", "call", "all in"});
    }
  } else {  // covers and can make minimum raise
    cout << "Do you fold, call ($" << currentBet - getBet() << "), raise ($"
         << 2 * currentBet - prevRaise - getBet()
         << " minimum), or go all in ($" << getStack() << ")?\n\n";
    decisionSanitizer(decision_in, {"fold", "call", "raise", "all in"});
  }
}

void Bot::decideFCRA(int &currentBet, int &prevRaise, string &decision_in) {
  decision_in = "call";
}