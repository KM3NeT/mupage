#ifndef IO_ASCII_INCLUDED
#define IO_ASCII_INCLUDED

#include <string>
#include <istream>
#include <ostream>
#include <sstream>

#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/offline/Hit.hh"
#include "km3net-dataformat/offline/Trk.hh"
#include "km3net-dataformat/offline/Head.hh"
#include "km3net-dataformat/offline/Exception.hh"
#include "km3net-dataformat/definitions/w2list_gseagen.hh" 
#include "km3net-dataformat/definitions/trkmembers.hh"

#include "TDatabasePDG.h"

namespace mc_keys
{
  const char* const  auto_t          =  "auto";
  const char* const  start_run_t     =  "start_run:";
  const char* const  start_event_t   =  "start_event:";
  const char* const  hit_t           =  "hit:";
  const char* const  hit_raw_t       =  "hit_raw:";
  const char* const  track_in_t      =  "track_in:";
  const char* const  track_t         =  "track:";
  const char* const  track_fit_t     =  "track_fit:";
  const char* const  neutrino_t      =  "neutrino:";
  const char* const  track_primary_t =  "track_primary:";
  const char* const  track_bundle_t  =  "track_bundle:";
  //const char* const  primarylepton_t =  "primarylepton:";
  const char* const  weights_t       =  "weights:";
  const char* const  w2list_t        =  "w2list:";
  const char* const  w3list_t        =  "w3list:";
  const char* const  hourangle_t     =  "hourangle:";
  const char* const  eventtime_t     =  "eventtime:";
  const char* const  center_on_can_t =  "center_on_can:";
  const char* const  muon_decay_t    =  "muon_decay:";
  const char* const  end_event_t     =  "end_event:";
}

namespace mc_usr_keys
{
  // track-level quantities
  const char* const energy_lost_in_can = "energy_lost_in_can";


  // event-level corsika variables
  const char* const muon_decay_x = "muon_decay_x";
  const char* const muon_decay_y = "muon_decay_y";
  const char* const muon_decay_z = "muon_decay_z";

  const char* const center_on_can_x = "center_on_can_x";
  const char* const center_on_can_y = "center_on_can_y";
  const char* const center_on_can_z = "center_on_can_z";
  const char* const hourangle = "hourangle";
}


namespace io_stringutil
{
  /**
   * Check if string starts with given text.
   *
   * \param  a             input string
   * \param  b             text
   * \return               true if string start with text; else false
   */
  inline bool startswith( const std::string& a, const std::string& b )
  {
    if ( a.find( b ) == 0 ) return true;
    return false;
  }

  /**
   * Remove leading and trailing white spaces.
   *
   * \param  s             input string
   * \return               trimmed string
   */
  inline std::string trim(const std::string& s)
  {
    using namespace std;

    if ( s == "" ) return s;

    string::size_type i1;
    string::size_type i2;

    for (i1 = 0; i1 < s.length(); i1++)
      {
        if ( !isspace (s[i1]) ) break;
      }
    for (i2 = s.length() - 1 ; i2 > i1 ; i2--)
      {
        if ( !isspace (s[i2]) ) break;
      }
    return s.substr( i1, i2 - i1 + 1 );
  }

}


/**
 * Convert Geant3 to PDG particle type.
 *
 * \param  geant3_code   Geant3 code
 * \return               PDG code
 */
inline int pdg_code( int geant3_code )
{
  if ( geant3_code == -1 ) return -1; // used for k40 hits
  if ( geant3_code < 0 ) return pdg_code( -geant3_code ); // used for scattered

  return TDatabasePDG::Instance()->ConvertGeant3ToPdg( geant3_code );
}

/**
 * Convert PDG to Geant3 particle type.
 *
 * \param  pdg_code      PDG code
 * \return               Geant3 code
 */
