#ifndef __TOOLS_MULTIPLICITY__
#define __TOOLS_MULTIPLICITY__

#include <sstream>

#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/offline/Trk.hh"


/**
 * Retrieve bundle multiplicity of given event.
 *
 * \param  event              event
 */
inline int get_multiplicity(const Evt& event)
{
  using namespace std;

  // Bundle multiplicity is stored in the `len` member variable for `track_bundle`s.
    
  for ( auto& t : event.mc_trks ) {
    if ( t.status == TRK_ST_MUONBUNDLE ) { return (int) t.len; } 
  }
    
  ostringstream out;
  out << "get_multiplicity(): The following event does not correspond to a muon bundle:" << endl;
  event.print(out);
  throw Exception(static_cast<ostringstream&>(out).str());
}

#endif
