#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#include "../include/Account.h"
#include "../include/UsrInfo.h"

// Define static member variable
std::vector<Account> AccountsMgr::List;

// RoleID class
// --------------------------------
// The class user default constructor, so we dont need to declare it
// Set roleID with user priviledge
void RoleID::setUSr(bool state){
    this -> usr = state;
}
// Set roleID with librarian priviledge
void RoleID::setLibra(bool state){
    this -> libra = state;
}
// Set roleID with system administrator priviledge
void RoleID::setSysadmin(bool state){
    this -> sysadmin = state;
}
// Return roleID in integer number
unsigned int RoleID::returnRID() const{
    unsigned int user = (unsigned int)(this -> usr);
    unsigned int librarian = (unsigned int)(this -> libra);
    unsigned int admin = (unsigned int)(this -> sysadmin);

    return (2*2*admin + 2*librarian + user);
}

// Account class
// --------------------------------
// Declare getters
std::string Account::getName() const{
    return this -> accountName;
}
std::string Account::getHash() const{
    return this -> hashed_pwd;
}
unsigned int Account::getAID() const{
    return this -> accountID;
}
bool Account::isActive() const{
    return this -> state;
}
unsigned int Account::getRID() const{
    return (this -> role).returnRID();
}
// Declare setters
void Account::setName(std::string name){
    this -> accountName = name;
}
void Account::setHash(std::string hash){
    this -> hashed_pwd = hash;
}
void Account::setAID(unsigned int accountID){
    this -> accountID = accountID;
}
void Account::setState(bool state){
    this -> state = state;
}
void Account::setRole(RoleID role){
    this -> role = role;
}
void Account::setRole(unsigned int roleID){
    // MSB = index 0
    bool bits[] = {false, false, false};
    unsigned int divisor = roleID;
    // Mod by 2 3 times
    for (int i = 0; i < 3; i++){
        bits[i] = (bool(divisor % 2));
        divisor = divisor / 2;
    }
    (this -> role).setUSr(bits[0]);
    (this -> role).setLibra(bits[1]);
    (this -> role).setSysadmin(bits[2]);
}

// Account management system
// --------------------------------
// Update or initialize a vector contains account info if not exists
void AccountsMgr::updateList(){
    std::fstream file_in;
    // Open file in input mode
    file_in.open(AFILE, std::ios::in);
    // Check if the file exists, if don't, create new file
    if(!file_in){
        // Open in output mode and truncate mode
        file_in.open(AFILE, std::ios::out | std::ios::trunc);
        // Reopen in input mode
        file_in.open(AFILE, std::ios::in);
    }
    // While loop for checking lines in the file
    std::string line;
    while(std::getline(file_in, line)){
        // Check if the line is ----- accID ------ 
        // First line of the account infomation block
        if (line[0] == '-'){
            // Initialize account info vars
            unsigned int AID, RID;
            std::string name, hash_pwd;
            bool activation;
            // Get info from first line - account id
            std::stringstream buffer1(line);
            std::string dump_str;
            buffer1 >> dump_str >> AID;
            // Get info from second line - name
            std::getline(file_in, name);
            // Third line - hash strings
            std::getline(file_in, hash_pwd);
            // Get info from fourth line - roleID and activation state
            file_in >> RID >> activation;
            // Create new account object and push into vector
            Account new_account;
            new_account.setAID(AID);
            new_account.setName(name);
            new_account.setHash(hash_pwd);
            new_account.setState(activation);
            new_account.setRole(RID);
            (this -> List).push_back(new_account);
        }
    }
}
// Save the accounts list to file
void AccountsMgr::saveList(){
    std::fstream file_out;
    file_out.open(AFILE, std::ios::out | std::ios::trunc);
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        file_out << "----- " << (this -> List)[i].getAID() << " -----" << "\n";
        file_out << (this -> List)[i].getName() << "\n";
        file_out << (this -> List)[i].getHash() << "\n";
        file_out << (this -> List)[i].getRID() << "\t";
        file_out << (int)(this -> List)[i].isActive() << "\n";
    }
}
// Add a new account to the list
void AccountsMgr::add(Account account){
    (this -> List).push_back(account);
}
// Remove account from the list
void AccountsMgr::remove(unsigned int accountID){
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        // Check for the accountID if they are correct
        // Otherwise do nothing
        if (accountID == (this -> List)[i].getAID()){
            (this -> List).erase((this -> List).begin() + i);
        }
    }
}
// Reset password for the account
std::string AccountsMgr::reset(Account account){
    std::string new_password;
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        // Check for the accountID if they are correct
        // Otherwise do nothing
        if (account.getAID() == (this -> List)[i].getAID()){
            // Seeding for new random character generator
            srand(time(NULL));
            // Loop RESETPWDSIZE times for new password
            for (int i = 0; i < RESETPWDSIZE; i++){
                new_password += rand_char();
            }
            std::string new_combination = (this -> List)[i].getName() + new_password;
            (this -> List)[i].setHash(AccountsMgr::xorCiphing(new_combination));
        }
    }
    return new_password;
}
// Return a list of Account - either all accounts or base on user
std::vector<Account> AccountsMgr::getList() const{
    return this -> List;
}
std::vector<Account> AccountsMgr::getList(User user) const{
    std::vector<Account> returnList;
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        std::vector<unsigned int> userAccountID = user.getAIDList();
        for (unsigned int j = 0; j < userAccountID.size(); j++){
            if (userAccountID[j] == (this -> List)[i].getAID()){
                returnList.push_back((this -> List)[i]);
            }
        }
    }
    
    return returnList;
}
std::vector<Account> AccountsMgr::getList(unsigned int userID) const{
    std::vector<Account> returnList;
    UsersMgr UManager;
    std::vector<User> UserList = UManager.getUserList();
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        for (unsigned int j = 0; j < UserList.size(); j++){
            if (userID == UserList[j].getUID()){
                std::vector<unsigned int> AIDs = UserList[j].getAIDList();
                for (unsigned int k = 0; k < AIDs.size(); k++){
                    if (AIDs[k] == (this -> List)[i].getAID()){
                        returnList.push_back((this -> List)[i]);
                    }
                }
            }
        }
    }
    
    return returnList;
}

