#include <string>

typedef double ANTADouble;
typedef unsigned short int ANTAUsInt;
typedef unsigned long int ANTAUlInt;
typedef unsigned int ANTAUInt;
typedef std::string ANTAString;
typedef bool ANTABool;

ANTADouble Emax = 500.0;
ANTADouble DEPTHmax;
ANTADouble can_center_x = 0.;
ANTADouble can_center_y = 0.;
ANTADouble Zmin;
ANTADouble Zmax;
ANTADouble CANr;
ANTADouble EnlargedCANr = 100.;
ANTADouble CANh;
ANTADouble THETAmin = 0.;
ANTADouble THETAmax = 85.;
ANTADouble Rmin = 0.;
ANTADouble Rmax = 100.;
ANTAUsInt MULTmin = 1;
ANTAUsInt MULTmax = 100;
ANTAUsInt GEANTid = 5;
ANTADouble density = 1.03;
ANTADouble AbsLength = 55;
ANTADouble NAbsLength = 2.5;
ANTADouble Ethreshold = 0.001;

typedef double FLUX;
typedef double ALLmu;
typedef double SINGLEmu;
typedef double MULTImu;
typedef double LateralSpread;

/* Parameters from APP 25 (2006) 1-13 */
FLUX K0a = 0.0072;
FLUX K0b = -1.927;
FLUX K1a = -0.581;
FLUX K1b = 0.034;
FLUX ni0a = -0.0771;
FLUX ni0b = 0.524;
FLUX ni0c = 2.068;
FLUX ni1a = 0.03;
FLUX ni1b = 0.47;

ALLmu BETA = 0.42;

SINGLEmu g0 = -0.232;
SINGLEmu g1 = 3.961;
SINGLEmu e0a = 0.0304;
SINGLEmu e0b = 0.359;
SINGLEmu e1a = -0.0077;
SINGLEmu e1b = 0.659;

MULTImu A0 = 0.0033;
MULTImu A1 = 0.0079;
MULTImu B0a = 0.0407;
MULTImu B0b = 0.0283;
MULTImu B1a = -0.312;
MULTImu B1b = 6.124;
MULTImu Q0 = 0.0543;
MULTImu Q1 = -0.365;
MULTImu C0a = -0.069;
MULTImu C0b = 0.488;
MULTImu C1 = -0.117;
MULTImu D0a = -0.398;
MULTImu D0b = 3.955;
MULTImu D1a = 0.012;
MULTImu D1b = -0.35;

LateralSpread RHO0a = -1.786;
LateralSpread RHO0b = 28.26;
LateralSpread RHO1 = -1.06;
LateralSpread THETA0 = 1.3;
LateralSpread Fpiccolo = 10.4;
LateralSpread ALPHA0a = -0.448;
LateralSpread ALPHA0b = 4.969;
LateralSpread ALPHA1a = 0.0194;
LateralSpread ALPHA1b = 0.276;

ANTAUlInt seed;
ANTAUlInt events;
ANTAUInt numrun;
ANTAString OutputFileName;
#ifdef _KM3NET_ENABLED__
ANTAString OutputFormat = "km3net";
#elif _ANTARES_ENABLED__
ANTAString OutputFormat = "antares";
#else
ANTAString OutputFormat = "native";
#endif
ANTAString LivetimeFileName = "";
ANTAString ParameterFileName;
ANTAString DetectorFileName;
ANTAString RootOutputFile;
ANTAUlInt TimeStampStart = 0;
ANTAUlInt TimeStampStop = 0;
ANTABool GenTime = false;
ANTABool TimeOrder = true;
ANTABool histo;
ANTABool lib_det;
ANTABool doweighted = false;
