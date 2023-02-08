#pragma once

#include <arrow/table.h>
#include <arrow/status.h>
#include <arrow/result.h>

arrow::Result<std::shared_ptr<arrow::Table>> VectorToColumnarTable(
        const std::vector<struct book_row>& rows);

arrow::Status XMLToTableConversion(const char* filename);
