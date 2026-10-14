#include "km3net-dataformat/online/JDAQEvent.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  /**
   * Get triggered hits.
   *
   * \return                triggered hits
   */
  template<>
  const std::vector<JDAQTriggeredHit>& JDAQEvent::getHits<JDAQTriggeredHit>() const
  {
    return triggeredHits;
  }


  /**
   * Get snapshot hits.
   *
   * \return                snapshot hits
   */
  template<>
  const std::vector<JDAQKeyHit>& JDAQEvent::getHits<JDAQSnapshotHit>() const
  {
    return snapshotHits;
  }


  /**
   * Get triggered hits.
   *
   * \return                triggered hits
   */
  template<>
  std::vector<JDAQTriggeredHit>& JDAQEvent::getHits<JDAQTriggeredHit>()
  {
    return triggeredHits;
  }


  /**
   * Get snapshot hits.
   *
   * \return                snapshot hits
   */
  template<>
  std::vector<JDAQKeyHit>& JDAQEvent::getHits<JDAQSnapshotHit>()
  {
    return snapshotHits;
  }


  /**
   * Get begin of triggered hits.
   *
   * \return                begin triggered hits
   */
  template<>
  JDAQEvent::const_iterator<JDAQTriggeredHit> JDAQEvent::begin<JDAQTriggeredHit>() const
  {
    return triggeredHits.begin();
  }


  /**
   * Get end of triggered hits.
   *
   * \return                end triggered hits
   */
  template<>
  JDAQEvent::const_iterator<JDAQTriggeredHit> JDAQEvent::end<JDAQTriggeredHit>() const
  {
    return triggeredHits.end();
  }


  /**
   * Get begin of snapshot hits.
   *
   * \return                begin snapshot hits
   */
  template<>
  JDAQEvent::const_iterator<JDAQSnapshotHit> JDAQEvent::begin<JDAQKeyHit>() const
  {
    return snapshotHits.begin();
  }


  /**
   * Get end of snapshot hits.
   *
   * \return                end snapshot hits
   */
  template<>
  JDAQEvent::const_iterator<JDAQSnapshotHit> JDAQEvent::end<JDAQKeyHit>() const
  {
    return snapshotHits.end();
  }


  /**
   * Get reverse begin of triggered hits.
   *
   * \return                begin triggered hits
   */
  template<>
  JDAQEvent::const_reverse_iterator<JDAQTriggeredHit> JDAQEvent::rbegin<JDAQTriggeredHit>() const
  {
    return triggeredHits.rbegin();
  }


  /**
   * Get reverse end of triggered hits.
   *
   * \return                end triggered hits
   */
  template<>
  JDAQEvent::const_reverse_iterator<JDAQTriggeredHit> JDAQEvent::rend<JDAQTriggeredHit>() const
  {
    return triggeredHits.rend();
  }


  /**
   * Get reverse begin of snapshot hits.
   *
   * \return                begin snapshot hits
   */
  template<>
  JDAQEvent::const_reverse_iterator<JDAQSnapshotHit> JDAQEvent::rbegin<JDAQKeyHit>() const
  {
    return snapshotHits.rbegin();
  }


  /**
   * Get reverse end of snapshot hits.
   *
   * \return                end snapshot hits
   */
  template<>
  JDAQEvent::const_reverse_iterator<JDAQSnapshotHit> JDAQEvent::rend<JDAQKeyHit>() const
  {
    return snapshotHits.rend();
  }


  /**
   * Get number of triggered hits.
   *
   * \return                number of triggered hits
   */
  template<>
  unsigned int JDAQEvent::size<JDAQTriggeredHit>() const
  {
    return triggeredHits.size();
  }


  /**
   * Get number of snapshot hits.
   *
   * \return                number of snapshot hits
   */
  template<>
  unsigned int JDAQEvent::size<JDAQSnapshotHit>() const
  {
    return snapshotHits.size();
  }


  /**
   * Check emptyness of triggered data.
   *
   * \return                emptyness triggered hits
   */
  template<>
  bool JDAQEvent::empty<JDAQTriggeredHit>() const
  {
    return triggeredHits.empty();
  }


  /**
   * Check emptyness of snapshot data.
   *
   * \return                emptyness snapshot hits
   */
  template<>
  bool JDAQEvent::empty<JDAQSnapshotHit>() const
  {
    return snapshotHits.empty();
  }

  
  /**
   * Equal operator for DAQ events.
   *
   * \param  first     event
   * \param  second    event
   * \result           true if first event equal to second; else false
   */
  bool operator==(const JDAQEvent& first,
		  const JDAQEvent& second)
  {
    return (first.getDAQEventHeader()         == second.getDAQEventHeader()          &&
	    first.getHits<JDAQTriggeredHit>() == second.getHits<JDAQTriggeredHit>()  &&
	    first.getHits<JDAQSnapshotHit> () == second.getHits<JDAQSnapshotHit> ());
  }
  

  /**
   * Print DAQ Event.
   *
   * \param  out         output stream
   * \param  lpr         long print
   * \return             output stream
   */
  std::ostream& JDAQEvent::print(std::ostream& out, const bool lpr) const
  {
    using namespace std;

    out << this->ClassName() << endl;
    out << dynamic_cast<const JDAQPreamble&>   (*this) << endl;
    out << dynamic_cast<const JDAQChronometer&>(*this) << endl;

    out << "Trigger counter    " <<        this->getCounter()                    << endl;
    out << "Trigger mask (hex) " << hex << this->getTriggerMask()         << dec << endl;
    out << "Overlays           " <<        this->getOverlays()                   << endl;
    out << "Triggered hits     " <<        this->size<JDAQTriggeredHit>()        << endl;
    out << "Snapshot  hits     " <<        this->size<JDAQSnapshotHit> ()        << endl;
    
    {
      typedef JDAQTriggeredHit  JHit_t;
    
      for (JDAQEvent::const_iterator<JHit_t> hit = this->begin<JHit_t>(); hit != this->end<JHit_t>(); ++hit) {
	out << setw(5)  <<        hit->getModuleID()    << ' '
	    << setw(2)  <<  (int) hit->getPMT()         << ' '
	    << setw(10) <<  (int) hit->getT()           << ' '
	    << setw(3)  <<  (int) hit->getToT()         << ' '
	    << setw(10) << hex << hit->getTriggerMask() << dec << endl;
      }
    }

    if (lpr) {

      typedef JDAQSnapshotHit  JHit_t;
      
      for (JDAQEvent::const_iterator<JHit_t> hit = this->begin<JHit_t>(); hit != this->end<JHit_t>(); ++hit) {
	out << setw(5)  <<        hit->getModuleID()    << ' '
	    << setw(2)  <<  (int) hit->getPMT()         << ' '
	    << setw(10) <<  (int) hit->getT()           << ' '
	    << setw(3)  <<  (int) hit->getToT()         << endl;
      }
    }

    return out;
  }


  /**
   * Get time difference between two events.
   *
   * \param  first       event
   * \param  second      event
   * \result             time difference [s]
   */
  inline double getTimeDifference(const JDAQEvent& first, const JDAQEvent& second)
  {
    double dt = getTimeDifference(first.getTimesliceStart(), second.getTimesliceStart());

    if (!first.empty<JDAQTriggeredHit>() && !second.empty<JDAQTriggeredHit>()) {
      dt += (second.begin<JDAQTriggeredHit>()->getT() -
             first .begin<JDAQTriggeredHit>()->getT()) * 1.0e-9;
    }

    return dt;
  }
}
