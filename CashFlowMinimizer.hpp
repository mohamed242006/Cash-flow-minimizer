//
//  CashFlowMinimizer.hpp
//  DS_PROJECT
//
//  Created by Mohamed Amir on 20/12/2024.
//

#ifndef CASHFLOWMINIMIZER_HPP
#define CASHFLOWMINIMIZER_HPP
#include "debt.hpp"
#include "Participants.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
using namespace std;

template <typename T, typename U>
class CashFlowMinimizer
{
private:
    Participant<T, U> *participants; // Dynamic array for participants
    int maxParticipants;             // Maximum number of participants
    int currentParticipantCount;     // Current count of participants

    Debt<T, U> *debts;    // Dynamic array for debts
    int maxDebts;         // Maximum number of debts
    int currentDebtCount; // Current count of debts

public:
    // Constructor and Destructor
    CashFlowMinimizer(int maxParticipants, int maxDebts);
    ~CashFlowMinimizer();

    // Add a participant
    void addParticipant(const T &name);

    void recordDebt(const T &lender, const T &borrower, const U &amount);

    void displayAllDebts() const;

    void displayBalances() const;

    void minimizeCashFlow();

    void generateGraphFile(const std::string &filename) const;
};

// Template function definitions
template <typename T, typename U>
CashFlowMinimizer<T, U>::CashFlowMinimizer(int maxParticipants, int maxDebts)
{
    if (maxParticipants <= 0)
    {
        throw invalid_argument("Max participants must be greater than 0.");
    }
    if (maxDebts <= 0)
    {
        throw invalid_argument("Max debts must be greater than 0.");
    }

    this->maxParticipants = maxParticipants;
    this->maxDebts = maxDebts;
    currentParticipantCount = 0;
    currentDebtCount = 0;

    participants = new Participant<T, U>[maxParticipants];
    debts = new Debt<T, U>[maxDebts];
}

