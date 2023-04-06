//
// Created by Lara Abu Hamad on 26/03/2023.
//

#ifndef CPP_HW2_GAME_H
#define CPP_HW2_GAME_H
#include "player.hpp"
#include <string>
#include <vector>
using namespace std;

namespace ariel {
    class Game {
    private:
        Player& p1;
        Player& p2;
        vector<pair<Card,Card> > turns;
        int draw;
        int numOfTurns;
        int turnStart;
        int turnEnd;
        int index;

    public:
        Game(Player& player1, Player& player2);

        void playTurn();

        void printLastTurn();

        void playAll();

        void printWiner();

        void printLog();

        void printStats();

        void shuffleCards();

        int play(Card card1, Card card2);

        void ChecksPrintTurn(Card p1c, Card p2c);

        vector<pair<Card,Card> > getVector();
    };
}

#endif //CPP_HW2_GAME_H
