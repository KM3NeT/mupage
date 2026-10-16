/*
 *MM        MM UU        UU PPPPPPPPPPP   AAAAAAAAAA   GGGGGGGGGG  EEEEEEEEEEEE
 *MMM      MMM UU        UU PPPPPPPPPPPP AAAAAAAAAAAA GGGGGGGGGGGG EEEEEEEEEEEE
 *MMMM    MMMM UU        UU PP        PP AA        AA GG        GG EE
 *MM MM  MM MM UU        UU PP        PP AA        AA GG           EE
 *MM  MMMM  MM UU        UU PP        PP AA        AA GG           EE
 *MM   MM   MM UU        UU PPPPPPPPPPPP AAAAAAAAAAAA GG           EEEEEEEE
 *MM        MM UU        UU PPPPPPPPPPP  AAAAAAAAAAAA GG     GGGGG EEEEEEEE
 *MM        MM UU        UU PP           AA        AA GG     GGGGG EE
 *MM        MM UU        UU PP           AA        AA GG        GG EE
 *MM        MM UU        UU PP           AA        AA GG        GG EE
 *MM        MM UUUUUUUUUUUU PP           AA        AA GGGGGGGGGGGG EEEEEEEEEEEE
 *MM        MM  UUUUUUUUUU  PP           AA        AA  GGGGGGGGGG  EEEEEEEEEEEE
 *
 *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * Author:       G.Carminati, A. Margiotta, M. Spurio
 * Contributors: J. Brunner, C. Distefano, V. Kulikovskiy, Th.Stolarczyk
 *
 */

#include <climits>  //for INT_MAX
#include <cmath>
#include <cstring>  //for memset
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "EventTime.hh"
#include "Muons.hh"
#include "OutputWriter.hh"
#include "OutputWriterNative.hh"
#ifdef _KM3NET_ENABLED__
#include "OutputWriterKM3NeT.hh"
#endif
#ifdef _ANTARES_ENABLED__
#include "OutputWriterANTARES.hh"
#endif
#include "CLI11.hpp"
#include "Parameters.hh"

#ifdef _ANTARES_ENABLED__
#include "io_gcc.hh"
#endif

namespace {
class MupageHelpFormatter : public CLI::Formatter {
 public:
  std::string make_help(const CLI::App *app, std::string name,
                        CLI::AppFormatMode mode) const override {
    if (mode == CLI::AppFormatMode::Sub) return make_expanded(app, mode);

    std::stringstream out;
    if ((app->get_name().empty()) && (app->get_parent() != nullptr)) {
      if (app->get_group() != "SUBCOMMANDS") out << app->get_group() << ':';
    }
    if (is_description_paragraph_formatting_enabled()) {
      CLI::detail::streamOutAsParagraph(out, make_description(app),
                                        description_paragraph_width_, "");
    } else {
      out << make_description(app) << '\n';
    }
    out << make_usage(app, name);
    out << make_positionals(app);
    out << make_groups_filtered(app, mode);
    out << make_subcommands(app, mode);
    std::string footer_string = make_footer(app);

    if (is_footer_paragraph_formatting_enabled()) {
      CLI::detail::streamOutAsParagraph(out, footer_string,
                                        footer_paragraph_width_);
    } else {
      out << footer_string;
    }

    return out.str();
  }

 private:
  std::string make_groups_filtered(const CLI::App *app,
                                   CLI::AppFormatMode mode) const {
    std::stringstream out;
    std::vector<std::string> groups = app->get_groups();

    for (const std::string &group : groups) {
      if (mode != CLI::AppFormatMode::All &&
          group == "Flux parametrisation parameters")
        continue;

      std::vector<const CLI::Option *> opts =
          app->get_options([app, mode, &group](const CLI::Option *opt) {
            return opt->get_group() == group && opt->nonpositional() &&
                   (mode != CLI::AppFormatMode::Sub ||
                    (app->get_help_ptr() != opt &&
                     app->get_help_all_ptr() != opt));
          });
      if (!group.empty() && !opts.empty())
        out << make_group(group, false, opts);
    }

    return out.str();
  }
};
}  // namespace

