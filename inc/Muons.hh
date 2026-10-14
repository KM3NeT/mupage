/*#############################################################
 *#                                                           #
 *#    Author: G.Carminati                                    #
 *#    First Release: Dec 2006                                #
 *#                                                           #
 *#############################################################
 */

#ifndef MUONS_h
#define MUONS_h

#include "Parameters.hh"
#include "Random.hh"
#include "Vector.hh"

class Muons {
 private:
  double zenith_;              //!< zenith angle  [rad]
  double azimuth_;             //!< azimuth angle [rad]
  unsigned int multiplicity_;  //!< muon multiplicity in a bundle
  double depth_;               //!< depth wrt sea level [km.w.e.]
  Random& trand_;              // random generator

  double depthmax_;   //!< maximum depth wrt sea level [km.w.e.]
  double canzmin_;    //!< z position of the CAN lower disk [m]
  double canzmax_;    //!< z position of the CAN upper disk [m]
  double canradius_;  //!< radius of the CAN [m]
  double lsmin_;      //!< minimum lateral spread [m]
  double lsmax_;      //!< maximum lateral spread [m]
  double energymin_;  //!< minimum energy [TeV]
  double energymax_;  //!< maximum energy [TeV]
  double logEmin;
  double logEmax;
  double dNdE_singlemu_min;
  double dNdE_multimu_min;

 public:
  Muons(double zenith, double azimuth, unsigned int multiplicity, double depth,
        Random& trand);  //!< constructor
  virtual ~Muons() {};   //!< virtual destructor

  //! set class parameter
  void Set(const double& zenith, const double& azimuth,
           const unsigned int& multiplicity, const double& depth);

  //! set private parameters from input file
  void SetParameters(const double& depthmax, const double& canzmin,
                     const double& canzmax, const double& canradius,
                     const double& lsmin, const double& lsmax,
                     const double& energymin, const double& energymax);

  //! compute the muon flux
  double Flux();

  //! compute the projected area of the CAN
  double ProjectedArea();

  //! compute the solid angle
  double SolidAngle(const double& alpha1, const double& alpha2);

  //! compute the product of flux, projected area and solid angle
  double GAMMA(const double& alpha1, const double& alpha2);

  //! generate a random impact point of the shower axis on the CAN (but not on
  //! the lower disk)
  Vec3D axisBundleOnCAN();

  //! initialize the vertical depth of muon wrt sea level [km.w.e.]
  double Depth(const double& zBundle);

  //! compute the vertical depth for energy distribution
  double BetaChi();

  //! compute the gamma parameter for energy distribution of single muons
  double Gamma_singlemu();

  //! compute the epsilon parameter for energy distribution of single muons
  double Epsilon_singlemu();

  //! compute the maximum energy value of a single muon
  double E_singlemu();

  //! compute the energy distribution of a single muon
  double dNdE_singlemu(const double& E_mu);

  double N_singlemu(const double& E_mu_min);

  //! compute the energy of a single muon (according with the energy
  //! distribution)
  double HoR_Energy_singlemu(const double& ethreshold);

  //! compute the average radius value for radial distribution
  double AverageRadius();

  //! compute che alpha parameter for radial distribution
  double Alpha();

  //! compute the maximum radial distribution value of a muon in a bundle
  double Rpeak();

  //! compute the radial distribution of a muon in a bundle
  double dNdR(const double& r);

  //! generate the lateral spread (according with the radial distribution) of a
  //! muon in bundle
  double HoR_Radius();

  //! compute the gamma parameter for energy distribution of multiple muons
  double Gamma_multimu(const double& r);

  //! compute the epsilon parameter for energy distribution of multiple muons
  double Epsilon_multimu(const double& r);

  //! compute the maximum energy value of a multiple muon
  double E_multimu(const double& r);

  //! compute the energy distribution of a multiple muon
  double dNdE_multimu(const double& r, const double& E_mu);

  double N_multimu(const double& r, const double& E_mu_min);

  //! compute the energy of a multiple muon in the bundle (according with the
  //! energy distribution)
  double HoR_Energy_multimu(const double& r, const double& ethreshold);

  //! compute the position of each muon in the bundle on the plane perpendicular
  //! at the shower axis
  Vec3D multimuOnLAB(Vec3D posBundleOnCAN, const double& r);

  //! compute the position of each muon in the bundle on the CAN (not on the
  //! lower disk)
  int muOnCAN(Vec3D posMuBefore, Vec3D uBundle, Vec3D& posMuAfter);

  //! Calculate the residual energy of a muon (GeV) after a path (m).
  //! Paremeters are taken from S. Klimushin E. Bugaev and I. Sokalski
  //! "Precise parameterization of muon energy losses in water" ICRC2001
  //! The calculation works for Dx>0 and Dx<0.
  double calcEloss(double E, double Dx);
};

/*! \class Muons
 *  \brief A class to create the events
 *
 *  In this class the muon flux, the radial distribution, the energy
 *  spectrum and the CAN impact point of the events are computed.
 *
 *  G.Carminati - First release: Dec 2006
 */

#endif /*MUONS_h*/
