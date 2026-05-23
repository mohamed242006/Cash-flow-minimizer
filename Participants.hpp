//
//  Participants.hpp
//  DS_PROJECT
//
//  Created by Ahmed Zahran on 20/12/2024.
//

#ifndef Participants_hpp
#define Participants_hpp
using namespace  std;
#include <stdio.h>
#include <string>

template <typename T, typename U>
class Participant {
public:
    T name;
    U balance;

    // Constructors
    Participant() : name(T()), balance(U()) {}
    Participant(const T& n) : name(n), balance(U()) {}
};

#endif