template <typename T, typename U>
CashFlowMinimizer<T, U>::~CashFlowMinimizer()
{
    delete[] participants;
    delete[] debts;
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::addParticipant(const T &name)
{
    for (int i = 0; i < currentParticipantCount; ++i)
    {
        if (participants[i].name == name)
        {
            cout << "Participant with name \"" << name << "\" already exists!" << endl;
            return;
        }
    }

    if (currentParticipantCount < maxParticipants)
    {
        participants[currentParticipantCount++] = Participant<T, U>(name);
        cout << "Participant added successfully: " << name << endl;
    }
    else
    {
        cout << "Maximum number of participants reached!" << endl;
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::recordDebt(const T &lender, const T &borrower, const U &amount)
{
    bool lenderFound = false, borrowerFound = false;

    for (int i = 0; i < currentParticipantCount; ++i)
    {
        if (participants[i].name == lender)
            lenderFound = true;
        if (participants[i].name == borrower)
            borrowerFound = true;
    }

    if (!lenderFound)
    {
        cout << "Error: Lender \"" << lender << "\" does not exist.\n";
        return;
    }
    if (!borrowerFound)
    {
        cout << "Error: Borrower \"" << borrower << "\" does not exist.\n";
        return;
    }

    if (amount <= 0)
    {
        cout << "Error: Debt amount must be a positive number.\n";
        return;
    }

    if (currentDebtCount < maxDebts)
    {
        for (int i = 0; i < currentParticipantCount; ++i)
        {
            if (participants[i].name == lender)
            {
                participants[i].balance += amount;
            }
            if (participants[i].name == borrower)
            {
                participants[i].balance -= amount;
            }
        }

        debts[currentDebtCount++] = Debt<T, U>(borrower, lender, amount);
        cout << "Debt recorded: " << borrower << " owes " << lender << " $" << amount << "\n";
    }
    else
    {
        cout << "Error: Cannot record more debts. Max debt limit reached.\n";
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::displayAllDebts() const
{
    cout << "Recorded debts:\n";
    for (int i = 0; i < currentDebtCount; ++i)
    {
        const Debt<T, U> &debt = debts[i];
        cout << debt.borrower << " owes " << debt.lender
             << " $" << debt.amount << "\n";
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::displayBalances() const
{
    cout << "Balances after recording debts:\n";
    if (currentParticipantCount == 0)
    {
        cout << "No participants added yet.\n";
    }
    else
    {
        for (int i = 0; i < currentParticipantCount; ++i)
        {
            cout << participants[i].name << " balance: "
                 << participants[i].balance << "\n";
        }
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::minimizeCashFlow()
{
    bool transactionsMade = false;

    while (true)
    {
        int maxCreditIndex = -1, maxDebitIndex = -1;
        U maxCredit = 0, maxDebit = 0;

        for (int i = 0; i < currentParticipantCount; ++i)
        {
            if (participants[i].balance > maxCredit)
            {
                maxCredit = participants[i].balance;
                maxCreditIndex = i;
            }
            if (participants[i].balance < maxDebit)
            {
                maxDebit = participants[i].balance;
                maxDebitIndex = i;
            }
        }

        bool allSettled = true;
        for (int i = 0; i < currentParticipantCount; ++i)
        {
            if (participants[i].balance != 0)
            {
                allSettled = false;
                break;
            }
        }

        if (allSettled)
        {
            break;
        }

        U amount = maxCredit < -maxDebit ? maxCredit : -maxDebit;
        cout << participants[maxDebitIndex].name << " owes "
             << participants[maxCreditIndex].name << " $" << amount << "\n";

        participants[maxCreditIndex].balance -= amount;
        participants[maxDebitIndex].balance += amount;

        transactionsMade = true;
    }

    if (!transactionsMade)
    {
        cout << "No transactions needed. All debts are already settled.\n";
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::generateGraphFile(const string &filename) const
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file for writing graph data!\n";
        return;
    }

    file << "digraph DebtGraph {\n";
    for (int i = 0; i < currentDebtCount; ++i)
    {
        const Debt<T, U> &debt = debts[i];
        file << "    \"" << debt.borrower << "\" -> \"" << debt.lender
             << "\" [label=\"$" << debt.amount << "\"];\n";
    }

    file << "}\n";
    file.close();

    cout << "Graph file \"" << filename << "\" generated successfully.\n";
}

#endif // CASHFLOWMINIMIZER_HPP

#ifndef CASHFLOWMINIMIZER_HPP
#define CASHFLOWMINIMIZER_HPP
// Include guard to prevent the header file from being included multiple times.

#include "debt.hpp"
// Include the header file for the Debt class.

#include "Participants.hpp"
// Include the header file for the Participant class.

#include <iostream>
// Include the iostream library for input and output operations.

#include <fstream>
// Include the fstream library for file handling.

#include <stdexcept>
// Include the standard exceptions library for handling runtime errors.

#include <string>
// Include the string library for string manipulation.

using namespace std;
// Use the standard namespace to simplify code readability.

template <typename T, typename U>
// Define a template class where T represents participant names, and U represents debt amounts.

class CashFlowMinimizer
{
    // Main class to handle participants, debts, and cash flow minimization.

private:
    Participant<T, U> *participants;
    // Dynamic array to store participants.

    int maxParticipants;
    // Maximum number of participants allowed.

    int currentParticipantCount;
    // Tracks the current number of participants.

    Debt<T, U> *debts;
    // Dynamic array to store debts.

    int maxDebts;
    // Maximum number of debts allowed.

    int currentDebtCount;
    // Tracks the current number of debts.

public:
    CashFlowMinimizer(int maxParticipants, int maxDebts);
    // Constructor to initialize the arrays and member variables.

    ~CashFlowMinimizer();
    // Destructor to clean up dynamically allocated memory.

    void addParticipant(const T &name);
    // Adds a participant to the system by name.

    void recordDebt(const T &lender, const T &borrower, const U &amount);
    // Records a debt from a borrower to a lender with a specified amount.

    void displayAllDebts() const;
    // Displays all the recorded debts.

    void displayBalances() const;
    // Displays the balances of all participants after debts are recorded.

    void minimizeCashFlow();
    // Minimizes the cash flow by settling debts.

    void generateGraphFile(const std::string &filename) const;
    // Generates a graph representation of debts in a DOT file.
};

template <typename T, typename U>
CashFlowMinimizer<T, U>::CashFlowMinimizer(int maxParticipants, int maxDebts)
{
    // Constructor implementation.

    if (maxParticipants <= 0)
    {
        throw invalid_argument("Max participants must be greater than 0.");
        // Ensure the number of participants is positive.
    }

    if (maxDebts <= 0)
    {
        throw invalid_argument("Max debts must be greater than 0.");
        // Ensure the number of debts is positive.
    }

    this->maxParticipants = maxParticipants;
    // Initialize the maximum number of participants.

    this->maxDebts = maxDebts;
    // Initialize the maximum number of debts.

    currentParticipantCount = 0;
    // Set the current number of participants to 0.

    currentDebtCount = 0;
    // Set the current number of debts to 0.

    participants = new Participant<T, U>[maxParticipants];
    // Allocate memory for the participants array.

    debts = new Debt<T, U>[maxDebts];
    // Allocate memory for the debts array.
}

template <typename T, typename U>
CashFlowMinimizer<T, U>::~CashFlowMinimizer()
{
    // Destructor implementation.

    delete[] participants;
    // Release the memory allocated for participants.

    delete[] debts;
    // Release the memory allocated for debts.
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::addParticipant(const T &name)
{
    // Add a new participant to the system.

    for (int i = 0; i < currentParticipantCount; ++i)
    {
        if (participants[i].name == name)
        {
            // Check if a participant with the same name already exists.

            cout << "Participant with name \"" << name << "\" already exists!" << endl;
            // Inform the user that the participant already exists.

            return;
            // Exit the function to avoid duplicate entries.
        }
    }

    if (currentParticipantCount < maxParticipants)
    {
        // Check if there is space to add a new participant.

        participants[currentParticipantCount++] = Participant<T, U>(name);
        // Add the participant and increment the count.

        cout << "Participant added successfully: " << name << endl;
        // Inform the user of the successful addition.
    }
    else
    {
        cout << "Maximum number of participants reached!" << endl;
        // Inform the user that no more participants can be added.
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::recordDebt(const T &lender, const T &borrower, const U &amount)
{
    // Record a new debt between a lender and a borrower.

    bool lenderFound = false, borrowerFound = false;
    // Flags to check if both participants exist.

    for (int i = 0; i < currentParticipantCount; ++i)
    {
        if (participants[i].name == lender)
            lenderFound = true;
        // Mark lender as found if their name matches.

        if (participants[i].name == borrower)
            borrowerFound = true;
        // Mark borrower as found if their name matches.
    }

    if (!lenderFound)
    {
        // If lender does not exist:

        cout << "Error: Lender \"" << lender << "\" does not exist.\n";
        // Print error message.

        return;
        // Exit the function.
    }

    if (!borrowerFound)
    {
        // If borrower does not exist:

        cout << "Error: Borrower \"" << borrower << "\" does not exist.\n";
        // Print error message.

        return;
        // Exit the function.
    }

    if (amount <= 0)
    {
        // Check if the debt amount is positive.

        cout << "Error: Debt amount must be a positive number.\n";
        // Print error message.

        return;
        // Exit the function.
    }

    if (currentDebtCount < maxDebts)
    {
        // Check if there is space to record a new debt.

        for (int i = 0; i < currentParticipantCount; ++i)
        {
            if (participants[i].name == lender)
            {
                participants[i].balance += amount;
                // Increase the lender's balance.
            }

            if (participants[i].name == borrower)
            {
                participants[i].balance -= amount;
                // Decrease the borrower's balance.
            }
        }

        debts[currentDebtCount++] = Debt<T, U>(borrower, lender, amount);
        // Record the debt and increment the debt count.

        cout << "Debt recorded: " << borrower << " owes " << lender << " $" << amount << "\n";
        // Inform the user of the successful recording.
    }
    else
    {
        cout << "Error: Cannot record more debts. Max debt limit reached.\n";
        // Inform the user that no more debts can be recorded.
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::displayAllDebts() const
{
    // Display all recorded debts.

    cout << "Recorded debts:\n";
    // Print a header for the debts.

    for (int i = 0; i < currentDebtCount; ++i)
    {
        // Loop through all recorded debts.

        const Debt<T, U> &debt = debts[i];
        // Reference the current debt.

        cout << debt.borrower << " owes " << debt.lender
             << " $" << debt.amount << "\n";
        // Print the borrower, lender, and debt amount.
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::displayBalances() const
{
    // Display the balances of all participants.

    cout << "Balances after recording debts:\n";
    // Print a header for the balances.

    if (currentParticipantCount == 0)
    {
        // Check if no participants have been added.

        cout << "No participants added yet.\n";
        // Inform the user that no participants are available.
    }
    else
    {
        for (int i = 0; i < currentParticipantCount; ++i)
        {
            // Loop through all participants.

            cout << participants[i].name << " balance: "
                 << participants[i].balance << "\n";
            // Print each participant's name and balance.
        }
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::minimizeCashFlow()
{
    // Minimize the cash flow to settle debts among participants.

    bool transactionsMade = false;
    // Track if any transactions are made during the process.

    while (true)
    {
        // Keep processing until all debts are settled.

        int maxCreditIndex = -1, maxDebitIndex = -1;
        // Indices for participants with the maximum credit and debit.

        U maxCredit = 0, maxDebit = 0;
        // Variables to store the maximum credit and debit values.

        for (int i = 0; i < currentParticipantCount; ++i)
        {
            // Loop through all participants to find max credit and debit.

            if (participants[i].balance > maxCredit)
            {
                maxCredit = participants[i].balance;
                maxCreditIndex = i;
                // Update maximum credit and its index.
            }

            if (participants[i].balance < maxDebit)
            {
                maxDebit = participants[i].balance;
                maxDebitIndex = i;
                // Update maximum debit and its index.
            }
        }

        bool allSettled = true;
        // Assume all debts are settled initially.

        for (int i = 0; i < currentParticipantCount; ++i)
        {
            // Check if any participant still has an unsettled balance.

            if (participants[i].balance != 0)
            {
                allSettled = false;
                // Set to false if a non-zero balance is found.
                break;
            }
        }

        if (allSettled)
        {
            // If all balances are zero, exit the loop.

            break;
        }

        U amount = maxCredit < -maxDebit ? maxCredit : -maxDebit;
        // Determine the amount to be transferred: the smaller of maxCredit and maxDebit (in absolute terms).

        cout << participants[maxDebitIndex].name << " owes "
             << participants[maxCreditIndex].name << " $" << amount << "\n";
        // Print the transaction to settle the debt.

        participants[maxCreditIndex].balance -= amount;
        // Deduct the amount from the participant with the maximum credit.

        participants[maxDebitIndex].balance += amount;
        // Add the amount to the participant with the maximum debit.

        transactionsMade = true;
        // Mark that a transaction has been made.
    }

    if (!transactionsMade)
    {
        // If no transactions were made, inform the user.

        cout << "No transactions needed. All debts are already settled.\n";
    }
}

template <typename T, typename U>
void CashFlowMinimizer<T, U>::generateGraphFile(const string &filename) const
{
    // Generate a DOT file for visualizing the debt graph.

    ofstream file(filename);
    // Open the specified file for writing.

    if (!file.is_open())
    {
        // Check if the file could not be opened.

        cout << "Error opening file for writing graph data!\n";
        // Print an error message.

        return;
        // Exit the function.
    }

    file << "digraph DebtGraph {\n";
    // Start the DOT file format with a directed graph declaration.

    for (int i = 0; i < currentDebtCount; ++i)
    {
        // Loop through all recorded debts.

        const Debt<T, U> &debt = debts[i];
        // Reference the current debt.

        file << "    \"" << debt.borrower << "\" -> \"" << debt.lender
             << "\" [label=\"$" << debt.amount << "\"];\n";
        // Write the debt relationship as a directed edge in the graph.
    }

    file << "}\n";
    // Close the graph definition.

    file.close();
    // Close the file after writing.

    cout << "Graph file \"" << filename << "\" generated successfully.\n";
    // Inform the user that the graph file was successfully created.
}

#endif // CASHFLOWMINIMIZER_HPP
// End of the include guard.
