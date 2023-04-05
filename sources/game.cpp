//
// Created by Lara Abu Hamad on 26/03/2023.
//

#include "game.hpp"
#include "player.hpp"
#include "card.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

namespace ariel {
    //Game::Game(): p1(Player("player1")), p2(Player("player2"));

    Game::Game(Player& player1, Player& player2): p1(player1), p2(player2) {
        if(player1.getName().empty() || player2.getName().empty())
            throw invalid_argument("At least one of the players has no name !");
        if(player1.getName() == player2.getName())
            throw invalid_argument("There is only one player !");
        this->p1 = player1;
        this->p2 = player2;
        p1.setCardsStack(26);
        p2.setCardsStack(26);
        shuffleCards();
        turnStart = 0;
        turnEnd = 0;
    }

    void Game::playAll() {
        while(p1.stacksize() > 0 && p2.stacksize() > 0){
            playTurn();
        }
    }

    void Game::playTurn() {
        if(p1.getName().empty() || p2.getName().empty())
            throw invalid_argument("At least one of the players has no name !");
        else if(p1.getName() == p2.getName())
            throw invalid_argument("There is only one player !");
        else if(p1.stacksize() > 0 && p2.stacksize() > 0) {
            Card* card1 = p1.getCard();
            Card* card2 = p2.getCard();
            cout << card1->getValue() << ", " << card2->getValue() << "\n";
            setP1Card(card1);
            setP2Card(card2);
            int win = 0;
            turnStart = turnEnd;
            turnEnd++;
            win = play();
            if (win == 0) {
                int num = 0;
                while (p1.stacksize() > 1 && p2.stacksize() > 1 && win == 0) {
                    draw++;
                    turnEnd++;
                    num += 4;
                    card1 = p1.getCard();
                    card2 = p2.getCard();
                    card1 = p1.getCard();
                    card2 = p2.getCard();
                    setP1Card(card1);
                    setP2Card(card2);
                    win = play();
                }
                if (win == 1) {
                    p1.updateCards(num);
                }
                if (win == 2) {
                    p2.updateCards(num);
                }
            }
            numOfTurns++;
            p1.setWinRate(p1.getWinTimes()/numOfTurns);
            p2.setWinRate(p2.getWinTimes()/numOfTurns);
        }
        else
            throw invalid_argument("There is no cards left !");
    }

