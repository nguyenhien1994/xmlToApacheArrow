#pragma once

#include <vector>
#include <string>

struct book_edition {
    std::string isbn;
    int year;
    float price;
};

struct book_row {
    std::string category;
    std::string title;
    std::string lang;
    std::string author;
    std::vector<book_edition> editions;
};

std::vector<book_row> load_bookstore(std::string filename);
