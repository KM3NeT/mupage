#include "km3net-dataformat/online/JDAQSummaryFrame.hh"


/**
 * \author mdejong
 */

/**
 * Streamer version of JDAQSummaryslice as obtained from ROOT file.
 */
int KM3NETDAQ::JDAQSummaryFrame::ROOT_IO_VERSION = -1;


/**
 * Custom ROOT streamer.
 *
 * \param  R__b          I/O buffer
 */
void KM3NETDAQ::JDAQSummaryFrame::Streamer(TBuffer& R__b)
{
  JDAQModuleIdentifier::Streamer(R__b);

  if (R__b.IsReading()) {

    switch (ROOT_IO_VERSION) {

    case -1:
    case  0:
    case  1:
    case  2:
    case  3:
      break;

    case  4:
      R__b >> status;
      break;
      
    default:
      R__b >> daq;
      R__b >> status;
      R__b >> fifo;
      R__b >> status_3;
      R__b >> status_4;
      break;
    }

    for (int i = 0; i != NUMBER_OF_PMTS; ++i) {
      R__b >> data[i].value;
    }

  } else {

    R__b << daq;
    R__b << status;
    R__b << fifo;
    R__b << status_3;
    R__b << status_4;

    for (int i = 0; i != NUMBER_OF_PMTS; ++i) {
      R__b << data[i].value;
    }
  }
}
