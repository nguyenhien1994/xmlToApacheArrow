#include <arrow/status.h>
#include <iostream>

#include "conversion.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << argv[0] << " <bookstore xml path>" << std::endl;
        return EXIT_FAILURE;
    }

    if (XMLToTableConversion(argv[1]) != arrow::Status::OK()) {
        std::cerr << "Failed to run conversion!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
