#ifndef TRK_HH_INCLUDED
#define TRK_HH_INCLUDED

#include <vector>
#include "TDatabasePDG.h"
#include "TPDGCode.h"
#include "km3net-dataformat/offline/AAObject.hh"
#include "km3net-dataformat/offline/Vec.hh"
#include "km3net-dataformat/definitions/trkmembers.hh"

/** 
 * The Trk class represents a Monte Carlo (MC) particle as well as a reconstructed track/shower. 
 */
struct Trk: public AAObject
{
  int    id;                          ///< track identifier
  Vec    pos;                         ///< postion [m] of the track at time t 
  Vec    dir;                         ///< track direction
  double t;                           ///< track time [ns] (when the particle is at pos )
  double E;                           ///< Energy [GeV] (either MC truth or reconstructed)

  double len;                         ///< length, if applicable [m]
  double lik;                         ///< likelihood or lambda value (for aafit, lambda)
  int    type;                        ///< MC: particle type in PDG encoding
  int               rec_type;         ///< identifier of the fitting algorithm/chain/strategy, see km3net-dataformat/definitions/reconstruction.csv
  std::vector<int>  rec_stages;       ///< list of identifiers of successful fitting stages resulting in this track

  int status;                         ///< MC or reconstruction status code, see km3net-dataformat/definitions/trkmembers.csv for values
  int mother_id;                      ///< id of the parent MC particle or of the reconstructed track at the previous stage
  int counter;                        ///< used by CORSIKA7 MC generation to store interaction counters, see <a href="https://web.iap.kit.edu/corsika/usersguide/usersguide.pdf">CORSIKA user guide</a>

  std::vector<double> fitinf;         ///< place to store additional fit info, see km3net-dataformat/definitions/fitparameters.csv
  std::vector<int>    hit_ids;        ///< list of associated hit-ids (corresponds to Hit::id).
  std::vector<double> error_matrix;   ///< (NxN) error covariance matrix for fit parameters (stored as linear vector)
  std::string    comment;             ///< use as you like

  /**
   * Default constructor.
   */
  Trk(): id(0),t(0),E(0),len(0),lik(0), type(0), rec_type(0), status(TRK_ST_UNDEFINED), mother_id(TRK_MOTHER_UNDEFINED), counter(0) {}




  /**
   * Read track (useful in python).
   *
   * \param  t             track
   */
  void read(const Trk& t) { *this = t;}

  /**
   * Write track (useful in python).
   *
   * \param  t             track
   */
  void write(Trk& t)      const { t = *this; }
 

  /**
   * Get the name of the MC particle type.
   *
   * \return               name
   */
  std::string name() const
  {
    TParticlePDG* p = TDatabasePDG::Instance()->GetParticle( type );
    if (!p) return "unnamed state ("+ std::to_string(type)+")";
    return p->GetName();
  }

  /**
   * Check if this is a primary particle.
   *
   * \return               true if primary; else false
   */  
  bool is_primary() const
  {
    return status==TRK_ST_PRIMARYNEUTRINO || status==TRK_ST_PRIMARYCOSMIC;
  }
  
  /**
   * Test whether given particle is a final state inside the detector.
   *
   * \return                true if particle is final state; else false
   */
  bool is_finalstate() const
  {
    return status==TRK_ST_FINALSTATE;
  }

  /**
   * Check if this is a netrino.
   *
   * Note that its is checked if the PDG type is a nu-e, nu-mu or nu-tau.
   *
   * \return               true if neutrino; else false
   */  
  bool is_neutrino() const
  {
    return type == kNuE || type == kNuEBar || type == kNuMu || type == kNuMuBar || type == kNuTau || type == kNuTauBar;
  }

  /**
   * Check if this is an electron or positron.
   *
   * \return               true if this is an electron or positron
   */
  bool is_e() const
  {
     return type == kElectron || type == kPositron;
  }

  /**
   * Check if this is a muon.
   *
   * Note that its is checked if the PDG type is a (anti-)muon.
   *
   * \return               true if muon; else false
   */
  bool is_muon() const 
  {
    return type == kMuonMinus || type == kMuonPlus;
  }

  /**
   * Check if this is a tau.
   *
   * Note that its is checked if the PDG type is a (anti-)tau.
   *
   * \return               true if tau; else false
   */
  bool is_tau() const
  {
    return type == kTauMinus || type == kTauPlus;
  }

  /**
   * Check if this is a charged lepton.
   *
   * Note that its is checked if the PDG type is a (anti-)electron, (anti-)muon or (anti-)tua.
   *
   * \return               true if charged lepton; else false
   */
  bool is_lepton() const 
  {
    return is_e() || is_muon() || is_tau();
  }

  /**
   * Check if this is an orphan (i.e. no mother).
   *
   * \return               true if orphan; else false
   */  
  bool is_orphan() const
  {
    return mother_id == TRK_MOTHER_NONE;
  }
  
  /**
   * Get list of of pointers to tracks, all of which have their mother identifier set to identifier of this track.
   *
   * \param  mctrks        list of input tracks
   * \return               list of pointers to tracks
   */
  std::vector< Trk* > get_daughters ( std::vector<Trk>& mctrks )
  {
    std::vector<Trk*> r;

    for( auto& t : mctrks )
      {
	     if ( t.mother_id == id ) r.push_back( &t );
      }
    return r;
  }

  /**
   * Print track.
   *
   * \param  out           output stream
   */
  void print(std::ostream& out=std::cout) const
  {
    out << "Trk: id=" << id << " pos="; pos.print(out);
    out << " dir="; dir.print(out);
    out << " t=" << t << " E=" << E << " pdg-type=" << type;
  }

  ClassDef(Trk,12)
};

#endif
