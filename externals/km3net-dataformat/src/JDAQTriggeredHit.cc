#include "km3net-dataformat/online/JDAQTriggeredHit.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQTriggeredHit::Streamer(TBuffer& R__b)
{
  JDAQKeyHit     ::Streamer(R__b);
  JDAQTriggerMask::Streamer(R__b);
}
