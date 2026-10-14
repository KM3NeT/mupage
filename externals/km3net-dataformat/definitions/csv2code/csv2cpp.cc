#include "csv2cpp.hh"

#include <iomanip>

template <>
void convertHeader<CPP>(std::ostream& out, const Header& header) {
  out << "#ifndef __" << header.base << "__\n";
  out << "#define __" << header.base << "__\n";
  out << "\n";
  out << "/**\n";
  out << " * \file \n";
  out << " * " << header.description << "\n";
  out << " * " << header.url << "\n";
  out << " */\n";
  out << "\n";
  out << "/**\n";
  out << " * " << header.base << "\n";
  out << " */\n";
}

template <>
void convertCSV<CPP>(std::ostream& out, const CSV& csv) {
  for (auto i = 0; i < csv.types.size(); i++) {
    auto number = csv.numbers[i] + ";";
    out << "static const " << csv.types[i] << " " << std::left << std::setw(32)
        << csv.names[i] << " = " << std::left << std::setw(10) << number
        << " //!< " << csv.comments[i] << "\n";
  }
  out << "#endif\n";
}