void ParseArgs(int argc, char **argv);
void PrintHeader(void);
void AddLivetimeThetaRange(unsigned int thetamin, unsigned int thetamax,
                           std::vector<unsigned int> ncounter,
                           const Vec3D &enlargedCAN, Random &genrandom,
                           std::ofstream &livetime_fout, double &ilivetime,
                           double &iweight);
void CalculateLivetime(const unsigned int &thetarange,
                       std::vector<unsigned int> ncounter,
                       const Vec3D &enlargedCAN, Random &genrandom,
                       std::ofstream &livetime_fout, double &avlivetime,
                       double &eavlivetime);

int main(int argc, char **argv) {
  /* print code header on screen */
  PrintHeader();

  /* options parsing */
  try {
    ParseArgs(argc, argv);
  } catch (const CLI::CallForAllHelp &e) {
    return 0;
  } catch (const CLI::CallForHelp &e) {
    return 0;
  } catch (const CLI::ParseError &e) {
    return 1;
  }

  /* seeds definition */
  if (seed == 0) seed = time(NULL);
  Random genrandom(seed);

  /* create parameters for the CAN */
  Vec3D can(Zmin, Zmax, CANr);
  CANh = CANheight();

  /* the CAN have to be enlarged to avoid losing events */
  Vec3D enlargedCAN(can.x, can.y, can.z + EnlargedCANr);

  /* check on vertical depth values */
  if (weDEPTHmin() < 1.5 || weDEPTHmin() > 5.) {
    std::cout << "The chosen value of minimum vertical depth \n"
              << "(i.e., the vertical depth of CAN upper disk"
              << "with respect to the sea level) \n"
              << "is " << DEPTHmin() << " km,"
              << " equivalent to " << weDEPTHmin() << " km.w.e.\n"
              << "The parametric formulas are valid only in the range"
              << " of 1.5 to 5 km.w.e.\n"
              << "Maybe the vertical depth of the seabed DEPTHmax \n"
              << "is not enough deep.\n";

    return 1;
  }
  if (weDEPTHmax() < 1.5 || weDEPTHmax() > 5.) {
    std::cout << "The chosen value of maximum vertical depth \n"
              << "(seabed = DEPTHmax) is " << DEPTHmax << " km,"
              << " equivalent to " << weDEPTHmax() << " km.w.e.\n"
              << "The parametric formulas are valid only in the range"
              << " of 1.5 to 5 km.w.e.\n";

    return 1;
  }

  /* opening output files */
  std::ofstream livetime_fout;
  if (LivetimeFileName != "")
    livetime_fout.open(LivetimeFileName.c_str(), std::ios::out);
  OutputWriter *OutWrt = NULL;
  if (OutputFormat == "native") {
    OutWrt = new OutputWriterNative();
  }
#ifdef _KM3NET_ENABLED__
  else if (OutputFormat == "km3net") {
    OutWrt = new OutputWriterKM3NeT();
  }
#endif
#ifdef _ANTARES_ENABLED__
  else if (OutputFormat == "antares") {
    OutWrt = new OutputWriterANTARES();
  }
#endif
  else {
    std::cerr << OutputFormat << " is not avaialbe" << std::endl;
  }

  OutWrt->WriteHeader();
  /* Implementation class Muons */
  Muons muon(Qmin(), 0., MULTmin, weDEPTHmin(), genrandom);
  muon.SetParameters(DEPTHmax, enlargedCAN.x, enlargedCAN.y, enlargedCAN.z,
                     Rmin, Rmax, Emin, Emax);

  /* initialization of the counter to compute the generated events
     on the enlarged can */
  unsigned int counter = 0;

  /* initialization of counters to compute livetime */
  const unsigned int thetarange = 86;
  std::vector<unsigned int> ncounter(thetarange, 0);

  /* compute the area of the enlarged CAN (in m**2) */
  double Sz = Pi() * enlargedCAN.z * enlargedCAN.z;  //<------ upper disk
  double Sxy = 2 * enlargedCAN.z * CANh;             //<------ barrel surface

  /* compute the maximum projected area*deltaOmega using the fact that
     d(Area*deltaOmega)/d(theta) = 0 (in m**2) */
  double ALFA = atan(-Pi() * enlargedCAN.z / (2 * CANh)) * .5 + Pi() / 2.;

  /* compute the maximum number of events per unit of time */
  double halfdegree = 0.5 * DegToRad();
  double Smax = Sz * cos(ALFA) + Sxy * sin(ALFA);
  Smax = max(Sz, Smax);
  double Nmax = muon.Flux() * Smax *
                muon.SolidAngle(ALFA - halfdegree, ALFA + halfdegree);

  /* counter for events with energy lower than threshold one */
  EventTime et(events, GenTime, TimeOrder, TimeStampStart, TimeStampStop,
               &genrandom);
  double Nlow = 0;
  bool generationOK;
  double evttime;
  unsigned long i = 0;
  while (i < events) {
    generationOK = true;
    muon.SetParameters(DEPTHmax, enlargedCAN.x, enlargedCAN.y, enlargedCAN.z,
                       Rmin, Rmax, Emin, Emax);

    unsigned int m = 0;
    double phi = 0.;
    /* thetaSG means the zenith angle in the System of Generation */
    double thetaSG = 0.;
    double ctSG = 0.;
    double stSG = 0.;
    /* thetaDF means the zenith angle in the Direction-to-fly System */
    double thetaDF = 0.;
    double ctDF = 0.;
    double stDF = 0.;
    double u = 0.;
    double wedepthBundle = 0.;
    Vec3D posBundle(0., 0., 0.);

    phi = genrandom.Rndm() * 2. * Pi();

    bool loop = true;
    while (loop) {
      /* generate random the muon multiplicity */
      m = (int)(genrandom.Rndm() * (MULTmax + 1 - MULTmin)) + MULTmin;

      /* generate random the zenith (theta) angle [in radians] */
      thetaSG = genrandom.Rndm() * (Qmax() - Qmin()) + Qmin();
      ctSG = cos(thetaSG);
      stSG = sin(thetaSG);

      /* implement new parameters in the class */
      muon.Set(thetaSG, phi, m, wedepthBundle);

      /* compute the shower axis position
         on the can surface (in m) */
      posBundle = muon.axisBundleOnCAN();

      /* convert the z-position of the impact point
         in the vertical depth with respect to the sea level
         (in km.w.e.) */
      wedepthBundle = muon.Depth(posBundle.z);

      /* compute the projected area of the enlarged can
         for the generated zenith angle (in m**2) */
      double S = Sz * ctSG + Sxy * stSG;

      /* compute the number of events per unit of time
         for generated zenith angle, multiplicity and vertical depth */
      double Nproj =
          muon.Flux() * S *
          muon.SolidAngle(thetaSG + halfdegree, thetaSG - halfdegree);

      /* accept or reject event */
      u = genrandom.Rndm() * Nmax;
      if (u <= Nproj)  //<------- accept event
        loop = false;
    }

    /* compute the generated single events N*
       on upper disk of the enlarged CAN at different zenith angles */
    float fhBundle = wedepthBundle;
    float fHmin = weDEPTHmin();
    if ((fabs(fhBundle - fHmin) < 1e-6) && (m == MULTmin)) {
      ++counter;
      for (unsigned int j = 0; j < thetarange; ++j) {
        if (thetaSG * RadToDeg() > j && thetaSG * RadToDeg() <= j + 1)
          ++(ncounter[j]);
      }
    }

    /* WARNING: According with the switching of the azimuth angle,
     *          the zenith angle must be switched into
     *          theta_mu = 180 degrees - zenith
     */
    thetaDF = Pi() - thetaSG; /* in rad */
    ctDF = cos(thetaDF);
    stDF = sin(thetaDF);
    muon.Set(thetaSG, phi, m, wedepthBundle);

    /* unit vector (i.e. direction cosines) of Bundle Axis */
    double cf = cos(phi);
    double sf = sin(phi);
    Vec3D uBundle(stDF * cf, stDF * sf, ctDF);

    if (m == 1) /* single muons */
    {
      double t = 0.;
      int MuOnCAN = 0;

      /* compute the energy of single muon */
      double weight = muon.N_singlemu(Ethreshold);
      double e;

      e = muon.HoR_Energy_singlemu(Ethreshold); /* in TeV */

      if (doweighted) {
        evttime = et.GetTime();
        OutWrt->WriteEvent(i, m, posBundle, uBundle, e, &posBundle.x,
                           &posBundle.y, &posBundle.z, &e, &t, &MuOnCAN,
                           evttime, weight);
      } else if (genrandom.Rndm() < weight) {
        evttime = et.GetTime();
        OutWrt->WriteEvent(i, m, posBundle, uBundle, e, &posBundle.x,
                           &posBundle.y, &posBundle.z, &e, &t, &MuOnCAN,
                           evttime, 1);
      } else /* events under the energy of threshold */ {
        ++Nlow;
        generationOK = false;
        continue;
      }

    }

    else /* multiple muons */

    {
      double bundle_energy = 0.;
      double bundle_energy_at_can = 0.;
      std::vector<double> xMuOnLAB(m, 0.);
      std::vector<double> yMuOnLAB(m, 0.);
      std::vector<double> zMuOnLAB(m, 0.);
      std::vector<double> xMuOnCAN(m, 0.);
      std::vector<double> yMuOnCAN(m, 0.);
      std::vector<double> zMuOnCAN(m, 0.);
      std::vector<double> distance(m, 0.);
      std::vector<double> delaytime(m, 0.);
      std::vector<double> E(m, 0.);
      std::vector<double> R(m, 0.);
      std::vector<int> MuOnCAN(m, 0);

      double weight = 1;
      unsigned int lastOK = 0;  // last muon that crossed the can

      for (unsigned int l = 0; l < m; ++l) {
        /* compute the lateral spread of each muon in bundle */
        R[l] = muon.HoR_Radius();

        /* compute the position of each muon in bundle on the laboratory
           frame (in m) */
        Vec3D posMuOnLAB = muon.multimuOnLAB(posBundle, R[l]);
        xMuOnLAB[l] = posMuOnLAB.x;
        yMuOnLAB[l] = posMuOnLAB.y;
        zMuOnLAB[l] = posMuOnLAB.z;

        /* compute if the i-th muon in bundle impacts point also
           on the can surface */
        Vec3D posMuOnCAN(0., 0., 0.);
        int iok = muon.muOnCAN(posMuOnLAB, uBundle, posMuOnCAN);
        MuOnCAN[l] = iok;
        lastOK = l;
        if (iok == 0) /* muon intercepts the can */
        {
          xMuOnCAN[l] = posMuOnCAN.x;
          yMuOnCAN[l] = posMuOnCAN.y;
          zMuOnCAN[l] = posMuOnCAN.z;

          /* compute the delay time (in ns) */
          distance[l] =
              sqrt((xMuOnCAN[l] - xMuOnLAB[l]) * (xMuOnCAN[l] - xMuOnLAB[l]) +
                   (yMuOnCAN[l] - yMuOnLAB[l]) * (yMuOnCAN[l] - yMuOnLAB[l]) +
                   (zMuOnCAN[l] - zMuOnLAB[l]) * (zMuOnCAN[l] - zMuOnLAB[l]));
          if (zMuOnLAB[l] < zMuOnCAN[l]) distance[l] = -distance[l];

          delaytime[l] = distance[l] / c_light_ns; /* in ns */
        }
      }
      for (unsigned int l = 0; l < m; ++l) {
        if (MuOnCAN[l] == 0) { /* muon intercepts the can */
                               /* compute the energy of multiple muons */
          // we adjust at max only the last muon (if it crosses the can and if
          // we still need to reach the energy)
          if ((l == lastOK) && (Ethreshold - bundle_energy_at_can > 0)) {
            /* this it to fix limited weigth calculation for non weighted
             * production */
            weight = muon.N_multimu(R[l], Ethreshold - bundle_energy_at_can) /
                     muon.N_multimu(R[l], Emin);
            E[l] = muon.HoR_Energy_multimu(
                R[l], Ethreshold - bundle_energy_at_can); /* in TeV */
          } else {
            E[l] = muon.HoR_Energy_multimu(R[l], Emin);
          }
          bundle_energy += E[l];

          E[l] = muon.calcEloss(E[l], distance[l]);
          if (E[l] > Emin)
            bundle_energy_at_can += E[l];
          else
            MuOnCAN[l] = -99;  // we consider that muon did not crossed the can

        } else {
          /* muon bundle energy should contain total energy including muons not
           * intercecting the can */
          E[l] = muon.HoR_Energy_multimu(R[l], Emin);
          bundle_energy += E[l];
        }
      }

      if (bundle_energy_at_can >= Ethreshold) {
        if (doweighted) {
          evttime = et.GetTime();
          OutWrt->WriteEvent(i, m, posBundle, uBundle, bundle_energy,
                             xMuOnCAN.data(), yMuOnCAN.data(), zMuOnCAN.data(),
                             E.data(), delaytime.data(), MuOnCAN.data(),
                             evttime, weight);
        } else if (genrandom.Rndm() < weight) {
          evttime = et.GetTime();
          OutWrt->WriteEvent(i, m, posBundle, uBundle, bundle_energy,
                             xMuOnCAN.data(), yMuOnCAN.data(), zMuOnCAN.data(),
                             E.data(), delaytime.data(), MuOnCAN.data(),
                             evttime, 1);
        } else /* events under the energy of threshold */ {
          ++Nlow;
          generationOK = false;
          continue;
        }
      } else {
        Nlow += weight;
        generationOK = false;
        continue;
      }
    }
    if (generationOK) i++;
  }

  if (livetime_fout.is_open()) {
    livetime_fout << std::endl;
    livetime_fout << "Generated events: " << events << std::endl;
    livetime_fout << "Generated events with multiplicity " << MULTmin
                  << " on the upper disk of the CAN: " << counter << std::endl;
    livetime_fout << "\nEvents with energy higher than energy of threshold: "
                  << events << std::endl;
    livetime_fout << "Events with energy lower than energy of threshold: "
                  << Nlow << std::endl;
    livetime_fout << std::endl;
    livetime_fout << "theta (degrees)    N*   Delta N*    N*_MC     T (sec) "
                  << "  Delta T\n";
    livetime_fout << "******************************************************"
                  << "**********\n";
  }

  double avlivetime, eavlivetime;
  CalculateLivetime(thetarange, ncounter, enlargedCAN, genrandom, livetime_fout,
                    avlivetime, eavlivetime);
  OutWrt->WriteLiveTime(avlivetime, eavlivetime);

  delete OutWrt;
}

