#include <iostream>
#include <string>
#include <vector>
#include <string>
#include "../include/finance.hpp"

Transaction::Transaction(double Amount, std::string Category, std::string Description, std::vector<int> Date)
{
    this->amount = Amount;
    this->category = Category;
    this->description = Description;
    this->date = Date;
}

double Transaction::Amount()
{
    return this->amount;
}
std::string Transaction::Category()
{
    return this->category;
}
std::string Transaction::Description()
{
    return this->description;
}
std::vector<int> Transaction::Date()
{
    return this->date;
}
void Transaction::LogData()
{
    std::cout << "-- Transaction --\n";

    if(this->Amount() > 0)
    {
        std::cout << "Amount: +$" << this->Amount() << "\n";
    }
    else if(this->Amount() == 0)
    {
        std::cout << "Amount: $0\n";
    }
    else
    {
        std::cout << "Amount: -$" << -this->Amount() << "\n";
    }

    std::cout << "Category: " << this->Category() << "\n";
    std::cout << "Description: " << this->Description() << "\n";
    std::cout << "Date: " << this->Date()[0] << "/" << this->Date()[1] << "/" << this->Date()[2] << "\n";
}
