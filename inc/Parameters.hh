// #############################################################
// #                                                           #
// #    Author: G.Carminati                                    #
// #    First Release: Feb 2006                                #
// #                                                           #
// #############################################################
//

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <cmath>  // M_PI
#include <string>

const double c_light_ns = 2.9979246e+08 * 1.e-9; /* in m/ns */
inline double Pi() { return M_PI; }

//
//! Length [L]
//
inline double MeterToKm() { return 1.e-3; }
inline double KmToMeter() { return 1.e+3; }
inline double Km2ToMeter2() { return KmToMeter() * KmToMeter(); }

//
//! Angle
//
inline double RadToDeg() { return 180. / Pi(); }
inline double DegToRad() { return Pi() / 180.; }

//
//! Time [T]
//
inline double sTOdays() { return 1 / 86400.; }

//
//! Energy [E]
//
inline double GeVtoTeV() { return 1.e-3; }
inline double TeVtoGeV() { return 1.e+3; }

typedef std::string MUPAGE;

const MUPAGE versn = GIT_VERSION;
const MUPAGE dater = BUILD_DATE;

typedef double ENERGY;

const ENERGY Emin = 0.001f;

typedef double ANTADouble;
typedef unsigned short int ANTAUsInt;
typedef unsigned long int ANTAUlInt;
typedef unsigned int ANTAUInt;

typedef std::string ANTAString;
typedef bool ANTABool;

typedef double FLUX;
typedef double ALLmu;
typedef double SINGLEmu;
typedef double MULTImu;
typedef double LateralSpread;

extern ANTADouble Emax;
extern ANTADouble DEPTHmax;
extern ANTADouble can_center_x;
extern ANTADouble can_center_y;
extern ANTADouble Zmax;
extern ANTADouble Zmin;
extern ANTADouble Zmax;
extern ANTADouble CANr;
extern ANTADouble EnlargedCANr;
extern ANTADouble CANh;
extern ANTADouble THETAmin;
extern ANTADouble THETAmax;
extern ANTADouble Rmin;
extern ANTADouble Rmax;
extern ANTAUsInt MULTmin;
extern ANTAUsInt MULTmax;
extern ANTAUsInt GEANTid;
extern ANTADouble density;
extern ANTADouble Ethreshold;

extern ANTAUlInt seed;
extern ANTAUlInt events;
extern ANTAUInt numrun;
extern ANTAString OutputFileName;
extern ANTAString OutputFormat;
extern ANTAString LivetimeFileName;
extern ANTAString ParameterFileName;
extern ANTAString DetectorFileName;
extern ANTAString RootOutputFile;
extern ANTAUlInt TimeStampStart;
extern ANTAUlInt TimeStampStop;
extern ANTABool GenTime;
extern ANTABool TimeOrder;
extern ANTABool histo;
extern ANTABool lib_det;
extern ANTABool doweighted;

// WARNING: Do not change the units!

/* to convert km in km.w.e. (1 km.w.e. = 1.e+6 kg m**(-3)) */
inline double kmTOkmwe() { return 1. * density; }

inline double CANheight() { return Zmax - Zmin; }        /* in m  */
inline double centerZ() { return (Zmax + Zmin) * 0.5; }  /* in m  */
inline double CANradius() { return CANr * MeterToKm(); } /* in km */
inline double DEPTHmin() { return DEPTHmax - CANheight() * MeterToKm(); }
/* in km */
inline double Qmin() { return THETAmin * DegToRad(); }         /* in rad */
inline double Qmax() { return THETAmax * DegToRad(); }         /* in rad */
inline double weDEPTHmin() { return DEPTHmin() * kmTOkmwe(); } /* in km.w.e. */
inline double weDEPTHmax() { return DEPTHmax * kmTOkmwe(); }   /* in km.w.e. */
inline double max(const double& a, const double& b) { return a > b ? a : b; }
inline double min(const double& a, const double& b) { return a < b ? a : b; }

/* Parameters from APP 25 (2006) 1-13 */
extern FLUX K0a;
extern FLUX K0b;
extern FLUX K1a;
extern FLUX K1b;
extern FLUX ni0a;
extern FLUX ni0b;
extern FLUX ni0c;
extern FLUX ni1a;
extern FLUX ni1b;

extern ALLmu BETA;

extern SINGLEmu g0;
extern SINGLEmu g1;
extern SINGLEmu e0a;
extern SINGLEmu e0b;
extern SINGLEmu e1a;
extern SINGLEmu e1b;

extern MULTImu A0;
extern MULTImu A1;
extern MULTImu B0a;
extern MULTImu B0b;
extern MULTImu B1a;
extern MULTImu B1b;
extern MULTImu Q0;
extern MULTImu Q1;
extern MULTImu C0a;
extern MULTImu C0b;
extern MULTImu C1;
extern MULTImu D0a;
extern MULTImu D0b;
extern MULTImu D1a;
extern MULTImu D1b;

extern LateralSpread RHO0a;
extern LateralSpread RHO0b;
extern LateralSpread RHO1;
extern LateralSpread THETA0;
extern LateralSpread Fpiccolo;
extern LateralSpread ALPHA0a;
extern LateralSpread ALPHA0b;
extern LateralSpread ALPHA1a;
extern LateralSpread ALPHA1b;

// Parameters for muon energy losses in TeV, m
const double DENSITY_SEA_WATER = 1.03;  // in g/cm^3
const double eps0 = 1e-10;
const double a0 = 2.30e-4 * DENSITY_SEA_WATER;   // 1e-3 GeV/cm = 1e-4 TeV/m
const double b0 = 15.50e-4 * DENSITY_SEA_WATER;  // 1e-6 /cm = 1e-4 /m

const double en1 = 0.03;  // in TeV
const double a1 = 2.67e-4 * DENSITY_SEA_WATER;
const double b1 = 3.4e-4 * DENSITY_SEA_WATER;

const double en2 = 35.3;  // in TeV
const double a2 = -6.50e-4 * DENSITY_SEA_WATER;
const double b2 = 3.66e-4 * DENSITY_SEA_WATER;

/*! \class Parameters
 *  \brief A class to define all parameters used in MUPAGE
 *
 *  G.Carminati - First release: Feb 2006
 */

#endif /*PARAMETERS_H*/
