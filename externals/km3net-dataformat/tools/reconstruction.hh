#ifndef __TOOLS_RECONSTRUCTION__
#define __TOOLS_RECONSTRUCTION__

#include <limits>
#include <algorithm>

#include "km3net-dataformat/offline/Hit.hh"
#include "km3net-dataformat/offline/Vec.hh"
#include "km3net-dataformat/offline/Trk.hh"
#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/offline/Exception.hh"
#include "km3net-dataformat/definitions/reconstruction.hh"


/**
 * \file
 *
 * Auxiliary methods for selection of reconstructed tracks.
 * \author mdejong
 */


/**
 * Range of reconstruction stages.
 */
struct rec_stages_range {
  /**
   * Defaut constructor.
   */
  rec_stages_range() :
    lower(0),
    upper(std::numeric_limits<int>::max())
  {}


  /**
   * Constructor.
   *
   * \param  lower          lower reconstruction stage
   * \param  upper          upper reconstruction stage
   */
  rec_stages_range(const int lower, const int upper) :
    lower(lower),
    upper(upper)
  {}


  /**
   * Constructor.
   *
   * \param  stage          reconstruction stage
   */
  rec_stages_range(const int stage) :
    lower(stage),
    upper(stage)
  {}

  
  /**
   * Test if given reconstruction stage is within range.
   *
   * \param  stage          reconstruction stage
   * \return                true if within range; else false
   */
  inline bool operator()(const int stage) const
  {
    return (stage >= lower && stage <= upper);
  }

  int lower;
  int upper;
};


/**
 * Reconstruction type dependent comparison of track quality.
 *
 * The specialisation of this class should implement the function object operator
 * <pre>
 *    inline bool operator()(const Trk& first, const Trk& second) const
 * </pre>
 * and return true if the first track is better then the second track.
 */
template<int reconstruction_type>
struct quality_sorter {
  /**
   * The default comparison is based on:
   *    -# number of reconstruction stages, i.e. <tt>Trk::rec_stages.size()</tt> (the larger the better);
   *    -# likelihood of the final track, i.e. <tt>Trk::lik</tt> (the larger the better).
   *
   * \param  first          first  track
   * \param  second         second track
   * \return                true if first track has better quality than second; else false
   */
  inline bool operator()(const Trk& first, const Trk& second) const
  {
    if (first.rec_stages.size() == second.rec_stages.size())
      return first.lik > second.lik;
    else
      return first.rec_stages.size() > second.rec_stages.size();
  }
};


/**
 * Auxiliary class to test whether given track has specified history.\n
 * The history of a track consists of a reconstruction type and a range of application types.
 */
struct has_history {
  /**
   * Constructor.
   *
   * \param  type           reconstruction type
   * \param  range          range of application types
   */
  has_history(const int type,  const rec_stages_range range) :
    type (type),
    range(range)
  {}

  /**
   * \param  track          track
   * \return                true if given track has specified history; else false
   */
  inline bool operator()(const Trk& track) const
  {
    if (track.rec_type == type)
      return std::find_if(track.rec_stages.begin(), track.rec_stages.end(), range) != track.rec_stages.end();
    else
      return false;
  }

  const int              type;    //!< reconstruction type
  const rec_stages_range range;   //!< range of application types
};
  

/**
 * Test whether given track has muon prefit in history.
 *
 * \param  track          track
 * \return                true if muon prefit in history; else false
 */
inline bool has_jppmuon_prefit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JMUONPREFIT)(track);
}


/**
 * Test whether given track has muon simplex fit in history.
 *
 * \param  track          track
 * \return                true if muon simplex fit in history; else false
 */
inline bool has_jppmuon_simplex(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JMUONSIMPLEX)(track);
}


/**
 * Test whether given track has muon gandalf fit in history.
 *
 * \param  track          track
 * \return                true if muon gandalf fit in history; else false
 */
inline bool has_jppmuon_gandalf(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JMUONGANDALF)(track);
}


/**
 * Test whether given track has muon energy fit in history.
 *
 * \param  track          track
 * \return                true if muon energy fit in history; else false
 */
inline bool has_jppmuon_energy(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JMUONENERGY)(track);
}


/**
 * Test whether given track has muon start fit in history.
 *
 * \param  track          track
 * \return                true if muon start fit in history; else false
 */
inline bool has_jppmuon_start(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JMUONSTART)(track);
}


/**
 * Test whether given track has default muon fit in history.
 *
 * \param  track          track
 * \return                true if muon fit in history; else false
 */
inline bool has_jppmuon_fit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, rec_stages_range(JMUONBEGIN, JMUONEND))(track);
}  


/**
 * Test whether given track has shower prefit in history.
 *
 * \param  track          track
 * \return                true if shower prefit in history; else false
 */
inline bool has_shower_prefit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JSHOWERPREFIT)(track);
}


/**
 * Test whether given track has shower position fit in history.
 *
 * \param  track          track
 * \return                true if shower position fit in history; else false
 */
inline bool has_shower_positionfit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JSHOWERPOSITIONFIT)(track);
}  


/**
 * Test whether given track has shower complete fit in history.
 *
 * \param  track          track
 * \return                true if shower complete fit in history; else false
 */
