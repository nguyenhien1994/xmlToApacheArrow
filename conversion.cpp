#include <arrow/api.h>
#include <arrow/result.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "bookstore.hpp"

using arrow::FloatBuilder;
using arrow::UInt32Builder;
using arrow::ListBuilder;
using arrow::StringBuilder;
using arrow::StructBuilder;
using arrow::ArrayBuilder;

arrow::Result<std::shared_ptr<arrow::Table>> VectorToColumnarTable(
        const std::vector<struct book_row>& bookstore_rows) {
    // The builders are more efficient using
    // arrow::jemalloc::MemoryPool::default_pool() as this can increase the size of
    // the underlying memory regions in-place. At the moment, arrow::jemalloc is only
    // supported on Unix systems, not Windows.
    arrow::MemoryPool* pool = arrow::default_memory_pool();
    std::vector<std::shared_ptr<arrow::Field>> editions_fields = {
        arrow::field("isbn", arrow::utf8()),
        arrow::field("year", arrow::uint32()),
        arrow::field("price", arrow::float32())
    };

    auto category_builder = std::make_shared<StringBuilder>(pool);
    auto title_builder = std::make_shared<StringBuilder>(pool);
    auto lang_builder = std::make_shared<StringBuilder>(pool);
    auto author_builder = std::make_shared<StringBuilder>(pool);
    auto isbn_builder = std::make_shared<StringBuilder>(pool);
    auto uint_builder = std::make_shared<UInt32Builder>(pool);
    auto float_builder = std::make_shared<FloatBuilder>(pool);
    std::vector<std::shared_ptr<ArrayBuilder>> vec_array_builder{isbn_builder, uint_builder, float_builder};
    auto struct_builder = std::make_shared<StructBuilder>(arrow::struct_(editions_fields), pool, vec_array_builder);
    auto list_builder = std::make_shared<ListBuilder>(pool, struct_builder, arrow::list(arrow::struct_(editions_fields)));

    // Now we can loop over our existing data and insert it into the builders. The
    // `Append` calls here may fail (e.g. we cannot allocate enough additional memory).
    // Thus we need to check their return values. For more information on these values,
    // check the documentation about `arrow::Status`.
    for (const auto& row : bookstore_rows) {
        ARROW_RETURN_NOT_OK(category_builder->Append(row.category));
        ARROW_RETURN_NOT_OK(title_builder->Append(row.title));
        ARROW_RETURN_NOT_OK(lang_builder->Append(row.lang));
        ARROW_RETURN_NOT_OK(author_builder->Append(row.author));
        ARROW_RETURN_NOT_OK(list_builder->Append());
        for (const auto& edition : row.editions) {
            ARROW_RETURN_NOT_OK(struct_builder->Append());
            ARROW_RETURN_NOT_OK(isbn_builder->Append(edition.isbn));
            ARROW_RETURN_NOT_OK(uint_builder->Append(edition.year));
            ARROW_RETURN_NOT_OK(float_builder->Append(edition.price));
        }
    }

    // At the end, we finalise the arrays, declare the (type) schema and combine them
    // into a single `arrow::Table`:
    std::shared_ptr<arrow::Array> category_array;
    std::shared_ptr<arrow::Array> title_array;
    std::shared_ptr<arrow::Array> lang_array;
    std::shared_ptr<arrow::Array> author_array;
    std::shared_ptr<arrow::Array> editions_array;

    ARROW_RETURN_NOT_OK(category_builder->Finish(&category_array));
    ARROW_RETURN_NOT_OK(title_builder->Finish(&title_array));
    ARROW_RETURN_NOT_OK(lang_builder->Finish(&lang_array));
    ARROW_RETURN_NOT_OK(author_builder->Finish(&author_array));
    ARROW_RETURN_NOT_OK(list_builder->Finish(&editions_array));

    std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
        arrow::field("category", arrow::utf8()),
        arrow::field("title", arrow::utf8()),
        arrow::field("lang", arrow::utf8()),
        arrow::field("author", arrow::utf8()),
        arrow::field("editions",  arrow::list(
            arrow::struct_(editions_fields)
        ))};

    auto schema = std::make_shared<arrow::Schema>(schema_vector);

    // The final `table` variable is the one we can then pass on to other functions
    // that can consume Apache Arrow memory structures. This object has ownership of
    // all referenced data, thus we don't have to care about undefined references once
    // we leave the scope of the function building the table and its underlying arrays.
    std::shared_ptr<arrow::Table> table =
        arrow::Table::Make(schema, {category_array, title_array, lang_array, author_array, editions_array});

    return table;
}