inline int geant3_code( int pdg_code )
{
  if (pdg_code == -1 )   return -1;
  if (pdg_code == +311 ) return geant3_code( 130 ); // K0 -> K0long
  if (pdg_code == -311 ) return geant3_code( 130 ); // K0bar -> K0long

  return TDatabasePDG::Instance()->ConvertPdgToGeant3( pdg_code );
}


/**
 * Read a Vec(tor) from a stream.
 *
 * \param  v             vector
 * \param  is            input stream
 * \return               true if correctly read; else false
 */
inline bool read ( Vec& v, std::istream& is)
{
  is >> v.x >> v.y >> v.z;
  return !is.fail();
}

/**
 * Write a Vec(tor) to a stream.
 *
 * \param  v             vector
 * \param  os            output stream
 * \return               true if correctly written; else false
 */
inline bool write( const Vec& v, std::ostream& os)
{
  os << v.x << ' ' << v.y << ' ' << v.z;
  return !os.fail();
}

/**
 * Read a hit from a stream.
 *
 * \param  h             hit
 * \param  is            input stream
 * \param  read_mc       option to read also type and origin
 * \return               true if correctly read; else false
 */
inline bool read ( Hit& h, std::istream& is, bool read_mc = false  )
{
  h.dom_id     = 0; // need a proper det file to
  h.channel_id = 0; // set these.

  is >> h.id >> h.pmt_id >> h.a >> h.t;
  if ( !read_mc )
    {
      return !is.fail();
    }
  else
    {
      is >> h.type >> h.origin;
    }

  // at this point, an additional pure_a and pure_t may be present,
  // but we do not read them.

  return !is.fail();
}


/**
 * Write a hit to a stream.
 *
 * \param  h             hit
 * \param  os            output stream
 * \param  tag           tag
 * \return               true if correctly written; else false
 */
inline bool write( const Hit& h, std::ostream& os, const std::string& tag = mc_keys::hit_t)
{
  int om_id = h.pmt_id; // todo: deal with this better.

  os << tag << ' ' << h.id << ' ' << om_id << ' ' << h.a << ' ' << h.t;
  if ( tag != mc_keys::hit_raw_t ) {
    os << ' ' << h.type  << ' ' << h.origin; // not writing pure_a and pure_t
  }
  os << std::endl;
  return !os.fail();
}


/**
 * Read data.
 *
 * \param  is            input stream
 * \return               data
 */
inline std::vector<double> read_line_to_vector( std::istream& is )
{
  using namespace std;

  vector<double> r;

  string ss;
  getline(is, ss);
  istringstream il(ss);
  for ( double x; il >> x ; ) r.push_back( x );

  return r;
}


/**
 * Put value in front of data.
 *
 * \param  vec           data
 * \param  value         value
 */
template<typename T>
inline void push_front( std::vector<T>& vec, const T& value )
{
  vec.insert( vec.begin(), value );
}


/**
 * Read event from a stream.
 *
 * \param  evt           event
 * \param  is            input stream
 * \param  skip_hits     option to skip reading of hits
 * \return               true if correctly read; else false
 */
