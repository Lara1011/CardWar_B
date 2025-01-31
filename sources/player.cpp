//
// Created by Lara Abu Hamad on 26/03/2023.
//

#include "player.hpp"
#include <string>
#include <iostream>

using namespace std;

namespace ariel {
    Player::Player(string name) {
        this->name = name;
        this->cardsLeft = 0;
        this->cardsWon = 0;
        this->winRate = 0.0;
        this->winTimes = 0;
        this->cards = nullptr;
    }

    string Player::getName() {
        return this->name;
    }

    void Player::setName(string name) {
        this->name = name;
    }

    double Player::getWinRate() {
        return this->winRate;
    }

    void Player::setWinRate(double rate) {
        this->winRate = rate;
    }

    int Player::stacksize() {
        return this->cardsLeft;
    }

    int Player::cardesTaken() {
        return this->cardsWon;
    }

    void Player::updateCards(int numOfCards) {
        this->cardsWon += numOfCards;
    }

    void Player::decreaseCards() {
        this->cardsLeft--;
    }

    void Player::setCards(Card *cards) {
        this->cards = cards;
    }

    Card *Player::getCards() {
        return this->cards;
    }

    Card *Player::getCard() {
        if (stacksize() < 0) {
            throw runtime_error("Stack is empty !");
        } else
            return cards;

    }

    void Player::setCardsStack(int size) {
        this->cardsLeft = size;
    }

    void Player::updateWinTimes() {
        this->winTimes++;
    }

    int Player::getWinTimes() {
        return this->winTimes;
    }
}