void ParseArgs(int argc, char **argv) {
  CLI::App app{
      "Generate atmospheric muon bundles from parametric formulas.\n"
      "Normal workflow: provide -p/--config with a parameters file, then use\n"
      "command-line options for run settings and any needed overrides.\n"
      "Use --help-all to show expert physics-model parameters.\n"
      "Running example:\n"
      "./mupage.exe -i 1 -n 10 -s 12345678 -o mupage.txt -p "
      "dat/parameters.dat\n"
      "compare output with benchmark/mupage.txt"};

  app.set_help_all_flag("--help-all",
                        "Print full help, including expert physics parameters");
  // app.failure_message(CLI::FailureMessage::help);
  app.formatter_fn(
      [](const CLI::App *app, std::string name, CLI::AppFormatMode mode) {
        auto fmt = std::make_shared<MupageHelpFormatter>();
        fmt->column_width(36);
        fmt->right_column_width(72);
        fmt->label("OPTIONS", "Options");
        fmt->enable_option_defaults(mode == CLI::AppFormatMode::All);
        fmt->enable_option_type_names(mode == CLI::AppFormatMode::All);
        fmt->enable_default_flag_values(mode == CLI::AppFormatMode::All);
        return fmt->make_help(app, name, mode);
      });

  //make error on unknown variables in the config file
  app.allow_config_extras(CLI::config_extras_mode::error);
  // config file i.e. parameter file
  std::vector<std::string> filenames;
  app.add_option("-o,--output", filenames,
                 "Output event file and optional livetime file")
      ->required()
      ->expected(1, 2)
      ->group("Run Options");
  app.set_config("-p,--config", "",
                 "Read parameters from config file (normal workflow)", true)
      ->check(CLI::ExistingFile)
      ->group("Run Options");
  app.get_config_formatter_base()->valueSeparator(' ');

  // Define options
  app.add_option("-i,--runnumber", numrun, "MC run number")
      ->required()
      ->check(CLI::Range(1, INT_MAX))
      ->group("Run Options");  // UINT_MAX not supported

  app.add_option("-n,--nevents", events, "Number of events to generate")
      ->required()
      ->check(CLI::Range(1, INT_MAX))
      ->group("Run Options");  // ULONG_MAX not supported

  app.add_option("-s,--seed", seed, "Random number seed")
      ->required()
      ->check(CLI::Range(1, INT_MAX))
      ->group("Run Options");  // ULONG_MAX not supported

  std::string ts;
  app.add_option("-t,--timerange", ts,
                 "Generation time interval as <tmin,tmax>")
      ->capture_default_str()
      ->group("Output / Timing");
  CLI::deprecate_option(app, "-t", "--tmin");

  app.add_option("--tmin", TimeStampStart, "Generation time interval min time")
      ->capture_default_str()
      ->group("Output / Timing");

  app.add_option("--tmax", TimeStampStop, "Generation time interval max time")
      ->capture_default_str()
      ->group("Output / Timing");

  std::map<std::string, std::string> formats;
  formats["native"] = "native ASCII format";
#ifdef _ANTARES_ENABLED__
  formats["antares"] = "ANTARES evt format";
#endif
#ifdef _KM3NET_ENABLED__
  formats["km3net"] = "KM3NeT ROOT format";
#endif

  std::string fhelper =
      "Output file format\n"
      "Output file format\n"
      "possible choices are:";
  for (const auto &pair : formats) {
    fhelper += "\n - ";
    fhelper += pair.first;
    fhelper += " : ";
    fhelper += pair.second;
  }

  app.add_option("-f,--format", OutputFormat, fhelper)
      ->capture_default_str()
      ->check(CLI::IsMember(formats))
      ->group("Output / Timing");

  app.add_option("-q,--timeorder", TimeOrder,
                 "Order generated event times (0 = no, 1 = yes)")
      ->capture_default_str()
      ->group("Output / Timing");

  app.add_option("-w,--weighted", doweighted,
                 "Generate weighted events (0 = no, 1 = yes)")
      ->capture_default_str()
      ->group("Output / Timing");

  // config variables processing
  app.add_option("--DEPTHmax", DEPTHmax, "maximum depth in km (not in km.w.e.)")
      ->required()
      ->group("Config Overrides");

  app.add_option("--Zmin", Zmin, "minimum z in m")
      ->required()
      ->group("Config Overrides")
      ->capture_default_str();
  app.add_option("--Zmax", Zmax, "maximum z in m")
      ->required()
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--CANr", CANr, "can radius in m")
      ->required()
      ->group("Config Overrides")
      ->capture_default_str();
  app.add_option("--EnlargedCANr", EnlargedCANr,
                 "meters to add to the can radius")
      ->group("Config Overrides");

  app.add_option("--can_center_x", can_center_x, "center of can X in m")
      ->capture_default_str()
      ->group("Config Overrides");
  app.add_option("--can_center_y", can_center_y, "center of can Y in m")
      ->capture_default_str()
      ->group("Config Overrides");

  app.add_option("--THETAmin", THETAmin, "minimum zenith angle in degrees")
      ->check(CLI::NonNegativeNumber)
      ->group("Config Overrides")
      ->capture_default_str();
  app.add_option("--THETAmax", THETAmax, "maximum zenith angle in degrees")
      ->check(CLI::Range(0., 85.))
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--Rmin", Rmin, "minimum radial distribution in m")
      ->check(CLI::NonNegativeNumber)
      ->group("Config Overrides")
      ->capture_default_str();
  app.add_option("--Rmax", Rmax, "maximum radial distribution in m")
      ->check(CLI::NonNegativeNumber)
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--Ethreshold", Ethreshold, "minimum energy threshold in TeV")
      ->check(CLI::Range(static_cast<double>(Emin) - 1e-8, 100000000.))
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--MULTmin", MULTmin, "minimum multiplicity")
      ->check(CLI::PositiveNumber)
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--MULTmax", MULTmax, "maximum multiplicity")
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--GEANTid", GEANTid, "particle ID (muon in GEANT convention)")
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--density", density, "mean seawater density in g/cm^-3")
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--Emax", Emax, "maximum energy of generated muons in TeV")
      ->check(CLI::Range(500., 100000000.))
      ->group("Config Overrides")
      ->capture_default_str();

  app.add_option("--K0a", K0a, "parameter K1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--K1a", K1a, "parameter K1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--K0b", K0b, "parameter K0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--K1b", K1b, "parameter K1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ni0a", ni0a, "parameter ni0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ni0b", ni0b, "parameter ni0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ni0c", ni0c, "parameter ni0c")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ni1a", ni1a, "parameter ni1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ni1b", ni1b, "parameter ni1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--BETA", BETA, "parameter BETA")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--g0", g0, "parameter g0")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--g1", g1, "parameter g1")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--e0a", e0a, "parameter e0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--e0b", e0b, "parameter e0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--e1a", e1a, "parameter e1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--e1b", e1b, "parameter e1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--A0", A0, "parameter A0")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--A1", A1, "parameter A1")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--B0a", B0a, "parameter B0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--B0b", B0b, "parameter B0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--B1a", B1a, "parameter B1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--B1b", B1b, "parameter B1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--Q0", Q0, "parameter Q0")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--Q1", Q1, "parameter Q1")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--C0a", C0a, "parameter C0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--C0b", C0b, "parameter C0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--C1", C1, "parameter C1")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--D0a", D0a, "parameter D0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--D0b", D0b, "parameter D0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--D1a", D1a, "parameter D1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--D1b", D1b, "parameter D1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--RHO0a", RHO0a, "parameter RHO0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--RHO0b", RHO0b, "parameter RHO0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--RHO1", RHO1, "parameter RHO1")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--THETA0", THETA0, "parameter THETA0")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--Fpiccolo", Fpiccolo, "parameter Fpiccolo")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  app.add_option("--ALPHA0a", ALPHA0a, "parameter ALPHA0a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ALPHA0b", ALPHA0b, "parameter ALPHA0b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ALPHA1a", ALPHA1a, "parameter ALPHA1a")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();
  app.add_option("--ALPHA1b", ALPHA1b, "parameter ALPHA1b")
      ->group("Flux parametrisation parameters")
      ->capture_default_str();

  // CLI11_PARSE(app, argc, argv);
  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    app.exit(e);  // to print help and error
    throw e;
  }

  // Post processing
  OutputFileName = filenames[0];
  if (filenames.size() >= 2) LivetimeFileName = filenames[1];

  /*
    if (ts.size() == 2) {
      TimeStampStart = ts[0];
      TimeStampStop  = ts[1];
    }
  */

  if (ts != "") {
    std::vector<std::string> timeinterval;
    std::stringstream ss(ts);
    std::string item;
    while (std::getline(ss, item, ',')) {
      timeinterval.push_back(item);
    }
    if (timeinterval.size() != 2) {
      auto e = CLI::ValidationError{
          "For -t use two comma separated integer values."};
      app.exit(e);  // to print error
      throw e;
    }
    TimeStampStart = std::stoi(timeinterval[0].c_str());
    TimeStampStop = std::stoi(timeinterval[1].c_str());
  }

  if ((TimeStampStart != 0) || (TimeStampStop != 0)) {
    if (TimeStampStop <= TimeStampStart) {
      auto e = CLI::ValidationError{
          "For -t and/or --tmin --tmax respect tmin < tmax condition."};
      app.exit(e);  // to print error
      throw e;
    } else {
      GenTime = true;
    }
  }

  if (Zmax <= Zmin) {
    auto e = CLI::ValidationError{
        "For --Zmin --Zmax respect Zmin < Zmax condition."};
    app.exit(e);  // to print error
    throw e;
  }

  if (Rmax <= Rmin) {
    auto e = CLI::ValidationError{
        "For --Rmin --Rmax respect Rmin < Rmax condition."};
    app.exit(e);  // to print error
    throw e;
  }

  // in future: app.config_to_str(CLI::ConfigOutputMode::AllDefaults)
  std::cout << app.config_to_str(true) << std::endl;
}

