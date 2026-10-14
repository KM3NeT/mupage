/*#############################################################
 *#                                                           #
 *#    Author: C. Distefano                                   #
 *#    Conributors: V. Kulikovskiy                            #
 *#                                                           #
 *#    Date: 31 May 2021                                      #
 *#    Class to write different format output files           #
 *#                                                           #
 *#############################################################
 */

#include "OutputWriter.hh"

OutputWriter::OutputWriter() {
  std::cout << std::endl
            << "Creating output file " << OutputFileName << " with format "
            << OutputFormat << std::endl;
}
