/*#############################################################
 *#                                                           #
 *#    Author: V.Kulikovskiy                                  #
 *#    First Release: May 2026                                #
 *#                                                           #
 *#############################################################
 */

#ifndef RANDOM_h
#define RANDOM_h

#include <random>

class Random {
 private:
  std::mt19937 mt_rand;
  std::uniform_real_distribution<double> dis;

 public:
  Random(std::mt19937::result_type seed);
  virtual ~Random() {};  //!< virtual destructor

  //! random value [0,1)
  double Rndm();
  //! random Exp value
  double Exp(double tau);
};

/*! \class Muons
 *  \brief A class to generate random numbers
 *
 *  Substitutes TRandom
 *
 *  V.Kulikovskiy - First release: May 2026
 */

#endif /*RANDOM_h*/
