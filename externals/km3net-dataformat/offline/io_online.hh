#ifndef IOONLINEINCLUDED
#define IOONLINEINCLUDED

#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/offline/Hit.hh"
#include "km3net-dataformat/offline/Trk.hh"
#include "km3net-dataformat/offline/Exception.hh"

#include "km3net-dataformat/online/JDAQEvent.hh"
#include "km3net-dataformat/online/JDAQKeyHit.hh"
#include "km3net-dataformat/online/JDAQTimeslice.hh"
#include "km3net-dataformat/online/JDAQSummaryslice.hh"

#include "TStreamerInfo.h"
#include "TFile.h"
#include "TTree.h"
#include "km3net-dataformat/definitions/root.hh"

#include <map>
#include <vector>


using namespace KM3NETDAQ;

/**
 * Read a hit from a DAQ hit.
 *
 * \param  hit           hit
 * \param  daqhit        DAQ hit
 */
inline void read(Hit& hit, const JDAQHit& daqhit )
{
  hit.channel_id = daqhit.getPMT();
  hit.tot        = daqhit.getToT();
  hit.tdc        = daqhit.getT(); // GetT() just return the bare TDC
}

/**
 * Read a hit from a DAQ key hit.
 *
 * \param  hit           hit
 * \param  daqhit        DAQ key hit
 */
inline void read(Hit& hit, const JDAQKeyHit& daqhit )
{
  hit.id = hit.pmt_id = 0;

  hit.dom_id     = daqhit.getModuleID();
  hit.channel_id = daqhit.getPMT();
  hit.tot        = daqhit.getToT();
  hit.tdc        = daqhit.getT(); // GetT() just return the bare TDC
}

/**
 * Read an event from a DAQ event.
 *
 * \param  evt           evt
 * \param  de            DAQ event
 */
inline void read(Evt& evt, const JDAQEvent& de)
{
  evt.run_id          = de.getRunNumber();
  evt.det_id          = de.getDetectorID();
  evt.frame_index     = de.getFrameIndex();
  evt.trigger_counter = de.getCounter();
  evt.overlays        = de.getOverlays();
  evt.trigger_mask    = de.getTriggerMask();
  evt.t.SetSec(     de.getTimesliceStart().getUTCseconds() );
  evt.t.SetNanoSec( de.getTimesliceStart().getUTC16nanosecondcycles() * 16 );

  // The only way to know the hits that are also in the triggeredhits collection
  // is by dom and channel id and time.

  Hit h;
  std::map<int, std::map< int, std::map < unsigned int, Hit*> > > M;

  const std::vector<JDAQSnapshotHit>&   snapshotHits = de.getHits<JDAQSnapshotHit>();
  const std::vector<JDAQTriggeredHit>& triggeredHits = de.getHits<JDAQTriggeredHit>();

  // http://stackoverflow.com/questions/10735135/reallocation-in-stdvector-after-stdvector-reserve
  evt.hits.clear();
  evt.hits.reserve(snapshotHits.size());

  for (auto& daqhit : snapshotHits ) // JDAQSnapshotHit
  {
    read( h, daqhit );
    h.trig = 0;
    evt.hits.push_back( h );
    M[ h.dom_id ][ h.channel_id ][ h.tdc ] = &(*evt.hits.rbegin());
  }

  for (auto& daqtrighit : triggeredHits)
  {
    Hit* g = M[daqtrighit.getModuleID()][daqtrighit.getPMT()][daqtrighit.getT()];

    if (g)
      g->trig = daqtrighit.getTriggerMask ();
    else
      THROW(Exception, "Failed to flag snaphot hit " << (int) daqtrighit.getModuleID() << "." << (int) daqtrighit.getPMT() << " " << daqtrighit.getT());
  }
}

/**
 * Read an event from a DAQ time slice.
 *
 * \param  evt           evt
 * \param  ts            DAQ time slice
 */

inline void read( Evt& evt, const JDAQTimeslice& ts )
{
  evt.run_id          = ts.getRunNumber();
  evt.det_id          = ts.getDetectorID();
  evt.frame_index     = ts.getFrameIndex();
  evt.t.SetSec( ts.getTimesliceStart().getUTCseconds() );
  evt.t.SetNanoSec( ts.getTimesliceStart().getUTC16nanosecondcycles() * 16 );
  evt.hits.clear();

  // a timeslice is a vector of JDAQSuperFrame's, which is a JDAQFrame, which
  // is an stl::vector-like object (supporting stl-like iteration.)

  Hit h; h.id = 0; h.pmt_id = 0;

  for (auto& sf : ts )
  {
    for (auto& daqhit : sf )
    {
      read( h, daqhit);
      h.dom_id = sf.getModuleID();
      evt.hits.push_back(h);
    }
  }
}


/**
 * Get summary slice from given file with given frame index. This function will
 * (re)build an index each time it encounters a new TFile as input.
 *
 * \param  f             pointer to ROOT file
 * \param  frame_index   frame index
 * \return               pointer to summary slice
 */

inline JDAQSummaryslice* get_summary_slice( TFile* f , int frame_index )
{
  static TFile* _f = 0;
  static TTree*  S = 0;
  static TBranch* BS = 0;
  JDAQSummaryslice *r = 0 ;

  if (!f) THROW(Exception, "get_summary_slice called with TFile pointer that is null");


  // in principle, event if the pointer-value is the same, we could have been given a new file

  if ( !_f || _f->GetUUID().Compare( f->GetUUID() ) != 0 ) // setup for tree reading and build tree index
  {
    _f = f;

    // first we have to deal with the following....
    // The streamer of JDAQSummaryFrame needs to know what to do since
    // this information is not written to root file.

    const char* name = JDAQSummaryslice::Class()->GetName();

    JDAQSummaryFrame::ROOT_IO_VERSION = ((TStreamerInfo*)_f -> GetStreamerInfoList()->FindObject(name))->GetClassVersion();

    S = (TTree*) _f->Get( TTREE_ONLINE_SUMMARYSLICE );

    if (!S)
    {
      THROW(Exception, "Failed to get summary slice TTree : " <<  TTREE_ONLINE_SUMMARYSLICE );
    }

    BS = S->GetBranch( TBRANCH_ONLINE_SUMMARYSLICE );

    if (!BS)
    {
      THROW(Exception, "Failed to get brach :" << TBRANCH_ONLINE_SUMMARYSLICE );
    }

    std::cout << "building index to lookup summary slices..." << std::endl;
    int n = S->BuildIndex("frame_index");
    (void) n;
    BS->SetAddress( &r );
  }

  int nbytes = S->GetEntryWithIndex( frame_index ); // returns -1 if not found
  if ( nbytes <= 0 )
  {
    THROW(Exception, "Failed to find summary slice entry with frame_index " << frame_index);
  }
  return r;
} 


#endif