/* compute live-time and inverse-variance weight and adds them to the sums*/
void AddLivetimeThetaRange(unsigned int thetamin, unsigned int thetamax,
                           std::vector<unsigned int> ncounter,
                           const Vec3D &enlargedCAN, Random &genrandom,
                           std::ofstream &livetime_fout, double &sumnum,
                           double &sumweight) {
  unsigned int rangecounter = 0;
  unsigned int erangecounter = 0;
  Muons muon(Qmin(), 0., MULTmin, weDEPTHmin(), genrandom);

  for (unsigned int jj = thetamin; jj < thetamax; ++jj)
    rangecounter += ncounter[jj];
  erangecounter = (int)(sqrt((double)rangecounter) + 0.5);
  muon.Set((double)(static_cast<double>(thetamin + thetamax) / 2. * DegToRad()),
           0., MULTmin, weDEPTHmin());
  muon.SetParameters(DEPTHmax, enlargedCAN.x, enlargedCAN.y, enlargedCAN.z,
                     Rmin, Rmax, Emin, Emax);
  double N_MC =
      muon.GAMMA((double)(thetamin)*DegToRad(), (double)(thetamax)*DegToRad());
  double ilivetime = (double)rangecounter / N_MC;
  double ielivetime = (double)erangecounter / N_MC;
  double iweight = 0;
  if (ielivetime != 0) iweight = 1. / (ielivetime * ielivetime);

  if (livetime_fout.is_open()) {
    livetime_fout.setf(std::ios::fixed | std::ios::right);
    livetime_fout << std::setw(5) << "]" << std::setw(2) << thetamin << ","
                  << thetamax << "]" << std::setw(10) << rangecounter
                  << std::setw(9) << erangecounter << std::setprecision(1)
                  << std::setw(10) << N_MC;
    livetime_fout.unsetf(std::ios::fixed);
    livetime_fout << std::setprecision(3) << std::setw(11) << ilivetime
                  << std::setprecision(1) << std::setw(10) << ielivetime
                  << " * " << std::endl;
  }

  sumnum += ilivetime * iweight;
  sumweight += iweight;
}

