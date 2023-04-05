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
        Card* p1_c;
        Card* p2_c;
        //string turns[26];
        vector<pair<Card*,Card*> > turns;
        int draw;
        int numOfTurns;
        int turnStart;
        int turnEnd;

    public:
        //Game();

        Game(Player& player1, Player& player2);

        void playTurn();

        void printLastTurn();

        void playAll();

        void printWiner();

        void printLog();

        void printStats();

        void shuffleCards();

        Card* getP1Card();

        void setP1Card(Card* c);

        Card* getP2Card();

        void setP2Card(Card* c);

        string getWinner();

        void setWinner(string p);

        int play();

        void ChecksPrintTurn(Card* p1c, Card* p2c);
    };
}

#endif //CPP_HW2_GAME_H
