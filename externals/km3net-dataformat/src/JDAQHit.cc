#include "km3net-dataformat/online/JDAQHit.hh"


/**
 * \author mdejong
 */

/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQHit::Streamer(TBuffer& R__b)
{
  if (R__b.IsReading()) {
    R__b >> pmt;
    R__b >> tdc;
    R__b >> tot;
  } else {
    R__b << pmt;
    R__b << tdc;
    R__b << tot;
  }
}