/* compute average live-time using inverse-variance weighting*/
void CalculateLivetime(const unsigned int &thetarange,
                       std::vector<unsigned int> ncounter,
                       const Vec3D &enlargedCAN, Random &genrandom,
                       std::ofstream &livetime_fout, double &avlivetime,
                       double &eavlivetime) {
  double sumweight = 0.;
  double sumnum = 0.;

  /* range of 0 to 9 degrees */
  AddLivetimeThetaRange(0, 10, ncounter, enlargedCAN, genrandom, livetime_fout,
                        sumnum, sumweight);

  /* range of 10 to 70 degrees with bin size 2*/
  for (unsigned int jj = 10; jj < 70; jj += 2) {
    AddLivetimeThetaRange(jj, jj + 2, ncounter, enlargedCAN, genrandom,
                          livetime_fout, sumnum, sumweight);
  }

  /* range of 70 to 85 degrees */
  for (unsigned int theta = 70; theta < 85; theta += 1) {
    AddLivetimeThetaRange(theta, theta + 1, ncounter, enlargedCAN, genrandom,
                          livetime_fout, sumnum, sumweight);
  }

  avlivetime = sumnum / sumweight;
  eavlivetime = 1. / (sqrt(sumweight));

  if (livetime_fout.is_open()) {
    livetime_fout.unsetf(std::ios::scientific);
    livetime_fout << std::endl;
    livetime_fout << std::setprecision(3) << "Average livetime = ("
                  << avlivetime << " pm " << std::setprecision(1) << eavlivetime
                  << ") sec\n";
    livetime_fout << std::setprecision(3) << "                 = ("
                  << avlivetime * sTOdays() << " pm " << std::setprecision(1)
                  << eavlivetime * sTOdays() << ") days\n";
    livetime_fout << std::endl;

    livetime_fout.close();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintHeader(void) {
  using namespace std;

  cout << endl;
  std::ifstream f("logo.txt");
  if (f.is_open()) std::cout << f.rdbuf();
  cout << "Version: " << versn << endl;

  return;
}

/*! The MUPAGE code was developed for the production of atmospheric
multimuons on the underwater detector CAN surface, without a
full Monte Carlo simulation but using parametric formulas.
MUPAGE is for ANTARES users.
*/
