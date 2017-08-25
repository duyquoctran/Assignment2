#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../include/Book.h"
using namespace std;

std::vector<Book> BooksCase::BooksList;
const string BFILE = "Booklist.txt";

void BooksCase::AddBook(Book book_info) {
	unsigned int bookID = 0;												// Set new BookID 
	for (unsigned int i = 0; i < BooksList.size(); i++) {					
		if (BooksList[i].BookID > bookID) bookID = BooksList[i].BookID;		// Find the highest BookID
	}																		
	bookID += 1;															// New BookId = Highest_BookID + 1
	book_info.BookID = bookID;
	BooksList.push_back(book_info);											// Have new book at the end of the list
}
void BooksCase::DeleteBook(unsigned int bookID) {
	for (int i = 0; i < BooksList.size(); i++) {							// Find BookID
		if (BooksList[i].BookID == bookID) {
			BooksList.erase(BooksList.begin() + i);							// Erase the book with the BookID found
		}
	}
}
void BooksCase::EditBook(Book book_info, unsigned int bookID) {
	for (int i = 0; i < BooksList.size(); i++) {							// Find BookID
		if (BooksList[i].BookID == bookID) {
			BooksList[i].Bookname = book_info.Bookname;						// Update new Bookname
			BooksList[i].Publisher = book_info.Publisher;					// Update new Publisher
			BooksList[i].Type = book_info.Type;								// Update new Type
			BooksList[i].Remain = book_info.Remain;							// Update new Remain
			BooksList[i].Total = book_info.Total;							// Update new Total
		}
	}
}
void BooksCase::UpdateList() {
	fstream file_in;
	file_in.open(BFILE, ios::in);
	if (!file_in) {
		file_in.open(BFILE, ios::out | ios::trunc);
		file_in.open(BFILE, ios::in);
	}
	string line;
	while (getline(file_in, line)) {
		unsigned int _BookID = 0, _Remain = 0, _Total = 0;				
		string _Bookname, _Publisher, _Type;
		stringstream ss1(line);											// get line 1 : BookID
		ss1 >> _BookID;													// _BookID = BookID 
		getline(file_in, _Bookname);									// _Bookname = Bookname	
		getline(file_in, _Publisher);									// _Publisher = Publisher
		getline(file_in, _Type);
		getline(file_in, line);											// _Type = Type
		stringstream ss2(line);											// get line 2 : Remain 
		ss2 >> _Remain;
		getline(file_in, line);													// _Remain = Remain
		stringstream ss3(line);											// get line 3 : Total
		ss3 >> _Total;													// _Total = Total
		Book book_info;													// insert book data
		book_info.BookID	= _BookID;
		book_info.Bookname	= _Bookname;
		book_info.Publisher = _Publisher;
		book_info.Type		= _Type;
		book_info.Remain	= _Remain;
		book_info.Total		= _Total;
		BooksList.push_back(book_info);
	}
}
void BooksCase::SaveList() const{
	ofstream file_out;
	file_out.open(BFILE, std::ios::out | std::ios::trunc);
	if (file_out.is_open())
	{
		for (int i = 0; i < BooksList.size(); i++) {
			file_out << BooksList[i].BookID		<< "\n";				
			file_out << BooksList[i].Bookname	<< "\n";
			file_out << BooksList[i].Publisher	<< "\n";
			file_out << BooksList[i].Type		<< "\n";
			file_out << BooksList[i].Remain		<< "\n";
			file_out << BooksList[i].Total		<< "\n";
		}
	}
}
vector<Book> BooksCase::returnList() const {
	return BooksList;														
}
// Deposit/return the book
void BooksCase::IncreaseBook(unsigned int bookID) {
	for (int i = 0; i < BooksList.size(); i++) {
		if (BooksList[i].BookID == bookID) {
			BooksList[i].Remain++;
			break;
		}
	}
}
// Lend the book - Only 1 at a time
void BooksCase::DecreaseBook(unsigned int bookID) {
	for (int i = 0; i < BooksList.size(); i++) {
		if (BooksList[i].BookID == bookID) {
			if (BooksList[i].Remain > 0) {
				BooksList[i].Remain--;
				break;
			}
		}
	}
}