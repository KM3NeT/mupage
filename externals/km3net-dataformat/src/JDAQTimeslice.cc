#include <map>
#include <vector>
#include <algorithm>

#include "km3net-dataformat/online/JDAQEvent.hh"
#include "km3net-dataformat/online/JDAQSummaryslice.hh"
#include "km3net-dataformat/online/JDAQTimeslice.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {
  
  /**
   * Constructor.
   *
   * \param  event          DAQ event
   * \param  snapshot       use shapshot hits (else use triggered hits)
   */
  JDAQTimeslice::JDAQTimeslice(const JDAQEvent& event,
			       const bool       snapshot) :
    JDAQPreamble(JDAQType<JDAQTimeslice>()),
    JDAQTimesliceHeader(event.getDAQChronometer()),
    std::vector<JDAQSuperFrame>()
  {
    using namespace std;

    map<int, vector<JDAQHit> > buffer;
  
    if (snapshot) {
    
      for (vector<JDAQSnapshotHit>::const_iterator  hit = event.begin<JDAQSnapshotHit> (); hit != event.end<JDAQSnapshotHit> (); ++hit) {
	buffer[hit->getModuleID()].push_back(*hit);
      }
    
    } else {

      for (vector<JDAQTriggeredHit>::const_iterator hit = event.begin<JDAQTriggeredHit>(); hit != event.end<JDAQTriggeredHit>(); ++hit) {
	buffer[hit->getModuleID()].push_back(*hit);
      }
    }
  
  
    for (map<int, vector<JDAQHit> >::iterator entry = buffer.begin(); entry != buffer.end(); ++entry) {
    
      sort(entry->second.begin(), entry->second.end());
      
      this->push_back(JDAQSuperFrame(JDAQSuperFrameHeader(getDAQChronometer(), entry->first)));
      
      this->rbegin()->add(entry->second.size(), entry->second.data());
    }
  }


  /**
   * Constructor.
   *
   * \param  event          DAQ event
   * \param  summary        summary
   * \param  snapshot       use shapshot hits (else use triggered hits)
   */
  JDAQTimeslice::JDAQTimeslice(const JDAQEvent&        event,
			       const JDAQSummaryslice& summary,
			       const bool              snapshot) :
    JDAQPreamble(JDAQType<JDAQTimeslice>()),
    JDAQTimesliceHeader(event.getDAQChronometer()),
    std::vector<JDAQSuperFrame>()
  {
    using namespace std;
    using namespace JPP;

    map<int, vector<JDAQHit> > buffer;
  
    if (snapshot) {
    
      for (vector<JDAQSnapshotHit>::const_iterator  hit = event.begin<JDAQSnapshotHit> (); hit != event.end<JDAQSnapshotHit> (); ++hit) {
	buffer[hit->getModuleID()].push_back(*hit);
      }
    
    } else {
    
      for (vector<JDAQTriggeredHit>::const_iterator hit = event.begin<JDAQTriggeredHit>(); hit != event.end<JDAQTriggeredHit>(); ++hit) {
	buffer[hit->getModuleID()].push_back(*hit);
      }
    }

    map<int, JDAQFrameStatus> sumo;
    
    for (JDAQSummaryslice::const_iterator i = summary.begin(); i != summary.end(); ++i) {
      sumo[i->getModuleID()] = i->getDAQFrameStatus();
    }

    for (map<int, vector<JDAQHit> >::iterator entry = buffer.begin(); entry != buffer.end(); ++entry) {

      sort(entry->second.begin(), entry->second.end());

      JDAQFrameStatus status = JDAQFrameStatus::getInstance();

      map<int, JDAQFrameStatus>::const_iterator ps = sumo.find(entry->first);

      if (ps != sumo.end()) {
	status = ps->second;
      }
	
      this->push_back(JDAQSuperFrame(JDAQSuperFrameHeader(getDAQChronometer(), entry->first, status)));

      this->rbegin()->add(entry->second.size(), entry->second.data());
    }
  }
}
