#include "csv2python.hh"

template <>
void convertHeader<PYTHON>(std::ostream& out, const Header& header) {
  out << "# -*- coding: utf-8 -*-\n";
  out << "\"\"\"\n";

  out << header.description << "\n";
  out << header.url << "\n";
  out << "\"\"\"\n";
  out << "\n";
  out << "# " << header.base << "\n";
}

template <>
void convertCSV<PYTHON>(std::ostream& out, const CSV& csv) {
  out << "data = dict(\n";
  for (auto i = 0; i < csv.types.size(); i++) {
    out << "    " << csv.names[i] << "=" << csv.numbers[i] << ",\n";
  }
  out << ")\n";
}
