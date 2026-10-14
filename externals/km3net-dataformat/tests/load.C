#include "TSystem.h"
#include "km3net-dataformat/online/JDAQHit.hh"

/**
 * Check we can load the library and access at least
 * one class defined in that library
 */
int load() {
  int ok = gSystem->Load("libKM3NeTROOT");
  if (ok == 0) {
    KM3NETDAQ::JDAQHit j;
  }
  return ok;
}
