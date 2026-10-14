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

#include "OutputWriterNative.hh"

OutputWriterNative::OutputWriterNative() {
  fOutStream.open(OutputFileName.c_str(), std::ios::out);
}

void OutputWriterNative::WriteHeader() {
  fOutStream.setf(std::ios::showpoint, std::ios::fixed);
  fOutStream << "Run Number: " << numrun << std::endl;
  fOutStream << taglivetime << ": ";
  fOutStream << "                                           " << std::endl;
}

///////////////////////////////////////////////////////////////////////////
void OutputWriterNative::WriteEvent(int i, unsigned int m, Vec3D posBundle,
                                    Vec3D uBundle, double bundle_energy,
                                    double* xMuOnCAN, double* yMuOnCAN,
                                    double* zMuOnCAN, double* E,
                                    double* delaytime, int* MuOnCAN,
                                    double evttime, double weight) {
  for (unsigned int ll = 0; ll < m; ++ll) {
    if (MuOnCAN[ll] == 0)  // writes muons if they have E>0 and are on the can
    {
      fOutStream.setf(std::ios::showpoint, std::ios::fixed);
      fOutStream << std::setprecision(3) << std::scientific << std::setw(15)
                 << i + 1 << std::setw(5) << ll + 1 << std::setw(12)
                 << xMuOnCAN[ll] + can_center_x << std::setw(12)
                 << yMuOnCAN[ll] + can_center_y << std::setw(12) << zMuOnCAN[ll]
                 << std::setw(12) << uBundle.x << std::setw(12) << uBundle.y
                 << std::setw(12) << uBundle.z << std::setw(12)
                 << E[ll] * TeVtoGeV() << std::setw(12) << evttime
                 << std::setw(12) << delaytime[ll] << std::setw(12) << weight
                 << std::endl;
    }
  }
}

///////////////////////////////////////////////////////////////////////////

void OutputWriterNative::WriteLiveTime(double avlivetime, double eavlivetime) {
  std::string line;
  std::ifstream file(OutputFileName.c_str());
  unsigned int position = 0;
  while (getline(file, line)) {
    if (line.find(taglivetime) != std::string::npos) break;
    position = file.tellg();
  }
  file.close();
  position += taglivetime.length() + 2;
  fOutStream.setf(std::ios::fixed, std::ios::floatfield);
  fOutStream.seekp(position) << std::setprecision(3) << avlivetime << "    "
                             << std::setprecision(1) << eavlivetime;
}