// Static function member for hashing(encrypting) password
// It uses XOR cipher combine with ROT47
// The ROT47 maps from ASCII decimal 37(!) to 126(~)
// The function hashes the string of accname+pwd and return it to be used as 'hashedPwd'
std::string AccountsMgr::xorCiphing(std::string strings){
    // Use first character of pwd as cipher key
    char key = strings[0];
    // Combine account name and password to generate encryption
    // Start mapping ROT47 and
    // Hash the string by using XOR operator
    std::string xor_encrypted = "";
    for (unsigned int i = 0; i < strings.length(); i++){
        if (strings[i] == ' '){
            xor_encrypted += strings[i];
        }
        else{
            char rot47_mapped = rot47_to[rot47_from.find(strings[i])];
            xor_encrypted += rot47_mapped^key;
        }
    }
    std::string hashed_string;
    std::stringstream hex_conv;
    for (unsigned int i = 0; i < xor_encrypted.length(); i++){
        hex_conv << std::setw(2) << std::setfill('0') << std::hex << (int)xor_encrypted[i];
    }
    hashed_string = hex_conv.str();
    return hashed_string;
}
// Create new account function
// This function will create a new account and push back to the list
int AccountsMgr::createNew(std::string accname, std::string password, RoleID role, unsigned int userID){
    // Check for name duplication
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        if (accname == (this -> List)[i].getName()){
            return INVALID_NAME;
        }
    }
    // Begin create a new Account object
    // Set account name
    Account newAccount;
    newAccount.setName(accname);
    // Set account hashed password
    // Account encrypted password = encryption(accname + password)
    std::string combination = accname + password;
    newAccount.setHash(AccountsMgr::xorCiphing(combination));
    // Set roleID of the account
    newAccount.setRole(role);
    // Set account ID
    // The function will seek the largest AID in the list, then add 1 for the new account
    unsigned int accountID = 0;
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        unsigned int currentID = (this -> List)[i].getAID();
        accountID = (currentID > accountID) ? currentID : accountID;
    }
    accountID += 1;
    newAccount.setAID(accountID);
    // Activate the account
    newAccount.setState(true);
    // Push back to the list
    (this -> List).push_back(newAccount);
    // Bind the account to a user
    UsersMgr UserManager;
    UserManager.attachAccount(userID, accountID);
    return SUCCESS;
}
// Verify function
// Use to verify username and password
int AccountsMgr::verify(std::string accname, std::string password) const{
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        if (accname == (this -> List)[i].getName()){
            std::string combination = accname + password;
            std::string currentHash = AccountsMgr::xorCiphing(combination);
            if (currentHash == (this -> List)[i].getHash()){
                return SUCCESS;
            }
            else{
                return INVALID_PWD;
            }
        }
    }
    return INVALID_NAME;
}
// Change the account state - lock (true) or unlock (false)
void AccountsMgr::changeState(unsigned int accountID, bool state){
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        if (accountID == (this -> List)[i].getAID()){
            (this -> List)[i].setState(state);
        }
    }
}
// Remove the account
// This will delete the account that binds to the user
void AccountsMgr::delAccount(unsigned int userID, unsigned int accountID){
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        if (accountID == (this -> List)[i].getAID()){
            UsersMgr UManager;
            (this -> List).erase((this -> List).begin() + i);
            UManager.detachAccount(userID, accountID);
        }
    }
}
// Random character generator
// For more random character generated, use srand() before reset password
char rand_char(){
    int stringlen = sizeof(alphanum) - 1;
    return alphanum[rand() % stringlen];
}
// Change password function
int AccountsMgr::changePassword(unsigned int accountID, std::string oldpassword, std::string newpassword){
    for (unsigned int i = 0; i < (this -> List).size(); i++){
        if (accountID == (this -> List)[i].getAID()){
            std::string old_combination = (this -> List)[i].getName() + oldpassword;
            std::string old_hash = AccountsMgr::xorCiphing(old_combination);
            if (old_hash == (this -> List)[i].getHash()){
                std::string new_combination = (this -> List)[i].getName() + newpassword;
                (this -> List)[i].setHash(AccountsMgr::xorCiphing(new_combination));
                return SUCCESS;
            }
            else{
                return INVALID_PWD;
            }
        }
    }
    // Just in case
    return INVALID_AID;
}