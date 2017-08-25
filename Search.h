#ifndef SEARCH_MODULE_H
#define SEARCH_MODULE_H

#include <string>
#include <vector>
#include "Account.h"

namespace search{
    unsigned int levenshtein_distance(const std::string& s1, const std::string& s2);
    std::vector<std::string> searchBook(std::string keyword);
    std::vector<std::string> searchAcc(Account access_acc, std::string keyword);
}


#endif