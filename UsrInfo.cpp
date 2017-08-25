#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "../include/Account.h"
#include "../include/UsrInfo.h"

// Define static member
std::vector<User> UsersMgr::userList;
// User class
// ----------------------------
// Getters
info User::getInfo() const{
    return this -> user_info;
}
unsigned int User::getUID() const{
    return this -> userID;
}
std::vector<unsigned int> User::getAIDList() const{
    return this -> accountIDs;
}
// Setters
// Set info for the user
void User::setInfo(info new_info){
    (this -> user_info).name = new_info.name;
    (this -> user_info).id = new_info.id;
    (this -> user_info).occupation = new_info.occupation;
    (this -> user_info).email = new_info.email;
    // Set date of birth
    (this -> user_info).DoB.date = new_info.DoB.date;
    (this -> user_info).DoB.month = new_info.DoB.month;
    (this -> user_info).DoB.year = new_info.DoB.year;
}
// Set user ID for the user
void User::setUID(unsigned int newUID){
    this -> userID = newUID;
}
// Add new account to the user
void User::addAccount(unsigned int accountID){
    (this -> accountIDs).push_back(accountID);
}
// Remove new account to the user
void User::removeAccount(unsigned int accountID){
    for (unsigned int i = 0; i < (this -> accountIDs).size(); i++){
        if (accountID == (this -> accountIDs)[i]){
            (this -> accountIDs).erase((this -> accountIDs).begin() + i);
        }
    }
}

// UsersMgr class
// ---------------------------------
// Update/Initialize the users list
void UsersMgr::updateList(){
    // Open the file, if not exists, crreate new one
    std::fstream file_in;
    file_in.open(INFOFILE, std::ios::in);
    if(!file_in){
        file_in.open(INFOFILE, std::ios::out | std::ios::trunc);
        file_in.open(INFOFILE, std::ios::in);
    }
    std::string line;
    while(std::getline(file_in, line)){
        if (line[0] == '*'){
            std::string dump_str;
            info user;
            unsigned int userID;
            std::vector<unsigned int> AIDs;
            // Get userID of the first line
            std::stringstream buffer1(line);
            buffer1 >> dump_str >> userID;
            // Second line - account ID
            std::getline(file_in, line);
            std::stringstream buffer2(line);
            unsigned int tempAID;
            while(buffer2 >> tempAID){
                AIDs.push_back(tempAID);
            }
            // Third line - name and occupation (tab separated)
            std::getline(file_in, line);
            std::stringstream buffer3(line);
            std::getline(buffer3, user.name, '\t');
            std::getline(buffer3, user.occupation);
            // Fourth line - date of birth
            std::getline(file_in, line);
            std::stringstream buffer4(line);
            char dummy_slash;
            buffer4 >> user.DoB.date >> dummy_slash
                    >> user.DoB.month >> dummy_slash
                    >> user.DoB.year;
            // Fifth line - id number and email
            std::getline(file_in, line);
            std::stringstream buffer5(line);
            std::getline(buffer5, user.id, '\t');
            std::getline(buffer5, user.email);
            // Push new user object to list
            User currentUser;
            currentUser.setInfo(user);
            currentUser.setUID(userID);
            for (unsigned int i = 0; i < AIDs.size(); i++){
                currentUser.addAccount(AIDs[i]);
            }
            (this -> userList).push_back(currentUser);
        }
    }
}
// Save the list to a file
void UsersMgr::saveList() const{
    // Open file in output and truncate mode
    std::fstream file_out;
    file_out.open(INFOFILE, std::ios::out | std::ios::trunc);
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        // Push the infomation of the user to file
        file_out << "***** " << (this -> userList)[i].getUID() << " *****" << "\n";
        std::vector<unsigned int> AIDs = (this -> userList)[i].getAIDList();
        for (unsigned int j = 0; j < AIDs.size(); j++){
            file_out << AIDs[j] << "\t";
        }
        file_out << "\n";
        info user = (this -> userList[i].getInfo());
        file_out << user.name << "\t" << user.occupation << "\n";
        file_out << user.DoB.date << "-" << user.DoB.month << "-" << user.DoB.year << "\n";
        file_out << user.id << "\t" << user.email << "\n";
    }
}
// Get user list function
// Return a vector of users
std::vector<User> UsersMgr::getUserList() const{
    return (this -> userList);
}
// Add user function
void UsersMgr::add(User user){
    (this -> userList).push_back(user);
}
// Remove user from list function
void UsersMgr::remove(unsigned int userID){
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        if (userID == (this -> userList)[i].getUID()){
            (this -> userList).erase((this -> userList).begin() + i);
        }
    }
}
// Create a new user and add it to the list
void UsersMgr::createNew(info user_info){
    // First we get the largest userID, then add 1
    // Then we use it as a new userID for this account
    unsigned int userID = 0;
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        unsigned int currentID = (this -> userList)[i].getUID();
        userID = (currentID > userID) ? currentID : userID;
    }
    userID += 1;
    // Push the new user to list
    User newUser;
    newUser.setInfo(user_info);
    newUser.setUID(userID);
    (this -> userList).push_back(newUser);
}

// Delete user function
void UsersMgr::delUser(unsigned int userID){
    // First we seek the account then delete it first
    // Then we remove the user out of the list
    AccountsMgr AManager;
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        if (userID == (this -> userList)[i].getUID()){
            // Get the list of account IDs
            std::vector<unsigned int> AIDs = (this -> userList)[i].getAIDList();
            // Delete accounts
            for (unsigned int j = 0; j < AIDs.size(); j++){
                AManager.remove(AIDs[j]);
            }
            // Delete the user
            (this -> userList).erase((this -> userList).begin() + i);
        }
    }
}
// Binding function
// Add account ID to a specific user using user ID
void UsersMgr::attachAccount(unsigned int userID, unsigned int accountID){
    // First we find a user using user ID
    // If not found, do nothing
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        if (userID == (this -> userList)[i].getUID()){
            (this -> userList)[i].addAccount(accountID);
        }
    }
}
// Remove account ID to a specific user using user ID
void UsersMgr::detachAccount(unsigned int userID, unsigned int accountID){
    for (unsigned int i = 0; i < (this -> userList).size(); i++){
        if (userID == (this -> userList)[i].getUID()){
            (this -> userList)[i].removeAccount(accountID);
        }
    }
}