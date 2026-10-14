#include "km3net-dataformat/online/JDAQPreamble.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQPreamble::Streamer(TBuffer& R__b)
{
  if (R__b.IsReading()) {
    R__b >> length;
    R__b >> type;
  } else {
    R__b << length;
    R__b << type;
  }
}
