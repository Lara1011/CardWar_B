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

    Game::Game(Player& player1, Player& player2): p1(player1), p2(player2) {
        this->p1 = player1;
        this->p2 = player2;
        this->p1.setCardsStack(26);
        this->p2.setCardsStack(26);
        shuffleCards();
        this->turnStart = 0;
        this->turnEnd = 0;
        this->index=0;
        this->numOfTurns = 0;
        this->draw = 0;
        this->turns.clear();

    }

    void Game::playAll() {
        while(p1.stacksize() > 0 && p2.stacksize() > 0){
            playTurn();
        }
    }

    void Game::playTurn() {
        if(p1.getName().empty() || p2.getName().empty())
            throw invalid_argument("At least one of the players has no name !");
        else if(&p1 == &p2)
            throw invalid_argument("There is only one player !");
        else if(p1.stacksize() > 0 && p2.stacksize() > 0 && numOfTurns < 26) {
            Card card1 = p1.getCard()[index];
            Card card2 = p2.getCard()[index];
            int win = 0;
            turnStart = turnEnd;
            turnEnd++;
            win = play(card1, card2);
            index++;
            p1.decreaseCards();
            p2.decreaseCards();
            if (win == 0) {
                int num = 0;
                while (p1.stacksize() > 1 && p2.stacksize() > 1 && win == 0 && numOfTurns<26) {
                    draw++;
                    turnEnd++;
                    num += 4;
                    card1 = p1.getCard()[index];
                    card2 = p2.getCard()[index];
                    index++;
                    p1.decreaseCards();
                    p2.decreaseCards();
                    card1 = p1.getCard()[index];
                    card2 = p2.getCard()[index];
                    win = play(card1, card2);
                    index++;
                    p1.decreaseCards();
                    p2.decreaseCards();
                }
                if (win == 1) {
                    p1.updateCards(num);
                }
                if (win == 2) {
                    p2.updateCards(num);
                }
                if(win == 0){
                    if(p1.stacksize() == 1 && p2.stacksize() == 1) {
                        p1.updateCards(num / 2 + 2);
                        p2.updateCards(num / 2 + 2);
                        p1.decreaseCards();
                        p2.decreaseCards();
                    }
                    else if(p1.stacksize() == 0 && p2.stacksize() == 0) {
                        p1.updateCards(num / 2 + 1);
                        p2.updateCards(num / 2 + 1);
                    }
                }
            }
            numOfTurns++;
            p1.setWinRate(static_cast<double>(p1.getWinTimes())/numOfTurns);
            p2.setWinRate(static_cast<double>(p2.getWinTimes())/numOfTurns);
        }
        else
            throw invalid_argument("There is no cards left !");
    }

    int Game::play(Card card1, Card card2){
        int value1 = card1.getValue();
        int value2 = card2.getValue();
        turns.push_back(make_pair(card1, card2));
        if(value1 == 1 && value2 > 2){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(value2 == 1 && value1 > 2){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(value2 == 1 && value1 == 2){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(value1 == 1 && value2 == 2){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(value1 > value2){
            p1.updateCards(2);
            p1.updateWinTimes();
            return 1;
        }
        else if(value1 < value2){
            p2.updateCards(2);
            p2.updateWinTimes();
            return 2;
        }
        else if(value1 == value2) {
            return 0;
        }
        return 0;
    }

    void Game::printLastTurn() {
        for (auto it = turns.begin() + turnStart; it != turns.begin() + turnEnd; ++it) {
            Card card1 = it->first;
            Card card2 = it->second;
            ChecksPrintTurn(card1, card2);
        }
    }

    void Game::printLog() {
        for (auto it = turns.begin(); it != turns.begin() + turnEnd; ++it) {
            Card card1 = it->first;
            Card card2 = it->second;
            ChecksPrintTurn(card1, card2);
        }
    }

    void Game::ChecksPrintTurn(Card p1c, Card p2c) {
        int value1 = p1c.getValue();
        int value2 = p2c.getValue();
        string suit1 = p1c.getSuit();
        string suit2 = p2c.getSuit();
        if(value1 == 1){
            if(value2 == 1)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". Draw. ";
            if(value2 == 2)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p2.getName() << " wins.\n";
            if(value2 < 11 && value2 > 2)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p1.getName() << " wins.\n";
            if(value2 == 11)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". " << p1.getName() << " wins.\n";
            if(value2 == 12)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". " << p1.getName() << " wins.\n";
            if(value2 == 13)
                cout << p1.getName() << " played Ace of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". " << p1.getName() << " wins.\n";
        }
        else if(value2 == 1){
            if(value1 == 2)
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". " << p1.getName() << " wins.\n";
            if(value1 < 11 && value1 > 2)
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". " << p2.getName() << " wins.\n";
            if(value1 == 11)
                cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". " << p2.getName() << " wins.\n";
            if(value1 == 12)
                cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". " << p2.getName() << " wins.\n";
            if(value1 == 13)
                cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played Ace of " << suit2 << ". " << p2.getName() << " wins.\n";
        }
        else if(value1 > value2 || value1 == value2){
            if(value1 == 11){
                if(value2 == 11)
                    cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p1.getName() << " wins.\n";
            }
            else if(value1 == 12){
                if(value2 == 11)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". " << p1.getName() << " wins.\n";
                else if(value2 == 12)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p1.getName() << " wins.\n";
            }
            else if(value1 == 13){
                if(value2 == 11)
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". " << p1.getName() << " wins.\n";
                else if(value2 == 12)
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". " << p1.getName() << " wins.\n";
                else if(value2 == 13)
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p1.getName() << " wins.\n";
            }
            else if(value2 == 12){
                if(value1 == 12)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". " << p1.getName() << " wins.\n";
            }
            else if(value2 == 13){
                cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". Draw. ";
            }
            else if(value1 != value2)
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p1.getName() << " wins.\n";
            else
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". Draw. ";
        }
        else if(value1 <= value2){
            if(value2 == 11){
                if(value1 == 11)
                    cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played Jack of " << suit2 << ". " << p2.getName() << " wins.\n";
            }
            else if(value2 == 12){
                if(value1 == 11)
                    cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". " << p2.getName() << " wins.\n";
                else if(value1 == 12)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". " << p2.getName() << " wins.\n";
            }
            else if(value2 == 13){
                if(value1 == 11)
                    cout << p1.getName() << " played Jack of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". " << p2.getName() << " wins.\n";
                else if(value1 == 12)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". " << p2.getName() << " wins.\n";
                else if(value1 == 13)
                    cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". " << p2.getName() << " wins.\n";
            }
            else if(value1 == 12){
                if(value2 == 12)
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played Queen of " << suit2 << ". Draw. ";
                else
                    cout << p1.getName() << " played Queen of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". " << p2.getName() << " wins.\n";
            }
            else if(value1 == 13){
                cout << p1.getName() << " played King of " << suit1 << " " << p2.getName() << " played King of " << suit2 << ". Draw. ";
            }
            else if(value1 != value2)
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". " << p2.getName() << " wins.\n";
            else
                cout << p1.getName() << " played " << value1 << " of " << suit1 << " " << p2.getName() << " played " << value2 << " of " << suit2 << ". Draw. ";
        }
    }

    void Game::printStats() {
        cout << p1.getName() << ": \n";
        cout << "  Cards won: " << p1.cardesTaken() << "\n";
        cout << "  Win rate: " << p1.getWinRate() << "\n";

        cout << p2.getName() << ": \n";
        cout << "  Cards won: " << p2.cardesTaken() << "\n";
        cout << "  Win rate: " << p2.getWinRate() << "\n";

        cout << "Number of draws: " << draw << "\n";
        cout << "Draw rate: " << static_cast<double>(draw)/static_cast<double>(numOfTurns) << "\n";
    }

    void Game::printWiner() {
        if (p1.cardesTaken() > p2.cardesTaken())
            cout << "The winner is: " << p1.getName() << "\n";
        else if (p2.cardesTaken() > p1.cardesTaken())
            cout << "The winner is: " << p2.getName() << "\n";
        else
            cout << "Draw ! Should throw an exception ! \n";
        //throw invalid_argument("Draw !");
    }


    void Game::shuffleCards() {
        Card deck[52];
        int t = 0;
        for (int i=1; i<=13; i++){
            deck[t++] = Card(i, "Diamonds");
            deck[t++] = Card(i, "Hearts");
            deck[t++] = Card(i, "Clubs");
            deck[t++] = Card(i, "Spades");
        }
        int i_p1 = 0, i_p2 = 0;
        Card p1_cards[26];
        Card p2_cards[26];
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
    }

    vector<pair<Card, Card> > Game::getVector() {
        return turns;
    }
}