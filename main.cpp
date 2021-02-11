#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

string toLowerCase(string string) {
    for (char &c : string) {
        c = tolower(c);
    }

    return string;
}

struct LibraryFullException : std::exception {
private:
    string message;
public:
    explicit LibraryFullException(const string &message) {
        this->message = message;
    }

    const char *what() const noexcept override {
        return message.c_str();
    }
};

struct BookNotFoundException : std::exception {
private:
    string message;
public:
    explicit BookNotFoundException(const string &message) {
        this->message = message;
    }

    const char *what() const noexcept override {
        return message.c_str();
    }
};

struct Book {
    int id;
    string title;
    int pages;
    float price;
    bool valid = false;

    static Book buildInteractively() {
        return editInteractively(Book{});
    }

    static Book editInteractively(Book book) {
        cout << "Please insert the book's title: ";
        cin >> book.title;

        cout << "Please insert the amount of pages: ";
        cin >> book.pages;

        cout << "Please insert the price of the book: ";
        cin >> book.price;

        book.valid = true;

        return book;
    }

    static Book searchInteractively() {
        while (true) {
            string input;

            cout << "Please insert the book's title/id: ";
            cin >> input;

            try {
                try {
                    int id = stoi(input);

                    return Book::search(id);
                } catch (invalid_argument &ignored) {
                    return Book::search(input);
                }
            } catch (BookNotFoundException &e) {
                cout << e.what() << endl;
            }
        }
    }

    static Book search(string title);

    static Book search(int id);

    void remove() const;

    void add();

    bool exists() const;

    void display() {
        cout << "Information for book #" << this->id << endl << endl;
        cout << "Title: " << this->title << endl;
        cout << "Pages: " << this->pages << endl;
        cout << "Price: " << this->price << "$" << endl;
    }

    bool equals(const Book &book) const {
        return book.id == this->id;
    }
};

const int SIZE = 10000;

Book books[SIZE];

void Book::remove() const {
    for (int i = 0; i < SIZE; i++) {
        if (this->equals(books[i])) {
            for (int k = i; k < SIZE - 1; k++) {
                books[k] = books[k + 1];
            }

            books[SIZE - 1] = Book{};
            i--;
        }
    }
}

void Book::add() {
    for (int i = 0; i < SIZE; i++) {
        if (books[i].valid) {
            continue;
        }

        this->id = i;
        books[i] = *this;

        return;
    }

    throw LibraryFullException("The library is already full!");
}

bool Book::exists() const {
    for (const auto &book : books) {
        if (this->equals(book)) {
            return true;
        }
    }

    return false;
}

Book Book::search(int id) {
    for (const auto &book : books) {
        if (book.id == id) {
            return book;
        }
    }

    throw BookNotFoundException("Book with ID " + to_string(id) + " not found!");
}

Book Book::search(string title) {
    title = toLowerCase(title);

    for (const auto &book : books) {
        if (toLowerCase(book.title) == title) {
            return book;
        }
    }

    throw BookNotFoundException("Book with title '" + title + "' not found!");
}

int getLibrarySize() {
    int size = 0;

    for (const auto &book : books) {
        if (book.valid) {
            size++;
        }
    }

    return size;
}

void displaySpacer() {
    cout << "-----------------------------------------" << endl;
}

void displayMenu() {
    cout << endl;
    displaySpacer();
    cout << "The library currently contains " << getLibrarySize() << " books." << endl;
    cout << "Please choose from the following options:" << endl;
    cout << "n - Adds a new book to the store." << endl;
    cout << "r - Removes a book from the store." << endl;
    cout << "s - Search for a book and display its information." << endl;
    cout << "l - List all books." << endl;
    cout << "f - List all books and their information." << endl << endl;
    cout << "> ";

    char choice;
    cin >> choice;

    cout << endl;

    switch (choice) {
        case 'n': {
            try {
                Book book = Book::buildInteractively();
                book.add();

                cout << "Book added successfully." << endl;
            } catch (LibraryFullException &e) {
                cout << "The library is full! Sorry :(" << endl;
            }
            break;
        }
        case 'r': {
            Book book = Book::searchInteractively();
            book.remove();

            cout << "Book removed successfully." << endl;
            break;
        }
        case 's': {
            Book book = Book::searchInteractively();
            book.display();
            break;
        }
        case 'l': {

        }
    }

}

int main() {
    srand(time(nullptr));
    cout << "Bookstore Manager v0.1-DEV" << endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