    int Game::play(){
//        cout << p1.stacksize() << ", " << p2.stacksize()<<"\n";
//        cout << getP1Card()->getValue() << ", " << getP2Card()->getValue() << "\n";
        turns.push_back(make_pair(getP1Card(), getP2Card()));
        if(getP1Card()->getValue() == 1 && getP2Card()->getValue() > 2){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(getP2Card()->getValue() == 1 && getP1Card()->getValue() > 2){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(getP2Card()->getValue() == 1 && getP1Card()->getValue() == 2){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(getP1Card()->getValue() == 1 && getP2Card()->getValue() == 2){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(getP1Card()->getValue() > getP2Card()->getValue()){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(getP1Card()->getValue() < getP2Card()->getValue()){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(getP1Card()->getValue() == getP2Card()->getValue()) {
            return 0;
        }
        return 0;
    }

    void Game::printLastTurn() {
        for(size_t i = static_cast<size_t>(turnStart); i < turnEnd; i++){
            Card* p1c = turns[i].first;
            Card* p2c = turns[i].second;
            ChecksPrintTurn(p1c, p2c);
        }
    }

    void Game::printLog() {
        for(std::vector<std::pair<ariel::Card, ariel::Card> >::size_type i = 0; i < turnEnd; i++){
            Card* p1c = turns[i].first;
            Card* p2c = turns[i].second;
            ChecksPrintTurn(p1c, p2c);
        }
    }

    void Game::ChecksPrintTurn(Card* p1c, Card* p2c) {
        if(p1c->getValue() == 1){
            if(p2c->getValue() == 1)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". Draw. ";
            if(p2c->getValue() == 2)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            if(p2c->getValue() < 11 && p2c->getValue() > 2)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            if(p2c->getValue() == 11)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            if(p2c->getValue() == 12)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            if(p2c->getValue() == 13)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
        }
        else if(p2c->getValue() == 1){
            if(p1c->getValue() == 1)
                cout << p1.getName() << " played Ace of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". Draw. ";
            if(p1c->getValue() == 2)
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            if(p1c->getValue() < 11 && p1c->getValue() > 2)
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            if(p1c->getValue() == 11)
                cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            if(p1c->getValue() == 12)
                cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            if(p1c->getValue() == 13)
                cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played Ace of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
        }
        else if(p1c->getValue() > p2c->getValue() || p1c->getValue() == p2c->getValue()){
            if(p1c->getValue() == 11){
                if(p2c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            }
            else if(p1c->getValue() == 12){
                if(p2c->getValue() == 11)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
                else if(p2c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            }
            else if(p1c->getValue() == 13){
                if(p2c->getValue() == 11)
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
                else if(p2c->getValue() == 12)
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
                else if(p2c->getValue() == 13)
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            }
            else if(p2c->getValue() == 11){
                if(p1c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". Draw. ";
                else if(p1c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
                else if(p1c->getValue() == 13)
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            }
            else if(p2c->getValue() == 12){
                if(p1c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            }
            else if(p2c->getValue() == 13){
                cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". Draw. ";
            }
            else if(p1c->getValue() != p2c->getValue())
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p1.getName() << " wins.\n";
            else
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". Draw. ";
        }
        else if(p1c->getValue() <= p2c->getValue()){
            if(p2c->getValue() == 11){
                if(p1c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            }
            else if(p2c->getValue() == 12){
                if(p1c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
                else if(p1c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            }
            else if(p2c->getValue() == 13){
                if(p1c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
                else if(p1c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
                else if(p1c->getValue() == 13)
                    cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            }
            else if(p1c->getValue() == 11){
                if(p2c->getValue() == 11)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Jack of " << p2c->getSuit() << ". Draw. ";
                else if(p2c->getValue() == 12)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
                else if(p2c->getValue() == 13)
                    cout << p1.getName() << " played Jack of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            }
            else if(p1c->getValue() == 12){
                if(p2c->getValue() == 12)
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played Queen of " << p2c->getSuit() << ". Draw. ";
                else
                    cout << p1.getName() << " played Queen of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            }
            else if(p1c->getValue() == 13){
                cout << p1.getName() << " played King of " << p1c->getSuit() << " " << p2.getName() << " played King of " << p2c->getSuit() << ". Draw. ";
            }
            else if(p1c->getValue() != p2c->getValue())
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". " << p2.getName() << " wins.\n";
            else
                cout << p1.getName() << " played " << p1c->getValue() << " of " << p1c->getSuit() << " " << p2.getName() << " played " << p2c->getValue() << " of " << p2c->getSuit() << ". Draw. ";
        }
    }

    void Game::printStats() {
        cout << p1.getName() << ": ";
        cout << "Cards won: " << p1.cardesTaken();
        cout << "Win rate: " << p1.getWinRate();

        cout << p2.getName() << ": ";
        cout << "Cards won: " << p2.cardesTaken();
        cout << "Win rate: " << p2.getWinRate();

        cout << "";
        cout << "Number of draws: " << draw;
        cout << "Draw rate: " << draw/numOfTurns;
    }

    void Game::printWiner() {
        if(p1.cardesTaken() > p2.cardesTaken())
            cout << "The winner is: " << p1.getName();
        if(p2.cardesTaken() > p1.cardesTaken())
            cout << "The winner is: " << p2.getName();
        else
            cout << "Draw !";
    }

    Card* Game::getP1Card() {
        return this->p1_c;
    }

    void Game::setP1Card(Card* c) {
        this->p1_c = c;
    }

    Card* Game::getP2Card() {
        return this->p2_c;
    }

    void Game::setP2Card(Card* c) {
        this->p2_c = c;
    }

//    string Game::getWinner() {
//        return this->winner;
//    }
//
//    void Game::setWinner(string p) {
//        this->winner = p;
//    }

    void Game::shuffleCards() {
        Card deck[52];
        int index = 0;
        for (int i=1; i<=13; i++){
            deck[index++] = Card(i, "Diamonds");
            deck[index++] = Card(i, "Hearts");
            deck[index++] = Card(i, "Clubs");
            deck[index++] = Card(i, "Spades");
        }
        int i_p1 = 0, i_p2 = 0;
        Card p1_cards[26];
        Card p2_cards[26];
        //random_shuffle(begin(deck), end(deck));
        //shuffle(begin(deck), end(deck), default_random_engine());
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(begin(deck), end(deck), default_random_engine(seed));
        for(int i=0; i<52; i++){
            if(i%2 == 0)
                p1_cards[i_p1++] = deck[i];
            else
                p2_cards[i_p2++] = deck[i];
        }
        p1.setCards(p1_cards);
        p2.setCards(p2_cards);
//        for(int i=0; i<26; i++){
//            cout << i << ": " << p1.getCards()[i].getValue() << ", " << p1.getCards()[i].getSuit() << "\n";
//        }
//        cout << "\n";
//        for(int i=0; i<26; i++){
//            cout << i << ": "<< p2.getCards()[i].getValue() << ", "  << p2.getCards()[i].getSuit() << "\n";
//        }
//        cout << "getCard: \n";
//        for(int i=0; i<26; i++){
//            Card* card = p1.getCard();
//            cout << i << ": " << card->getValue() << ", of " << card->getSuit() << "\n";
//        }
//        cout << "\n";
//        for(int i=0; i<26; i++){
//            Card* card = p2.getCard();
//            cout << i << ": "<< card->getValue() << ", of "  << card->getSuit() << "\n";
//        }
    }
}