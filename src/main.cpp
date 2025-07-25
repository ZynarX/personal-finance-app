#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include "../include/finance.hpp"

#define APPNAME "Personal Finance Tracker"
#define FILEPATH "data.txt"
#define MIN_OPTION 1
#define MAX_OPTION 6

std::vector<Transaction> TRANSACTIONS;
double BUDGET_GOAL;

void ShowMenu();
int GetOption(int min, int max);
void HandleOption(int option);

void LoadTransactions();
void AddTransaction();
void ViewAllTransactions();
void SearchTransactions();

void SearchByCategory();
void SearchByDescription();
void SearchByDate();

void SetBudgetGoal();
void GenerateMonthlyReport();
void SaveTransactions();
bool IsValidDate(int day, int month, int year);
std::vector<std::string> Split(std::string input, char breakOn);

int main()
{
    LoadTransactions();

    ShowMenu();

    SaveTransactions();
    
    return 0;
}

void ShowMenu()
{
    int option;

    do
    {
        try
        {
            option = GetOption(MIN_OPTION, MAX_OPTION);

            HandleOption(option);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    } while (option != 6);
}

int GetOption(int min, int max)
{
    int option;

    do
    {
        try
        {
            std::cout << "======== " << APPNAME << " ========\n";
            std::cout << "1. Add Transaction\n2. View All Transactions\n3. Search Transactions\n4. Set Budget Goal\n5. Generate Monthly Report\n6. Save & Exit\n\nChoose an option: ";
            std::cin >> option;
        }
        catch(const std::exception& e)
        {
            std::cout << "Invalid input!\n";
        }
    } while (option < min || option > max);
    
    return option;
}

void HandleOption(int option)
{
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

void LoadTransactions()
{
    size_t min_size = 0;
    size_t budget_index = 0;
    std::vector<std::string> data = {};

    std::ifstream fileContent;
    fileContent.open(FILEPATH);

    if(fileContent.is_open())
    {
        std::string curLine;

        while(std::getline(fileContent, curLine))
        {
            data.push_back(curLine);
        }

        fileContent.close();
    }
    else
    {
        std::cerr << "Couldn't find file!\n";
    }

    if(data.size() > min_size)
    {
        for(size_t i = 0; i < data.size(); i++)
        {
            if(i == budget_index)
            {
                BUDGET_GOAL = std::stod(data[i]);
            }
            else
            {
                std::string curData = data[i];
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
            }
        }
    }
}

void AddTransaction()
{
    bool is_valid;

    do
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

        do
        {
            std::cin.ignore();
            std::cout << "Transaction Category: ";
            std::getline(std::cin, category);
        } while(category == "");

        do
        {
            std::cin.ignore();
            std::cout << "Transaction Description: ";
            std::getline(std::cin, description);
        } while(description == "");

        do
        {
            try
            {
                std::cout << "Transaction Date Month Day: ";
                std::cin >> date[0];
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        } while(date[0] == 0);

        do
        {
            try
            {
                std::cout << "Transaction Date Month: ";
                std::cin >> date[1];
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        } while(date[1] == 0);

        do
        {
            try
            {
                std::cout << "Transaction Date Year: ";
                std::cin >> date[2];
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        } while(date[2] == 0);

        is_valid = IsValidDate(date[0], date[1], date[2]);

        if(is_valid)
        {
            Transaction newTransaction(amount, category, description, date);
            TRANSACTIONS.push_back(newTransaction);
        }
        else
        {
            std::cout << "Invalid date!\n";
        }
    } while(is_valid == false);
}

void ViewAllTransactions()
{
    size_t min_size = 1;

    if(TRANSACTIONS.size() < min_size)
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
        SearchByCategory();
    }
    else if (option == 2)
    {
        SearchByDescription();
    }
    else if(option == 3)
    {
        SearchByDate();
    }

}

void SearchByCategory()
{
    try
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
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void SearchByDescription()
{
    try
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
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void SearchByDate()
{
    try
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
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void SetBudgetGoal()
{
    try
    {
        std::cout << "Enter your monthly budget goal: ";
        std::cin >> BUDGET_GOAL;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void GenerateMonthlyReport()
{
    // Will be done later
}

void SaveTransactions()
{
    try
    {
        std::ofstream fileContent;
        fileContent.open(FILEPATH, std::ios::out | std::ios::trunc);

        if(fileContent.is_open())
        {
            fileContent << BUDGET_GOAL << '\n';

            for(Transaction& transaction : TRANSACTIONS)
            {
                fileContent << "" << transaction.Amount() << "[" << transaction.Category() << "[" << transaction.Description() << "[" << transaction.Date()[0] << "/" << transaction.Date()[1] << "/" << transaction.Date()[2] << '\n';
            }

            fileContent.close();
        }
        else
        {
            std::cerr << "Couldn't save data!\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

bool IsValidDate(int day, int month, int year)
{
    if(year < 0 || year > 2100)
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
            int daysInMoth[] = {
                0,
                31,
                29,
                31,
                30,
                31,
                30,
                31,
                31,
                30,
                31,
                30,
                31
            };
            
            if((year % 400 == 0 && year % 100 != 0) || (year % 400 == 0))
            {
                
                daysInMoth[2] = 29;
            }
            else
            {
                daysInMoth[2] = 29;
            }

            if(day < 1 || day > daysInMoth[month])
            {
                return false;
            }
            else
            {
                return true;
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
