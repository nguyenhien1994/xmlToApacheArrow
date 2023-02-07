#pragma once

arrow::Result<std::shared_ptr<arrow::Table>> VectorToColumnarTable(
        const std::vector<struct book_row>& rows);