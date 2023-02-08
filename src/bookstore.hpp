#pragma once

struct book_edition {
    std::string isbn;
    uint32_t year;
    float price;
};

struct book_row {
    std::string category;
    std::string title;
    std::string lang;
    std::string author;
    std::vector<book_edition> editions;
};

std::vector<book_row> LoadBookstoreFromXML(const char* filename);
