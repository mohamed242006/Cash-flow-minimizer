//
//  main.cpp
//  DS_PROJECT
//
//  Created by Mohamed Amir on 20/12/2024.
//

#include <fstream> // Added to handle file operations
#include <iostream>
#include <string>
#include <limits>
#include "CashFlowMinimizer.hpp"
using namespace std;

int getValidInteger(const string &prompt, int minValue = 1)
{
    int value;
    cout << prompt;
    while (true)
    {
        cin >> value;
        if (cin.fail() || value < minValue)
        {
            cout << "Invalid input. Please enter a valid number.\n";
            cin.clear();             // Clear error state
            cin.ignore(10000, '\n'); // Ignore invalid input
            cout << prompt;          // Prompt again
        }
        else
        {
            break; // Valid input, exit the loop
        }
    }
    return value;
}

int getMenuChoice(const string &prompt)
{
    int value;
    cout << prompt;
    while (true)
    {
        cin >> value;
        if (cin.fail() || value < 1 || value > 7)
        {
            cout << "Invalid input. Please enter a number between 1 and 7.\n";
            cin.clear();             // Clear error state
            cin.ignore(10000, '\n'); // Ignore invalid input
            cout << prompt;          // Prompt again
        }
        else
        {
            break; // Valid input, exit the loop
        }
    }
    return value;
}

double getValidDouble(const string &prompt, double minValue = 1)
{
    string input;
    double value;
    while (true)
    {
        cout << prompt;
        cin >> input;

        try
        {
            value = stod(input); // Convert string to double
            if (value >= minValue)
            {
                return value;
            }
            else
            {
                cout << "Invalid input. Please enter a number >= " << minValue << ".\n";
            }
        }
        catch (exception &)
        {
            cout << "Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

bool isValidName(const string &name)
{
    if (name.empty())
        return false;
    for (char c : name)
    {
        if (!isalpha(c))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    cout << "Welcome to the Cash Flow Minimizer!\n";

    int maxParticipants = getValidInteger("Enter the number of participants: ", 1);
    int maxDebts = getValidInteger("Enter the maximum number of debts: ", 1);

    CashFlowMinimizer<string, double> cfm(maxParticipants, maxDebts);

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Add a participant\n";
        cout << "2. Record a debt\n";
        cout << "3. Display all debts\n";
        cout << "4. Display balances\n";
        cout << "5. Minimize cash flow\n";
        cout << "6. Generate graph file\n";
        cout << "7. Exit\n";

        int choice = getMenuChoice("Choose an option (1-7): ");

        if (choice == 7)
        {
            cout << "Exiting program. Goodbye!\n";
            break;
        }

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "Enter participant name: ";
            cin >> name;
            while (!isValidName(name))
            {
                cout << "Invalid name! Please enter a valid name (letters only, non-empty): ";
                cin >> name;
            }
            cfm.addParticipant(name);
            break;
        }
        case 2:
        {
            string lender, borrower;
            cout << "Enter lender's name: ";
            cin >> lender;
            cout << "Enter borrower's name: ";
            cin >> borrower;
            double amount = getValidDouble("Enter debt amount: ", 0);
            cfm.recordDebt(lender, borrower, amount);
            break;
        }
        case 3:
            cfm.displayAllDebts(); // Shows original debts
            break;
        case 4:
            cfm.displayBalances();
            break;
        case 5:
            cfm.minimizeCashFlow(); // Minimizes cash flow
            break;
        case 6:
        {
            string filename;
            cout << "Enter filename for the graph file: ";
            cin >> filename;
            cfm.generateGraphFile(filename);
            break;
        }
        }
    }

    return 0;
}
