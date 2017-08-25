#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "../include/Book.h"
#include "../include/Request.h"

using namespace std;

vector<Ticket> Request::TicketsList;

void Request::saveList() const{
	ofstream file_out;
	file_out.open(REQFILE, std::ios::out | std::ios::trunc);
	if (file_out.is_open())
	{
		for (int i = 0; i < TicketsList.size(); i++) {
			file_out << TicketsList[i].id		 << "\t";
			file_out << TicketsList[i].accountID << "\t";
			file_out << TicketsList[i].bookID	 << "\n";
			file_out << TicketsList[i].pending	 << "\t";
			file_out << TicketsList[i].accepted	 << "\n";
		}
	}
}
void Request::updateList() {
	fstream file_in;
	file_in.open(REQFILE, ios::in);
	if (!file_in) {
		file_in.open(REQFILE, ios::out | ios::trunc);
		file_in.open(REQFILE, ios::in);
	}
	if (file_in.is_open()) {
		string line;
		while (getline(file_in, line)) {
			unsigned int _id, _accountID, _bookID;
			bool _pending, _accepted;
			stringstream ss1(line);
			ss1 >> _id >> _accountID >> _bookID;
			getline(file_in, line);
			stringstream ss2(line);
			ss2 >> _pending >> _accepted;
			Ticket ticket{ _id,_accountID,_bookID,_pending,_accepted };
			TicketsList.push_back(ticket);
		}
	}
}
int Request::RequestBook(unsigned int accountID, unsigned int bookID) {
	BooksCase book_manager;
	vector<Book> BooksList = book_manager.returnList();
	for (int i = 0; i < BooksList.size(); i++) {
		if (BooksList[i].BookID == bookID) {
			if (BooksList[i].Remain > 0) {
				Ticket ticket;
				ticket.id = 0;
				for (int j = 0; j < TicketsList.size(); j++) {
					if (TicketsList[j].id > ticket.id) ticket.id = TicketsList[j].id;
				}
				ticket.id++;
				ticket.accountID = accountID;
				ticket.bookID = bookID;
				ticket.pending = true;
				ticket.accepted = false;
				TicketsList.push_back(ticket);
				book_manager.DecreaseBook(bookID);
				return SUCCESSFUL;
			}
			else {
				return OUT_OF_BOOK;
			}
		}
	}
}
void Request::DenyTicket(unsigned int TicketID) {
	BooksCase book_manager;
	for (int i = 0; i < TicketsList.size(); i++) {
		if (TicketsList[i].id == TicketID) {
			TicketsList[i].pending = false;
			TicketsList[i].accepted = false;
			book_manager.IncreaseBook(TicketsList[i].bookID);
		}
	}
}
void Request::AcceptTicket(unsigned int TicketID) {
	for (int i = 0; i < TicketsList.size(); i++) {
		if (TicketsList[i].id == TicketID) {
			TicketsList[i].pending = false;
			TicketsList[i].accepted = true;
		}
	}
}
vector<Ticket> Request::returnList() const{
	return this -> TicketsList;
}
vector<string> Request::fetchMessages(unsigned int accountID){
	BooksCase book_manager;
	vector<string> messages;
	vector<Book> booklist = book_manager.returnList();
	for (unsigned int i = 0; i < TicketsList.size(); i++){
		if (accountID == TicketsList[i].accountID){
			stringstream text;
			string book_name;
			for (unsigned int j = 0; j < booklist.size(); j++){
				if (booklist[j].BookID == TicketsList[i].bookID){
					book_name = booklist[j].Bookname;
					break;
				}
			}
			string ticket_status;
			if (TicketsList[i].pending){
				ticket_status = "PENDING";
			}
			else{
				ticket_status = (TicketsList[i].accepted) ? "ACCEPTED" : "DENIED";
			}
			text << "ID: " << TicketsList[i].id
				 << " --- Book name: " << book_name 
				 << " --- Status: " << ticket_status;
			messages.push_back(text.str());
		}
	}
	return messages;
}
