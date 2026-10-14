/*#############################################################
 *#                                                           #
 *#    Author: C. Distefano                                   #
 *#                                                           #
 *#    Date: 31 May 2021                                      #
 *#    Class to write different format output files           #
 *#                                                           #
 *#############################################################
 */

#ifndef _OUTPUTWRITER__
#define _OUTPUTWRITER__

#include <fstream>

#include "Parameters.hh"
#include "Vector.hh"

/* -------------------------------------------------------------------------- */
class OutputWriter {
 protected:
  const std::string taglivetime = "livetime";

 public:
  OutputWriter();
  virtual ~OutputWriter() {};  //!< virtual destructor

  virtual void WriteHeader() {};
  virtual void WriteEvent(int i, unsigned int m, Vec3D posBundle, Vec3D uBundle,
                          double bundle_energy, double* xMuOnCAN,
                          double* yMuOnCAN, double* zMuOnCAN, double* E,
                          double* t, int* MuOnCAN, double evttime,
                          double weight) {};
  virtual void WriteLiveTime(double avlivetime, double eavlivetime) {};
};

#endif
