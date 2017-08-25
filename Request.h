#ifndef REQUEST_MODULE
#define REQUEST_MODULE

#include <vector>
#include <string>

// Exit code for ticket respond
enum ticket_respond{SUCCESSFUL, OUT_OF_BOOK};

// File name const variable
const std::string REQFILE = "TicketSubmissions.txt";
// File format
// .......
// (TicketID)\t(accountID)\t(BookID)\n
// (pending)\t(accepted)\n
// .......

// Struct ticket
struct Ticket{
    unsigned int id;
    unsigned int accountID;
    unsigned int bookID;
    bool pending;
    bool accepted;
};

// Request class
// This class will handle all the lists of ticket information
class Request{
protected:
    static std::vector<Ticket> TicketsList;
public:
    // Retrieving and saving functions
    void updateList();
    void saveList() const;
    // Ticket request function
    int  RequestBook(unsigned int accountID, unsigned int bookID);
    // Ticket respond functions
    void DenyTicket(unsigned int TicketID);
    void AcceptTicket(unsigned int TicketID);
    // Return function, for others function/class to use
    std::vector<Ticket> returnList() const;
    // This function will return an array of strings, which is a message showing
    // The status of the submitted ticket of an account
    std::vector<std::string> fetchMessages(unsigned int accountID);
};

#endif