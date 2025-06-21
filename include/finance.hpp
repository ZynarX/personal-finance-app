#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <map>

class Transaction
{
private:
    double amount;
    std::string category;
    std::string description;
    std::vector<int> date;
public:
    Transaction(double Amount, std::string Category, std::string Description, std::vector<int> Date);

    double Amount();
    std::string Category();
    std::string Description();
    std::vector<int> Date();

    void LogData();
};
