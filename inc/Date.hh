/*#############################################################
 *#                                                           #
 *#    Authors: G.Carminati, A.Sottoriva                      #
 *#    First Release: Jan 2006                                #
 *#                                                           #
 *#############################################################
 */

#ifndef Date_h
#define Date_h

#include <string>

class Date {
 private:
  int date_;  //!< date

 public:
  Date() {};           //!< constructor
  virtual ~Date() {};  //!< virtual destructor

  //! convert integer number in a string
  std::string character(int d);

  //! inizialize date and time and convert in a string
  std::string date_clock(std::string d_c);
};

/*! \class Date
 *  \brief A class to write processing date and time in the ANTARES format
 *
 *  G.Carminati, A.Sottoriva - First release: Jan 2006
 */

#endif /*Date_h*/
