#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>    

#include "card.hpp"
#include "pack.hpp"
#include "player.hpp"
#include "round.hpp"

using namespace std;

int main() {
  std::vector<std::unique_ptr<Player>> players;

  // Create and add four new Human players to the vector
  players.emplace_back(std::make_unique<Human>("Esben", "Human", 1000));
  players.emplace_back(std::make_unique<Human>("Parker", "Human", 1000));
  // players.emplace_back(std::make_unique<Human>("Adam", "Human", 1000));
  // players.emplace_back(std::make_unique<Human>("Tyler", "Human", 1000));

  Round *round = new Round(players);
  round->setup();
  round->preflop();
  round->resetBets();
  round->postflop("flop");
  round->resetBets();
  round->postflop("turn");
  round->resetBets();
  round->postflop("river");
  round->show_cards();
}
