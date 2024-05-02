#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>    
#include <ctime>     
#include "card.hpp"
#include "pack.hpp"
// #include "player.hpp" 

using namespace std;


int main(){
    Pack* pack = new Pack();
    for(int i = 0; i < 52; ++i){
        cout << pack->dealCard().printCard() << endl;;
    }
}