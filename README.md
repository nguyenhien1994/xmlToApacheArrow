# xmlToApacheArrow

## XML To Apache arrow::Table example

Example of converting XML to Apache arrow::Table

This section assumes you already have the Arrow C++ libraries on your system, either after [installing them using a package manager](https://arrow.apache.org/install/) or after [building them yourself](https://arrow.apache.org/docs/developers/cpp/building.html#building-arrow-cpp).

### How to build and run this example
- Clone code
```
git clone git@github.com:nguyenhien1994/xmlToApacheArrow.git
```

- If you want to build with local user library paths, you can modify the `.env` to approriate paths and run `source .env`
- Build:
```
./build.sh
```

- Run:
```
./build/arrowTableExample bookstore.xml
```


### Input
```
<?xml version="1.0" encoding="UTF-8"?>
<bookstore>
  <book category="cooking">
    <title lang="en">Everyday Italian</title>
    <author>Giada De Laurentiis</author>
    <editions>
      <edition>
        <isbn>a12d27ae-c539-11ec-9d64-0242ac120002</isbn>
        <year>2005</year>
        <price>30.00</price>
      </edition>
      <edition>
        <isbn>b8b1824e-c539-11ec-9d64-0242ac120002</isbn>
        <year>2018</year>
        <price>29.00</price>
      </edition>
    </editions>
  </book>
  <book category="children">
    <title lang="en">Harry Potter</title>
    <author>J K. Rowling</author>
    <editions>
      <edition>
        <year>2005</year>
        <price>29.99</price>
      </edition>
    </editions>
  </book>
  <book category="web">
    <title lang="en">Learning XML</title>
    <author>Erik T. Ray</author>
    <editions>
      <edition>
        <year>2003</year>
        <price>39.95</price>
      </edition>
    </editions>
  </book>
</bookstore>
```

### Output: an arrow::Table object

### References:
* arrow::Table: https://arrow.apache.org/docs/cpp/api/table.html#_CPPv4N5arrow5TableE
* Dependencies: https://github.com/apache/arrow/tree/master/cpp
* Input XML (with modification): https://www.w3schools.com/xml/xml_tree.asp

### Notes for improvements:
- Load [Dynamic XML Schemas](https://arrow.apache.org/docs/cpp/examples/row_columnar_conversion.html#dynamic-schemas)
- Write a wrapper for TinyXML2 with templates
- Deep dive to [Arrow document](https://arrow.apache.org/docs/index.html)
