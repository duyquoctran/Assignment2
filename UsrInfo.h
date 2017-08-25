#ifndef USRINFO_H
#define USRINFO_H

// The user info will not be change once entered
// Only add and delete

#include <string>
#include <vector>

#include "Account.h"

enum mon{JAN=1,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEPT,OCT,NOV,DEC};
// File format
// ***** (userID) *****
// (accID)\t(accID)\t(accID)...
// (name)\t(occupation)
// (date of birth)
// (idn)\t(email)
const std::string INFOFILE = "UserInfo.txt";

// Structure for store date
struct date{
    int date, month, year;
};
// Infomation structure
struct info{
    std::string name, id, occupation, email;
    date DoB;
};

// This class contain info for current active account.
class Account;
class AccountsMgr;

class User{
protected:
    info user_info;
    unsigned int userID;
    std::vector<unsigned int> accountIDs;
public:
    // Getters
    info getInfo() const;
    unsigned int getUID() const;
    std::vector<unsigned int> getAIDList() const;
    //Setters
    void setInfo(info new_info);
    void setUID(unsigned int newUID);
    void addAccount(unsigned int accountID);
    void removeAccount(unsigned int accountID);
};

class UsersMgr{
protected:
    static std::vector<User> userList;
public:
    // Functions for managing the list of users
    void updateList();
    void saveList() const;
    void add(User user);
    void remove(unsigned int userID);
    // Getter - return a list of users
    std::vector<User> getUserList() const;
    // Functions for managing a user
    // Add a new user and push to the list
    // The user comes with no account
    void createNew(info user_info);
    // Bind an account to a user
    void attachAccount(unsigned int userID, unsigned int accountID);
    void detachAccount(unsigned int userID, unsigned int accountID);
    // Delete a user
    // The function will delete all accounts that bind with user
    // Along with the user itself
    void delUser(unsigned int userID);
};

#endif