inline bool has_shower_completefit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, JSHOWERCOMPLETEFIT)(track);
}


/**
 * Test whether given track has default shower fit in history.
 *
 * \param  track          track
 * \return                true if shower fit in history; else false
 */
inline bool has_shower_fit(const Trk& track)
{
  return ::has_history(JPP_RECONSTRUCTION_TYPE, rec_stages_range(JSHOWERBEGIN, JSHOWEREND))(track);
}


/**
 * Test whether given track has default shower fit in history.
 *
 * \param  track          track
 * \return                true if shower fit in history; else false
 */
inline bool has_aashower_fit(const Trk& track)
{
  return ::has_history(AANET_RECONSTRUCTION_TYPE, rec_stages_range(AASHOWERBEGIN, AASHOWEREND))(track);
}


/**
 * Test whether given event has a track according selection.\n
 * The track selector corresponds to the function operator <tt>bool selector(const Trk&);</tt>.
 *
 * \param  evt            event
 * \param  selector       track selector
 * \return                true if at least one corresponding track; else false
 */
template<class JTrackSelector_t>
inline bool has_reconstructed_track(const Evt& evt, JTrackSelector_t selector)
{
  return std::find_if(evt.trks.begin(), evt.trks.end(), selector) != evt.trks.end();
}


/**
 * Test whether given event has a track according selection.
 *
 * \param  evt            event
 * \param  range          range of application types
 * \return                true if at least one corresponding track; else false
 */
template<int reconstruction_type>
inline bool has_reconstructed_track(const Evt& evt, const rec_stages_range range = rec_stages_range())
{
  return ::has_reconstructed_track(evt, ::has_history(reconstruction_type, range));
}
  

/**
 * Test whether given event has a track with muon reconstruction.
 *
 * \param  evt            event
 * \return                true if at least one reconstructed muon; else false
 */
inline bool has_reconstructed_jppmuon(const Evt& evt)
{
  return ::has_reconstructed_track(evt, has_jppmuon_fit);
}
  

/**
 * Test whether given event has a track with shower reconstruction.
 *
 * \param  evt            event
 * \return                true if at least one reconstructed shower; else false
 */
inline bool has_reconstructed_jppshower(const Evt& evt)
{
  return ::has_reconstructed_track(evt, ::has_shower_fit);
}
  

/**
 * Test whether given event has a track with aashower reconstruction.
 *
 * \param  evt            event
 * \return                true if at least one reconstructed shower; else false
 */
inline bool has_reconstructed_aashower(const Evt& evt)
{
  return ::has_reconstructed_track(evt, has_aashower_fit);
}


/**
 * Get best reconstructed track.\n
 * The track selector corresponds to the function operator <tt>bool selector(const Trk&);</tt> and
 * the track comparator to <tt>bool comparator(const Trk&, const Trk&);</tt>.\n
 * This method throws an exception in case no track is present.\n
 * The method <tt>has_reconstructed_track</tt> can be used to avoid this exception.
 *
 * \param  evt            event
 * \param  selector       track selector
 * \param  comparator     track comparator
 * \return                track
 */
template<class JTrackSelector_t, class JQualitySorter_t>
inline const Trk& get_best_reconstructed_track(const Evt&       evt,
					       JTrackSelector_t selector,
					       JQualitySorter_t comparator)
{
  std::vector<Trk>::const_iterator p = std::find_if(evt.trks.begin(), evt.trks.end(), selector);

  for (std::vector<Trk>::const_iterator i = p; i != evt.trks.end(); ++i) {
    if (selector(*i) && comparator(*i, *p)) {
      p = i;
    }
  }

  if (p != evt.trks.end())
    return *p;
  else
    THROW(Exception, "This event has no reconstructed track with given selector.");
}


/**
 * Get best reconstructed track.
 *
 * \param  evt            event
 * \param  range          range of application types
 * \return                track
 */
template<int reconstruction_type>
inline const Trk& get_best_reconstructed_track(const Evt& evt, const rec_stages_range range = rec_stages_range())
{
  return get_best_reconstructed_track(evt, ::has_history(reconstruction_type, range), quality_sorter<reconstruction_type>());
}


/**
 * Get best reconstructed muon.
 *
 * \param  evt            event
 * \return                track
 */
inline const Trk& get_best_reconstructed_jppmuon(const Evt& evt)
{
  return get_best_reconstructed_track(evt, has_jppmuon_fit, quality_sorter<JPP_RECONSTRUCTION_TYPE>());
}


/**
 * Get best reconstructed shower.
 *
 * \param  evt            event
 * \return                track
 */
inline const Trk& get_best_reconstructed_jppshower(const Evt& evt)
{
  return get_best_reconstructed_track(evt, ::has_shower_fit, quality_sorter<JPP_RECONSTRUCTION_TYPE>());
}


/**
 * Get best reconstructed aashower.
 *
 * \param  evt            event
 * \return                track
 */
inline const Trk& get_best_reconstructed_aashower(const Evt& evt)
{
  return get_best_reconstructed_track(evt, ::has_aashower_fit, quality_sorter<AANET_RECONSTRUCTION_TYPE>());
}

#endif
