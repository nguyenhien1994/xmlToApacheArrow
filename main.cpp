#include <arrow/pretty_print.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <iostream>

#include "bookstore.hpp"
#include "conversion.hpp"

arrow::Status RunRowConversion() {
    auto bookstore_rows = load_bookstore("bookstore.xml");

    std::shared_ptr<arrow::Table> table;

    ARROW_ASSIGN_OR_RAISE(table, VectorToColumnarTable(bookstore_rows));

    ARROW_RETURN_NOT_OK(arrow::PrettyPrint(*table, {}, &std::cerr));
    return arrow::Status::OK();
}

int main(){
    RunRowConversion();

    return EXIT_SUCCESS;
}
