#ifndef EVT_HH_INCLUDED
#define EVT_HH_INCLUDED

#include "km3net-dataformat/offline/AAObject.hh"
#include "km3net-dataformat/offline/Hit.hh"
#include "km3net-dataformat/offline/Trk.hh"
#include "km3net-dataformat/offline/Hit.hh"
#include "km3net-dataformat/offline/Exception.hh"

#include "TTimeStamp.h"

#include <uuid/uuid.h>
#include <vector>

/**
 * The Evt class respresent a Monte Carlo (MC) event as well as an offline event.\n
 * Some data from the online (DAQ) event are copied.
 */

struct Evt: public AAObject
{
  int id;                       ///< offline event identifier
  int det_id;                   ///< detector identifier from DAQ
  int mc_id;                    ///< identifier of the MC event (as found in ascii or antcc file).

  int run_id;                   ///< DAQ run identifier
  int mc_run_id;                ///< MC run identifier

  int frame_index;              ///< from the raw data
  ULong64_t trigger_mask;       ///< trigger mask from raw data (i.e. the trigger bits)
  ULong64_t trigger_counter;    ///< trigger counter
  unsigned int overlays;        ///< number of overlaying triggered events
  TTimeStamp t;                 ///< UTC time of the timeslice, or the event_time for MC. (default: 01 Jan 1970 00:00:00)

  uuid_t header_uuid;           ///< UUID of header containing the event-weight information

  //hits and tracks
  std::vector<Hit> hits;        ///< list of hits
  std::vector<Trk> trks;        ///< list of reconstructed tracks (can be several because of prefits,showers, etc).

  //Monte carlo
  std::vector<double> w;         ///< MC: Weights w[0]=w1, w[1]=w2, w[2]=w3 (see e.g. <a href="https://simulation.pages.km3net.de/taglist/taglist.pdf">Tag list</a> or km3net-dataformat/definitions)
  std::vector<double> w2list;    ///< MC: factors that make up w[1]=w2       (see e.g. <a href="https://simulation.pages.km3net.de/taglist/taglist.pdf">Tag list</a> or km3net-dataformat/definitions)
  std::vector<double> w3list;    ///< MC: atmospheric flux information

  TTimeStamp mc_event_time;      ///< MC: true generation time (UTC) of the event, (default: 01 Jan 1970 00:00:00)
  double mc_t;                   ///< MC: time where the mc-event was put in the timeslice, since start of run (offset+frameidx*timeslice_duration)
  std::vector<Hit> mc_hits;      ///< MC: list of MC truth hits
  std::vector<Trk> mc_trks;      ///< MC: list of MC truth tracks

  // --- place to store user info ---
  TString  comment;              ///< user can use this as he/she likes
  int      index;                ///< user can use this as he/she likes
  int      flags;                ///< user can use this as he/she likes


  /**
   * Default constructor.
   */
  Evt() :
    id(0), det_id(0), mc_id(0), run_id(0), mc_run_id(0), frame_index(0),
    trigger_mask(0), trigger_counter(0),
    overlays(0), t(0), mc_event_time(0), mc_t(0), index(0), flags(0)
  {
    uuid_clear(this->header_uuid);
  }


  /**
   * Print event.
   *
   * \param  out           output stream
   */
  void print(std::ostream& out = std::cout) const
  {
    out << "Evt: id=" << id <<
        " run_id=" << run_id <<
        " #hits=" << hits.size() <<
        " #mc_hits=" << mc_hits.size() <<
        " #trks=" << trks.size() <<
        " #mc_trks=" << mc_trks.size() << std::endl;
  }


  /**
   * Reset event.
   */
  void clear()
  {
    *this = Evt();
  }

  
  /**
   * Clear the hit vectors and all the references to hits in the tracks
   */
  void clear_hits()
  {
    hits.clear();
    mc_hits.clear();
    for (auto& t : trks )    t.hit_ids.clear();
    for (auto& t : mc_trks ) t.hit_ids.clear();
  }