inline bool read ( Evt& evt, std::istream& is, bool skip_hits = false )
{
  using namespace std;

  string w;

  // find next start_event

  while ( w != mc_keys::start_event_t && is.good() ) is >> w;

  int mc_event_type;   // dummy - this is always 1 in all files.
  is >> evt.mc_id >> mc_event_type;

  Trk trk_nu, trk_primary;
  bool have_trk_nu(false), have_trk_primary(false);


  evt.mc_trks.clear();
  evt.hits.clear();
  evt.mc_hits.clear();

  string w_old;

  while ( is )
    {
      static Hit h;
      static Trk t;

      is >> w;

      if (skip_hits && ( w == mc_keys::hit_t || w == mc_keys::hit_raw_t)) {
        is.ignore( 1000, '\n' );
        continue;
      }

      if ( w == mc_keys::hit_t ) {
      
        read( h, is, true );
        evt.mc_hits.push_back( h );
      
      } else if ( w == mc_keys::hit_raw_t ) {
      
        read( h, is, false);
        evt.hits.push_back( h );
      
      } else if ( w == mc_keys::track_in_t     ||
                  w == mc_keys::track_t        ||
                  w == mc_keys::neutrino_t     ||
                  w == mc_keys::track_bundle_t ||
                  w == mc_keys::track_primary_t ) {

        t.id  = 0;
        t.len = 0.0;
        t.clearusr();
        t.comment = w;
      
        string line;
        getline( is, line );
        istringstream ii(line);

        if ( w != mc_keys::track_bundle_t ) {
          ii >> t.id;
        } else {
          ii >> t.len;
        }
      
        ii >> t.pos >> t.dir >> t.E;

        if (!ii.fail()) {

          if ( w == mc_keys::track_in_t) {

            t.status = TRK_ST_FINALSTATE;
	
            ii >> t.t >> t.type;

            if (!ii.fail()) {
	
              t.type = pdg_code( t.type );
	
              ii >> t.len;
	
              if ( ii.fail() ) { // missing length is not an error
                evt.mc_trks.push_back( t );
                continue;
              } 

              double eloss = 0;
              ii >> eloss;
	
              if ( ii.fail() ) { // missing eloss is not an error
                evt.mc_trks.push_back( t );
                continue;
              } 

              t.setusr( mc_usr_keys::energy_lost_in_can, eloss);

              evt.mc_trks.push_back( t );
            }
	  
          } else if ( w == mc_keys::track_t ) {

            ii >> t.t;
	
            evt.trks.push_back( t );
	  
          } else if ( w == mc_keys::neutrino_t ) {

            t.status = TRK_ST_PRIMARYNEUTRINO;
	
            // the last item we will read is W2LIST_GSEAGEN_CC, make enough space;
            if (evt.w2list.size() < W2LIST_GSEAGEN_CC+1 ) evt.w2list.resize(W2LIST_GSEAGEN_CC+1);

            ii >> t.t >>
              evt.w2list[W2LIST_GSEAGEN_BX]    >> 
              evt.w2list[W2LIST_GSEAGEN_BY]    >> 
              evt.w2list[W2LIST_GSEAGEN_ICHAN] >> 
              t.type                           >> 
              evt.w2list[W2LIST_GSEAGEN_CC];

            trk_nu = t;
            have_trk_nu = true;
	  
          } else if ( w == mc_keys::track_primary_t ) {
	
            t.status = TRK_ST_PRIMARYCOSMIC;
	
            ii >> t.t >> trk_primary.type; // nucleus id (in pdg format or not?)

            trk_primary = t;
            have_trk_primary = true;
	  
          } else if ( w == mc_keys::track_bundle_t ) {

            t.type   = PDG_MUONBUNDLE;
            t.status = TRK_ST_MUONBUNDLE;

            evt.mc_trks.push_back( t );
	  
          } else {
	  
            ostringstream out;
            out << "Unknown tag " << w << " for trk ";
            t.print(out);
            throw Exception(static_cast<ostringstream&>(out).str());
          }
        }

        if ( ii.fail() ) {
          ostringstream out;
          out << "Error reading trk ";
          t.print(out);
          throw Exception(static_cast<ostringstream&>(out).str());
        }
      
      } else if ( w == mc_keys::weights_t) {
      
        evt.w = read_line_to_vector( is );
      
      } else if ( w == mc_keys::w2list_t) {
      
        auto v = read_line_to_vector( is );
        evt.w2list.resize( std::max( evt.w2list.size(), v.size()));
        std::copy( v.begin(), v.end() , evt.w2list.begin() );
      
      } else if ( w == mc_keys::w3list_t) {
      
        evt.w3list = read_line_to_vector( is );
      
      } else if ( w == mc_keys::hourangle_t ) {
      
        double ha;
        is >> ha;
        evt.setusr(mc_usr_keys::hourangle, ha );
      
      } else if ( w == mc_keys::center_on_can_t) {
      
        // in corsika files, there is the (undocumented?) center_on_can tag,
        // which denoets the projection of the primary on the can. The direction
        // of the center_on_can 'track' is by defintion the direction of the
        // primary. We record the position in the usr data.

        vector<double> v = read_line_to_vector( is );
      
        if ( v.size() > 3 ) {
          evt.setusr(mc_usr_keys::center_on_can_x, v[1] );
          evt.setusr(mc_usr_keys::center_on_can_y, v[2] );
          evt.setusr(mc_usr_keys::center_on_can_z, v[3] );
        }
      
      } else if ( w == mc_keys::eventtime_t ) {
      
        unsigned nsec, n16ns_ticks;
        is >> nsec >> n16ns_ticks;
        evt.mc_event_time.SetSec( nsec );
        evt.mc_event_time.SetNanoSec( n16ns_ticks * 16 );
      
      } else if ( w == mc_keys::muon_decay_t) {
      
        // in km3sim files, there are additional tags, including this one
        vector<double> v = read_line_to_vector( is );
        if ( v.size() > 4 )
          {
            evt.setusr(mc_usr_keys::muon_decay_x, v[2] );
            evt.setusr(mc_usr_keys::muon_decay_y, v[3] );
            evt.setusr(mc_usr_keys::muon_decay_z, v[4] );
          }
      
      } else if ( w == mc_keys::end_event_t) {
      
        // finalize the mc_tracks -- as best we can.

        // If there is both a primary, and a neutrino, then the primary
        // will go second (mc_trks[1]) with id=-1 and the neutrino will
        // be mc_trks[0] with id=0. Unless they are same particle (identical
        // pos,dir,E); in that case the primary is skipped.
        // The primarylepton tag is not stored as a seperate Trk.

        if ( have_trk_primary && have_trk_nu ) {
	
          bool same = trk_nu.pos == trk_primary.pos &&
            trk_nu.dir == trk_primary.dir &&
            trk_nu.E   == trk_primary.E;

          if (!same) {
            trk_primary.id = -1;
            push_front( evt.mc_trks, trk_primary);
          }

          trk_nu.id = 0;
          push_front( evt.mc_trks, trk_nu);
	
        } else if ( have_trk_primary  ) {
	
          trk_primary.id = 0;
          push_front( evt.mc_trks, trk_primary);
	
        } else if ( have_trk_nu ) {
	
          trk_nu.id = 0;
          push_front( evt.mc_trks, trk_nu);
        }

        return true;
      
      } else {
        is.ignore( 1000, '\n' );
      }
    
      w_old = w;
    }

  if (!is.eof()) {
    THROW(Exception, "Error while reading ascii event" << w << ' ' << evt.id);
  }

  return false;
}


