/*#############################################################
 *#                                                           #
 *#    Author: C. Distefano                                   #
 *#                                                           #
 *#    Date: 31 May 2021                                      #
 *#    Class to write different format output files           #
 *#                                                           #
 *#############################################################
 */

#ifndef _OUTPUTWRITERNATIVE__
#define _OUTPUTWRITERNATIVE__

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "OutputWriter.hh"
#include "Parameters.hh"

/* -------------------------------------------------------------------------- */
class OutputWriterNative : public OutputWriter {
 public:
  OutputWriterNative();              ///< Constructor
  virtual ~OutputWriterNative() {};  ///< Destructor

  void WriteHeader();
  void WriteEvent(int i, unsigned int m, Vec3D posBundle, Vec3D uBundle,
                  double bundle_energy, double* xMuOnCAN, double* yMuOnCAN,
                  double* zMuOnCAN, double* E, double* t, int* MuOnCAN,
                  double evttime, double weight);
  void WriteLiveTime(double avlivetime, double eavlivetime);

 private:
  std::ofstream fOutStream;
};

#endif
