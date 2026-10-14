#include "km3net-dataformat/online/JDAQKeyHit.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQKeyHit::Streamer(TBuffer& R__b)
{
  JDAQModuleIdentifier::Streamer(R__b);
  JDAQHit             ::Streamer(R__b);
}
