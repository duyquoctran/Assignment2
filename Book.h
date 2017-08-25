#ifndef BOOK_MODULE
#define BOOK_MODULE

#include <string>
#include <vector>
using namespace std;


typedef struct {
	unsigned int BookID;
	string Bookname;
	string Type;
	string Publisher;
	unsigned int Remain, Total;
}Book;

class BooksCase {
protected:
	static vector<Book> BooksList;
public:
	void AddBook(Book book_info);
	void EditBook(Book book_info, unsigned int BookID);
	void DeleteBook(unsigned int BookID);
	void UpdateList();
	void SaveList() const;
	void IncreaseBook(unsigned int bookID);
	void DecreaseBook(unsigned int bookID);
	vector<Book> returnList() const;
};


#endif