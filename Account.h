#ifndef ACCOUNT_H
#define ACCOUNT_H

#define RESETPWDSIZE 8

#include <string>
#include <vector>

// File format:
// ----- (accID) -----
// (accname)
// (hash)
// (roleID)\t(activate)
const std::string AFILE = "AccountList.txt";
// Return code: SUCCESS, ACCESS_ERR(not a sysadmin account)
// INVALID_NAME (name already avalable/not exists)
// INVALID_PWD (wrong passsword)
enum exit_code{
    SUCCESS, FAIL, INVALID_NAME, INVALID_UID, 
    INVALID_AID ,ACCESS_ERR, INVALID_PWD
};
// String mapping for ROT47 hash
const std::string rot47_from = "!\"#$%&\'()*+,-./0123456789"
    ":;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
    "abcdefghijklmnopqrstuvwxyz{|}~";
const std::string rot47_to = "PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz"
    "{|}~!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO";
// Alphanumeric character for password reset
const char alphanum[] = "012345678"
    "!@#$&"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

class User;
class UsersMgr;
// The roleID class system
// It represents 3 bits number system, MSB would be highest priviledge role
// You can have multiple roles
// The role always start
class RoleID{
protected:
    bool sysadmin = false;
    bool libra = false;
    bool usr = false;
public:
    void setUSr(bool state);
    void setLibra(bool state);
    void setSysadmin(bool state);
    // The return function will return number in decimals
    // Example: 110 (Sysadmin/Librarian) will return 6
    unsigned int returnRID() const;
};

class Account{
protected:
    std::string accountName;
    std::string hashed_pwd;
    unsigned int accountID;
    bool state;
    RoleID role;
public:
    // Getter
    std::string getName() const;
    std::string getHash() const;
    unsigned int getAID() const;
    bool isActive() const;
    unsigned int getRID() const;
    // Setters
    void setName(std::string name);
    void setHash(std::string hash);
    void setAID(unsigned int accountID);
    void setState(bool state);
    void setRole(unsigned int roleID);
    void setRole(RoleID role);
};

class AccountsMgr{
protected:
    static std::vector<Account> List;
public:
    // Accounts list management functions
    void updateList();
    void saveList();
    void add(Account account);
    void remove(unsigned int accountID);
    std::vector<Account> getList() const;
    std::vector<Account> getList(User user) const;
    std::vector<Account> getList(unsigned int userID) const;
    // Account management functions
    int createNew(std::string accname, std::string password, RoleID role, unsigned int userID);
    std::string reset(Account account);
    int verify(std::string accname, std::string password) const;
    // Changing password function
    int changePassword(unsigned int accountID, std::string oldpassword, std::string newpassword);
    void changeState(unsigned int accountID, bool state);
    void delAccount(unsigned int userID, unsigned int accountID);
    // Password encryption method
    static std::string xorCiphing(std::string strings);
};

// Random character generator
char rand_char();

#endif