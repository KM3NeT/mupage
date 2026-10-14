#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQModuleIdentifier::Streamer(TBuffer& R__b)
{
  if (R__b.IsReading())
    R__b >> id;
  else
    R__b << id;
}
