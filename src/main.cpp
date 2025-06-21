#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
// #include <map>
#include "../include/finance.hpp"

#define APPNAME "Personal Finance Tracker"
#define FILEPATH "data.txt"

std::vector<Transaction> TRANSACTIONS;
int TRANSACTIONS_LENGTH = 0;

void LoadTransactions();
void AddTransaction();
void ViewAllTransactions();
void SearchTransactions();
void SetBudgetGoal();
void GenerateMonthlyReport();
void SaveTransactions();
bool IsValidDate(int day, int month, int year);
std::vector<std::string> Split(std::string input, char breakOn);

int main()
{
    LoadTransactions();

    int option;

    do
    {
        try
        {
            std::cout << "======== " << APPNAME << " ========\n";
            std::cout << "1. Add Transaction\n2. View All Transactions\n3. Search Transactions\n4. Set Budget Goal\n5. Generate Monthly Report\n6. Save & Exit\n\nChoose an option: ";
            std::cin >> option;

            switch(option)
            {
                case 1:
                    AddTransaction();
                    break;
                case 2:
                    ViewAllTransactions();
                    break;
                case 3:
                    SearchTransactions();
                    break;
                case 4:
                    SetBudgetGoal();
                    break;
                case 5:
                    GenerateMonthlyReport();
                    break;
                case 6:
                    break;
                default:
                    std::cout << "Invalid option!\n\n";
                    break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    } while (option != 6);

    SaveTransactions();
    
    return 0;
}

void LoadTransactions()
{
    std::vector<std::string> data = {};
    int data_length = 0;

    std::ifstream fileContent;
    fileContent.open(FILEPATH);

    if(fileContent.is_open())
    {
        std::string curLine;

        while(std::getline(fileContent, curLine))
        {
            data.push_back(curLine);
            data_length++;
        }

        fileContent.close();
    }
    else
    {
        std::cerr << "Couldn't find file!\n";
    }

    if(data_length > 0)
    {
        for(std::string curData : data)
        {
            double curAmmount;
            std::string curCategory = "";
            std::string curDescription = "";
            std::vector<int> curDate = {00, 00, 0000};

            std::vector<std::string> splittedLine = Split(curData, '[');

            std::vector<std::string> splittedDate = Split(splittedLine[3], '/');

            try
            {
                curAmmount = std::stod(splittedLine[0]);
                curCategory = splittedLine[1];
                curDescription = splittedLine[2];

                for(int i = 0; i < 3; i++)
                {
                    curDate[i] = std::stoi(splittedDate[i]);
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }

            Transaction curTransaction(curAmmount, curCategory, curDescription, curDate);
            TRANSACTIONS.push_back(curTransaction);
            TRANSACTIONS_LENGTH++;
        }
    }
}

void AddTransaction()
{
    double amount = 0.00;
    std::string category = "";
    std::string description = "";
    std::vector<int> date = {00, 00, 0000};

    std::cout << "---- Add Transaction ----\n";

    try
    {
        std::cout << "Transaction Amount (+/-): ";
        std::cin >> amount;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cin.ignore();
    std::cout << "Transaction Category: ";
    std::getline(std::cin, category);

    std::cin.ignore();
    std::cout << "Transaction Description: ";
    std::getline(std::cin, description);

    try
    {
        std::cout << "Transaction Date Month Day: ";
        std::cin >> date[0];
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        std::cout << "Transaction Date Month: ";
        std::cin >> date[1];
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        std::cout << "Transaction Date Year: ";
        std::cin >> date[2];
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    if(IsValidDate(date[0], date[1], date[2]))
    {
        Transaction newTransaction(amount, category, description, date);
        TRANSACTIONS.push_back(newTransaction);
        TRANSACTIONS_LENGTH++;
    }
    else
    {
        AddTransaction();
    }
}

void ViewAllTransactions()
{
    if(TRANSACTIONS_LENGTH < 1)
    {
        std::cout << "No transactions found!\n";
    }
    else
    {
        double total = 0;

        for(auto &transaction : TRANSACTIONS)
        {
            transaction.LogData();
            
            total += transaction.Amount();
        }

        if(total > 0)
        {
            std::cout << "Total Amount: +$" << total << "\n";
        }
        else if(total == 0)
        {
            std::cout << "Total Amount: $0\n";
        }
        else
        {
            std::cout << "Total Amount: -$" << -total << "\n";
        }
    }
}

void SearchTransactions()
{
    int option = 0;
    std::cout << "---- Search Transactions ----\n";
    
    do
    {
        try
        {
            std::cout << "1. By Category\n2. By Description\n3. By Date\n\nChoose an option: ";
            std::cin >> option;

            if(option == 1 || option == 2 || option == 3)
            {
                option = option;
            }
            else
            {
                option = 0;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    } while (option == 0);
    
    if (option == 1)
    {
        std::string category;

        std::cout << "--- Transaction Category ---\n";

        std::cin.ignore();
        std::cout << "Transaction Category: ";
        std::getline(std::cin, category);

        int transactions_count = 0;

        for(Transaction& transaction : TRANSACTIONS)
        {
            if(transaction.Category() == category)
            {
                transaction.LogData();
                
                transactions_count++;
            }
        }

        if(transactions_count == 0)
        {
            std::cout << "No matching transactions found!\n";
        }
    }
    else if (option == 2)
    {
        std::string description;

        std::cout << "--- Transaction Description ---\n";

        std::cin.ignore();
        std::cout << "Transaction Description: ";
        std::getline(std::cin, description);

        int transactions_count = 0;

        for(Transaction& transaction : TRANSACTIONS)
        {
            if(transaction.Description() == description)
            {
                transaction.LogData();
                
                transactions_count++;
            }
        }

        if(transactions_count == 0)
        {
            std::cout << "No matching transactions found!\n";
        }
    }
    else if(option == 3)
    {
        std::vector<std::string> titles = {"Month Day: ", "Month: ", "Year: "};
        std::vector<int> date = {00, 00, 0000};

        std::cout << "--- Transaction Date ---\n";

        for(int i = 0; i < 3; i++)
        {
            std::cout << titles[i];
            std::cin >> date[i];
        }

        int transactions_count = 0;

        for(Transaction& transaction : TRANSACTIONS)
        {
            std::vector<int> transactionDate = transaction.Date();
            
            if(transactionDate[0] == date[0] && transactionDate[1] == date[1] && transactionDate[2] == date[2])
            {
                transaction.LogData();
                
                transactions_count++;
            }
        }

        if(transactions_count == 0)
        {
            std::cout << "No matching transactions found!\n";
        }
    }

}

void SetBudgetGoal()
{
    // Will be done later
}

void GenerateMonthlyReport()
{
    // Will be done later
}

void SaveTransactions()
{
    std::ofstream fileContent;
    fileContent.open(FILEPATH, std::ios::out | std::ios::trunc);

    if(fileContent.is_open())
    {
        for(Transaction& transaction : TRANSACTIONS)
        {
            fileContent << "" << transaction.Amount() << "[" << transaction.Category() << "[" << transaction.Description() << "[" << transaction.Date()[0] << "/" << transaction.Date()[1] << "/" << transaction.Date()[2] << "\n";
        }

        fileContent.close();
    }
    else
    {
        std::cerr << "Couldn't save data!\n";
    }
}

bool IsValidDate(int day, int month, int year)
{
    if(year < 0)
    {
        return false;
    }
    else
    {
        if(month > 12 || month < 1)
        {
            return false;
        }
        else
        {
            switch (month)
            {
                case 1:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 2:
                    if(day > 0 && day <= 28)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 3:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 4:
                    if(day > 0 && day <= 30)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 5:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 6:
                    if(day > 0 && day <= 30)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 7:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 8:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 9:
                    if(day > 0 && day <= 30)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 10:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 11:
                    if(day > 0 && day <= 30)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                case 12:
                    if(day > 0 && day <= 31)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                default:
                    return false;
                    break;
            }
        }
    }
}

std::vector<std::string> Split(std::string input, char breakOn)
{
    std::vector<std::string> output = {};
    std::string curString = "";

    for(int i = 0; i < input.length(); i++)
    {
        if(input[i] != breakOn)
        {
            curString += input[i];
        }
        else if(input[i] == breakOn)
        {
            output.push_back(curString);
            curString = "";
        }
    }

    output.push_back(curString);

    return output;
}
