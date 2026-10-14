#include "csv2julia.hh"

template <>
void convertHeader<JULIA>(std::ostream& out, const Header& header) {
  out << "\"\"\"\n";
  out << "# " << header.description << "\n";
  out << header.url << "\n";
  out << "\"\"\"\n";
  out << "\n";
  std::string BASE = header.base;
  for (auto& c : BASE) {
    c = toupper(c);
  }
  out << "module " << BASE << "\n";
}

template <>
void convertCSV<JULIA>(std::ostream& out, const CSV& csv) {
  for (auto i = 0; i < csv.types.size(); i++) {
    out << "  const " << csv.names[i] << " = " << csv.numbers[i] << "\n";
  }
  out << "end\n";
}
