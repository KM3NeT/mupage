/*#############################################################
 *#                                                           #
 *#    Author: G.Carminati                                    #
 *#    First Release: Mar 2007                                #
 *#                                                           #
 *#############################################################
 */

#ifndef EVENTTIME_h
#define EVENTTIME_h

#include <queue>

#include "Random.hh"

class EventTime {
 public:
  EventTime() {};
  EventTime(unsigned long events, bool _GenTime = false,
            bool _TimeOrder = false, double _TimeStampStart = 0.,
            double _TimeStampStop = 0.,
            Random *_rnd = NULL);  //!< constructor
  virtual ~EventTime() {};         //!< virtual destructor
  double GetTime();                //!< get event time

 private:
  std::queue<double> deltaT;  // vector with random times
  double sumT;                // normalisation to rescale times to a given range
  double evttime;             // time of the last generated event
  bool GenTime;
  bool TimeOrder;
  double TimeStampStart;
  double TimeStampStop;
  Random *rnd;
};

#endif /*EVENTTIME_h*/
