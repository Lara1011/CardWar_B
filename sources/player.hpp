//
// Created by Lara Abu Hamad on 26/03/2023.
//

#ifndef CPP_HW2_PLAYER_H
#define CPP_HW2_PLAYER_H
#include <string>
#include "card.hpp"
using namespace std;

namespace ariel {
    class Player {
    private:
        string name;
        int cardsLeft;
        int cardsWon;
        double winRate;
        Card *cards;
        int winTimes;
        //int index;

    public:
        //Player() {};

        Player(string name);

        string getName();

        void setName(string name);

        double getWinRate();

        void setWinRate(double rate);

        int stacksize();

        int cardesTaken();

        void updateCards(int numOfCards);

        void decreaseCards();

        void setCards(Card* cards);

        Card* getCard();

        void setCardsStack(int size);

        void updateWinTimes();

        int getWinTimes();

        Card* getCards();
    };
}

#endif //CPP_HW2_PLAYER_H
