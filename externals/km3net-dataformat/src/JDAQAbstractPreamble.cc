#include "km3net-dataformat/online/JDAQAbstractPreamble.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQAbstractPreamble::Streamer(TBuffer& R__b)
{
  if (R__b.IsReading()) {
    R__b >> length;
    R__b >> type;
  } else {
    R__b << length;
    R__b << type;
  }
}