/**
 * Write event to a stream.
 *
 * \param  evt           event
 * \param  os            output stream
 * \return               true if correctly read; else false
 */
inline bool write( const Evt& evt, std::ostream& os )
{
  using namespace std;

  // set precision to 12 digits.
  const int precision = 12;
  auto old_flags = os.flags();
  auto old_precision = os.precision( precision );
  os.unsetf( std::ios_base::scientific | std::ios_base::fixed ); // default behaviour

  os << mc_keys::start_event_t << ' ' << evt.mc_id << ' ' << 1 << endl;

  for ( auto& trk : evt.mc_trks ) {
    
    const std::string& tag = trk.comment;

    os <<  tag                                                << ' '
       << (tag != mc_keys::track_bundle_t ? trk.id : trk.len) << ' '
       << trk.pos                                             << ' '
       << trk.dir                                             << ' '
       << trk.E;
    
    if ( tag == mc_keys::track_in_t ) {
      
      os << ' ' << trk.t << ' ' << geant3_code(trk.type) << ' ' << trk.len;

      if ( trk.haveusr( mc_usr_keys::energy_lost_in_can ) ) {
        os << ' ' << trk.getusr( mc_usr_keys::energy_lost_in_can );
      }

      os << endl;
      
    } else if ( tag == mc_keys::track_primary_t ) {

      os << ' ' << trk.t << ' ' << trk.type << endl;
      
    } else if ( tag == mc_keys::neutrino_t ) {

      double bx(0), by(0);
      int ichan(0), cc(0);

      if ( evt.w2list.size() > W2LIST_GSEAGEN_CC ) {
        bx    = evt.w2list[W2LIST_GSEAGEN_BX];
        by    = evt.w2list[W2LIST_GSEAGEN_BY];
        ichan = evt.w2list[W2LIST_GSEAGEN_ICHAN];
        cc    = evt.w2list[W2LIST_GSEAGEN_CC];
      }

      os << ' ' << trk.t
         << ' ' << bx
         << ' ' << by
         << ' ' << ichan
         << ' ' << trk.type
         << ' ' << cc
         << endl;
      
    } else {

      os << endl;
    }
  }
  
  for ( auto& trk : evt.trks    ) {
    os << mc_keys::track_fit_t << ' ' << trk.id << ' ' << trk.pos << ' ' << trk.dir << ' ' << trk.E << ' ' << trk.t << endl;
  }
  
  for ( auto& hit : evt.mc_hits )  write ( hit, os, mc_keys::hit_t);
  for ( auto& hit : evt.hits    )  write ( hit, os, mc_keys::hit_raw_t);
  
  os << mc_keys::weights_t; for (auto&  w : evt.w      ) os << ' ' << w; os << endl;
  os << mc_keys::w2list_t;  for (auto&  w : evt.w2list ) os << ' ' << w; os << endl;
  os << mc_keys::w3list_t;  for (auto&  w : evt.w3list ) os << ' ' << w; os << endl;

  os << mc_keys::eventtime_t << evt.mc_event_time.GetSec() << " "
     << evt.mc_event_time.GetNanoSec() / 16 << endl;

  os << mc_keys::end_event_t << endl;

  // restore os to how we found it.
  os.flags( old_flags );
  os.precision( old_precision );

  return true;
}