  /**
   * Return a vector with const pointers to the tracks that are 'primary'.\n
   * Here, primary means the tracks have no parents.
   *
   * This method only works if MC parent-child relations are availabe.
   *
   * \return               list of pointers to primary tracks
   */
  std::vector<const Trk*> primary_trks() const
  {
    std::vector<const Trk*> r;
    for (auto& t : mc_trks )
    {
      if ( t.is_primary() ) r.push_back(&t);
    }
    return r;
  }


 /**
   * Return a vector of pointers to tracks that are 'primary'.\n
   * Here, primary means the tracks have no parents.
   *
   * \return               list of pointers to primary tracks
   */
  std::vector<Trk*> primary_trks()
  {
    std::vector<Trk*> r;
    for (auto& t : mc_trks )
    {
      if ( t.is_primary() ) r.push_back(&t);
    }
    return r;
  }


  /**
   * Get a const pointer to the (first) neutrino from the MC track list.
   *
   * \return pointer to neutrino (nullptr if no neutrino is in the list)
   */
  const Trk* neutrino() const
  {
    for (auto& t : mc_trks )
    {
      if ( t.is_neutrino() ) return &t;
    }
    return nullptr;
  }

  /**
   * Get a pointer to the (first) neutrino from the MC track list.
   *
   * \return const pointer to neutrino (nullptr if no neutrino is in the list)
   */

  Trk* neutrino()
  {
    // see Effective C++, Scott Meyers, ISBN-13: 9780321334879.
    return const_cast<Trk *>(static_cast<const Evt &>(*this).neutrino() );
  }


  /**
   * Get a const pointer to primary neutrino from the MC track list.
   *
   * Only works if MC parent-child relations are availabe.
   *
   * \return const pointer to primary neutrino (may be nullptr)
   */
  const Trk* primary_neutrino() const
  {
    for ( auto& t : mc_trks )
    {
      if ( t.is_neutrino() and t.is_primary() ) return &t;
    }
    return nullptr;
  }

  /**
   * Get a pointer to primary neutrino from the MC track list.
   *
   * Only works if MC parent-child relations are availabe.
   *
   * \return pointer to primary neutrino
   */
  Trk* primary_neutrino()
  {
    return const_cast<Trk *>(static_cast<const Evt &>(*this).primary_neutrino() );
  }


  /**
   * Get a const pointer to the first leading lepton from the MC track list.
   * Here, leading means the lepton that has a neutrino as mother.
   *
   * Only works if MC parent-child relations are availabe.
   *
   * \return pointer to leadig lepton (may be nullptr in case not found)
   */
  const Trk* leading_lepton() const
  {
    const Trk* nu = primary_neutrino();
    if (!nu) return nullptr;

    for (auto& t : mc_trks )
    {
      if ( t.is_lepton()    &&
      t.mother_id == nu->id &&
      !t.is_orphan() ) return &t;
    }
    return nullptr;
  }

  /**
   * Get a pointer to leading lepton from the MC track list.
   * Here, leading means the lepton that has a neutrino as mother.
   *
   * Only works if MC parent-child relations are availabe.
   *
   * \return pointer to leadig lepton (may be nullptr in case not found)
   */
  Trk* leading_lepton()
  {
    return const_cast<Trk *>(static_cast<const Evt &>(*this).leading_lepton() );
  }

  /**
   * Get a const pointer to the (first) parent of the track 'child'.\n
   * This method return nullptr if no parent is found.
   *
   * \param  child         child particle
   * \return               pointer to parent
   */
  const Trk * get_parent_of( const Trk & child ) const
  {
    for (auto& t : mc_trks )
    {
      if (child.mother_id == t.id ) return &t;
    }
    return nullptr;
  }


 /**
   * Get a pointer ot the (first) parent of the track 'child'.\n
   * This method return nullptr if no parent is found.
   *
   * \param  child         child particle
   * \return               pointer to parent
   */

  Trk* get_parent_of( const Trk & child )
  {
    return const_cast<Trk *>(static_cast<const Evt &>(*this).get_parent_of(child) );
  }


  /**
   * Action method at file open.
   *
   * \param  version   version
   */
  static void actionAtFileOpen(int version)
  {
    ROOT_IO_VERSION = version;
  }

  static int ROOT_IO_VERSION;    //!< Streamer version as obtained from ROOT file.

  ClassDef(Evt, 16)
};

#endif
