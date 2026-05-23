//
//  Participants.cpp
//  DS_PROJECT
//
//  Created by Mohamed Amir on 20/12/2024.
//
#include <iostream>
#include "Participants.hpp"
using namespace std;
template <typename T, typename U>
class Participant
{
public:
    T name;
    U balance;

    Participant(const T &name = T()) : name(name), balance(0) {}

    T getName() const
    {
        return name;
    }

    U getBalance() const
    {
        return balance;
    }

    void setBalance(U newBalance)
    {
        balance = newBalance;
    }
};
