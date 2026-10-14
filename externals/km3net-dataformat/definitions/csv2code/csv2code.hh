#ifndef __csv2code__hh
#define __csv2code__hh

#include <iostream>
#include <string>
#include <vector>

struct Header {
  std::string base;
  std::string description;
  std::string url;
};

// the columns of the input CSV file
struct CSV {
  std::vector<std::string> types;
  std::vector<std::string> names;
  std::vector<std::string> numbers;
  std::vector<std::string> comments;
};

template <typename LANG>
void convertCSV(std::ostream& out, const CSV& csv);

template <typename LANG>
void convertHeader(std::ostream& out, const Header& header);

template <typename LANG>
void convert(std::ostream& out, const Header& header, const CSV& csv) {
  convertHeader<LANG>(out, header);
  convertCSV<LANG>(out, csv);
}

CSV readCSV(std::istream& in);

#endif
