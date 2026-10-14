/*#############################################################
 *#                                                           #
 *#    Author: V. Kulikovskiy                                 #
 *#                                                           #
 *#    Date: 28 May 2026                                      #
 *#    Class to generate the event time                       #
 *#                                                           #
 *#############################################################
 */

#include "EventTime.hh"

#include <iostream>

#include "Random.hh"

EventTime::EventTime(unsigned long events, bool _GenTime, bool _TimeOrder,
                     double _TimeStampStart, double _TimeStampStop,
                     Random *_rnd)
    : GenTime(_GenTime),
      TimeOrder(_TimeOrder),
      TimeStampStart(_TimeStampStart),
      TimeStampStop(_TimeStampStop),
      rnd(_rnd) {
  sumT = 0.;
  evttime = 0.;
  if (GenTime && TimeOrder) {
    /*
     * Assumption:
     * - probability of the event appearence in time T0+dx is equal to dp=dt/tau
     * - this probability does not depend on the T0 so one can choose T0 as the
     * time of the previous
     * - to generate n event times one can generate n+1 time intervals and
     * "compact" them to the needed period T
     * - by renormalizing to T/Sum_{i=0}^{n} dT_i
     * - each interval is the exponential random variable (starting from any
     * given time).
     * - average delay time (tau) is not important since later the sum is
     * normalized to 1
     * https://stats.stackexchange.com/questions/348358/a-fast-uniform-order-statistic-generator
     */
    double tempT;

    for (unsigned long i = 0; i <= events; i++) {
      tempT = rnd->Exp(1.);
      deltaT.push(tempT);
      sumT += tempT;
    }
    sumT = (double)(TimeStampStop - TimeStampStart) / sumT;  // to speed up
                                                             // later
    evttime = double(TimeStampStart);
  }
}

/* generate the event time */
double EventTime::GetTime() {
  if (!GenTime) return 0.;
  if (TimeOrder) {
    if (deltaT.size() > 0) {
      evttime = evttime + deltaT.front() * sumT;
      deltaT.pop();
    } else {
      std::cerr << "Time stack is empty" << std::endl;
    }
  } else {
    evttime = double(TimeStampStart) +
              (double)(TimeStampStop - TimeStampStart) * rnd->Rndm();
  }
  return evttime;
}
