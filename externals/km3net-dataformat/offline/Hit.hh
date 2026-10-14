#ifndef HIT_HH_INCLUDED
#define HIT_HH_INCLUDED

#include "TObject.h"
#include "TString.h"
#include "km3net-dataformat/offline/Vec.hh"

struct Hit :
  public TObject
{
  int id;

  // straight from the data
  int dom_id;                          ///< module identifier from the data (unique in the detector).
  unsigned int channel_id;             ///< PMT channel id {0,1, .., 30} local to moduke
  unsigned int tdc;                    ///< hit tdc (=time in ns)
  unsigned int tot;                    ///< tot value as stored in raw data (int for pyroot)
  ULong64_t trig;                      ///< non-zero if the hit is a trigger hit.

  int pmt_id;                          ///< global PMT identifier as found in evt files

  // values after calibration
  double t;                            ///< hit time (from tdc+calibration or MC truth)
  double a;                            ///< hit amplitude (in p.e.)
  Vec pos;                             ///< hit position
  Vec dir;                             ///< hit direction; i.e. direction of the PMT

  int type;                            ///< particle type or parametrisation used for hit (mc only)
  int origin;                          ///< track id of the track that created this hit (mc only)

  unsigned pattern_flags;              ///< some number that you can use to flag the hit

  /**
   * Default constructor.
   */
  Hit(): id(0), dom_id(0), channel_id(0), tdc(0), tot(0), trig(0), pmt_id(0),
    t(0), a(0), type(0), origin(0), pattern_flags(0) {}
  //virtual ~Hit() {}


  /**
   * Read hit (useful in python).
   *
   * \param  h             hit
   */
  void read(const Hit& h) { *this = h;}

  /**
   * Write hit (useful in python).
   *
   * \param  h             hit
   */
  void write(Hit& h) const { h = *this;}

  /**
   * Print hit.
   *
   * \param  out           output stream
   */
  void print( std::ostream& out = std::cout ) const
  {
    out << "Hit: id=" << id << " dom=" << dom_id << " channel=" << channel_id;
    out << " pmt=" << pmt_id << " t=" << t << " tot=" << tot;
    out << " pos="; pos.print(out);
    out << " dir="; dir.print(out);
  }

  ClassDefNV(Hit, 106) // reserve <100 for antcc class of the same name
};

#endif
