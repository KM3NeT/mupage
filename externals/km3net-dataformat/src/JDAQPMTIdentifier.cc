#include "km3net-dataformat/online/JDAQPMTIdentifier.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQPMTIdentifier::Streamer(TBuffer& R__b)
{
  JDAQModuleIdentifier::Streamer(R__b);

  if (R__b.IsReading())
    R__b >> address;
  else
    R__b << address;
}
