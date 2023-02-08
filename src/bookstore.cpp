#include <iostream>
#include <vector>
#include <string>

#include "bookstore.hpp"
#include "../tinyxml2/tinyxml2.h"

void print_book(const std::vector<book_row>& book_store) {
    for (const auto& book : book_store) {
        std::cout << "category: " << book.category << "\n";
        std::cout << "title: " << book.title << "\n";
        std::cout << "lang: " << book.lang << "\n";
        std::cout << "author: " << book.author << "\n";
        for (const auto& edition : book.editions) {
            std::cout << "\tisbn: " << edition.isbn << "\n";
            std::cout << "\tyear: " << edition.year << "\n";
            std::cout << "\tprice: " << edition.price << "\n";
        }
        std::cout << "\n";
    }
}

book_row parse(tinyxml2::XMLElement * p_book) {
    auto get_value = [&p_book](const char* name) -> auto {
        tinyxml2::XMLElement* ptr = p_book->FirstChildElement(name);
        if (ptr) {
            return ptr->GetText();
        }
        return "";
    };

    auto get_attribute_value = [&p_book](const char* tag_name, const char* attribute_name) -> auto {
        if (tag_name) {
            tinyxml2::XMLElement* ptr = p_book->FirstChildElement(tag_name);
            return ptr->Attribute(attribute_name);
        }
        return p_book->Attribute(attribute_name);
    };

    book_row book;
    book.title = get_value("title");
    book.author = get_value("author");
    book.category = get_attribute_value(nullptr, "category");
    book.lang = get_attribute_value("title", "lang");

    auto p_edittions = p_book->FirstChildElement("editions");
    if (p_edittions) {
        auto p_edittion = p_edittions->FirstChildElement("edition");
        while (p_edittion) {
            book_edition edition;
            tinyxml2::XMLElement* p_isbp = p_edittion->FirstChildElement("isbn");
            if (p_isbp) {
                edition.isbn = p_isbp->GetText();
            }

            tinyxml2::XMLElement* p_year = p_edittion->FirstChildElement("year");
            if (p_year) {
                p_year->QueryIntText(&edition.year);
            }

            tinyxml2::XMLElement* p_price = p_edittion->FirstChildElement("price");
            if (p_price) {
                p_price->QueryFloatText(&edition.price);
            }

            book.editions.emplace_back(std::move(edition));

            p_edittion = p_edittion->NextSiblingElement("edition");
        }
    }

    return std::move(book);
}

std::vector<book_row> load_bookstore(const char* filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename);
    std::vector<book_row> bookstore;

    tinyxml2::XMLElement * p_root_element = doc.RootElement();
    tinyxml2::XMLElement * p_book = (p_root_element != nullptr) ? p_root_element->FirstChildElement("book") : nullptr; 

    while(p_book){
        auto b = parse(p_book);
        bookstore.push_back(b);
        p_book = p_book->NextSiblingElement("book");
    }

    print_book(bookstore);

    return bookstore;
}
