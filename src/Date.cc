// #############################################################
// #                                                           #
// #    Authors: G.Carminati, A.Sottoriva                      #
// #    First Release: Jan 2006                                #
// #                                                           #
// #############################################################
//
//      to write processing date and time
//
// #############################################################

#include "Date.hh"

#include <time.h>

#include <cstdio>
#include <iostream>

// convert the integer number of date or time in a string
std::string Date::character(int d) {
  char temp[16];
  date_ = d;
  sprintf(temp, "%02d", d);
  std::string tt = temp;
  return tt;
}

std::string Date::date_clock(std::string date_clock) {
  // inizialization time
  time_t rawtime;
  struct tm *timeinfo;
  int year, month, day, hour, min;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  year = timeinfo->tm_year - 100;
  month = timeinfo->tm_mon + 1;
  day = timeinfo->tm_mday;
  hour = timeinfo->tm_hour;
  min = timeinfo->tm_min;
  mktime(timeinfo);

  std::string temp1 = this->character(year);
  std::string temp2 = this->character(month);
  std::string temp3 = this->character(day);
  std::string temp4 = this->character(hour);
  std::string temp5 = this->character(min);
  std::string date = temp1 + temp2 + temp3;
  std::string clock = temp4 + temp5;
  date_clock = date + " " + clock;
  return date_clock;
}
