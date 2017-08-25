#include <iostream>
#include <fstream>

#include "../include/Account.h"
// #include "../include/UsrInfo.h"

int main()
{
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    std::cout << Account::hashPwd(username, password) << std::endl;
    return 0;
}