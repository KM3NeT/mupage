#ifndef __TOOLS__TIME_CONVERTER__
#define __TOOLS__TIME_CONVERTER__

#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/online/JDAQClock.hh"
#include "km3net-dataformat/online/JDAQChronometer.hh"


/**
 * \file
 * Auxiliary include file for time conversion between DAQ/trigger hit and Monte Carlo hit.
 *
 * \author mdejong
 */

/**
 * Auxiliary class to convert DAQ hit time to/from Monte Carlo hit time. 
 */
class time_converter
{
public:
  /**
   * Default constructor.
   */
  time_converter() :
    __t0(0.0),
    __t1(0.0)
  {}


  /**
   * Constructor.
   * Note that this constructor should only be used after incorporation of KM3NETDAQ::JDAQEvent.
   *
   * \param  event         event
   */
  time_converter(const Evt& event) :
    __t0(event.mc_t),
    __t1(getTimeOfRTS(event.frame_index))
  {}


  /**
   * Constructor.
   *
   * \param  event         Monte Carlo event
   * \param  chronometer   DAQ chronometer
   */
  time_converter(const Evt&             event,
		 const JDAQChronometer& chronometer) :
    __t0(event.mc_t),
    __t1(getTimeOfRTS(chronometer))
  {}


  /**
   * Get DAQ/trigger time minus Monte Carlo time. 
   *
   * \return               time difference [ns]
   */
  double getTime() const
  {
    return __t1 - __t0;
  }


  /**
   * Get Monte Carlo time minus DAQ/trigger time.
   *
   * \return               time difference [ns]
   */
  double putTime() const
  {
    return __t0 - __t1;
  }


  /**
   * Get Monte Carlo hit time.
   *
   * \param  t0            DAQ/trigger hit time [ns]
   * \return               Monte Carlo hit time [ns]
   */
  double getTime(const double t0) const
  {
    return t0 + __t1 - __t0;
  }


  /**
   * Get DAQ hit time.
   *
   * \param  t0            Monte Carlo hit time [ns]
   * \return               DAQ/trigger hit time [ns]
   */
  double putTime(const double t0) const
  {
    return t0 - __t1 + __t0;
  }

protected:
  double __t0;   // Monte Carlo event time [ns]
  double __t1;   // DAQ RTS [ns]
};

#endif
