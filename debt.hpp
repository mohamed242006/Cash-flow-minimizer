//
//  debt.hpp
//  DS_PROJECT
//
//  Created by Mohamed Amir on 21/12/2024.
//

#ifndef debt_hpp
#define debt_hpp
#include <stdio.h>
#include <string>
using namespace std;
template <typename T, typename U>
struct Debt {
    T borrower;
    T lender;
    U amount;

    // Constructors
    Debt() : borrower(T()), lender(T()), amount(U()) {}
    Debt(const T& b, const T& l, const U& a) : borrower(b), lender(l), amount(a) {}
};


#endif /* debt_hpp */