/**
 * Read header from a stream.
 *
 * The stream may be positioned at any point before the tag mc_keys::start_run_t,
 * which marks the beginning of the header.
 * Information before the header (if any) is disgarded.
 *
 * \param  hdr           header
 * \param  is            input stream
 * \return               true if correctly read; else false
 */
inline bool read( Head& hdr, std::istream& is )
{
  using namespace std;

  string line;

  bool start = false;

  while (getline( is, line ))
    {
      if ( io_stringutil::startswith(line, mc_keys::end_event_t))
        {
          break;
        }

      if ( io_stringutil::startswith(line, mc_keys::start_run_t))
        {
          start = true;
        }

      if ( io_stringutil::startswith(line, mc_keys::start_event_t))
        {
          THROW(Exception, "Unexpected tag " << mc_keys::start_event_t << " found while reading header at " << line << " (could mean the evt file has no header)");
        }

      if (!start) continue;

      const string::size_type pos = line.find(':');

      if (pos == string::npos)
        {
          std::cout << "Warning: line with empty tag found when reading header" << endl;
          std::cout << "        "<< line << endl;
          std::cout << "         will be skipped" << endl;
          continue;
        }

      // the following with unsure key in the map is unique by adding _1 _2 etc.
      hdr.set_line( io_stringutil::trim(line.substr(0,pos)), io_stringutil::trim(line.substr(pos+1)) );

    }

  if (!start)
    {
      THROW(Exception, "Reading of MC header terminated before finding a start_run: tag. Please check your file");
    }

  return start;
}


/**
 * Write header to a stream.
 *
 * \param  hdr           header
 * \param  os            output stream
 * \return               true if correctly written; else false
 */
inline bool write( const Head& hdr, std::ostream& os )
{
  hdr.print(os);
  return true;
}

#endif
