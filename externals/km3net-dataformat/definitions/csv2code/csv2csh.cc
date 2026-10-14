#include "csv2csh.hh"

template <>
void convertHeader<CSH>(std::ostream& out, const Header& header) {
  out << "#!/usr/bin/env csh\n";
  out << "# " << header.description << "\n";
  out << "# " << header.url << "\n";
  out << "\n";
}

template <>
void convertCSV<CSH>(std::ostream& out, const CSV& csv) {
  for (auto i = 0; i < csv.types.size(); i++) {
    out << "setenv " << csv.names[i] << " " << csv.numbers[i] << "\n";
  }
  out << "\n";
}
