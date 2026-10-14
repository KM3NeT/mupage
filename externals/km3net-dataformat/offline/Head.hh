#ifndef HEAD_HH_INCLUDED
#define HEAD_HH_INCLUDED

#include "km3net-dataformat/offline/Vec.hh"
#include "km3net-dataformat/offline/Exception.hh"

#include "TObject.h"

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>


/**
 * Trim a string in place
 *
 * \param  s             input string
 */
static inline void trimstring(std::string &s)
{
  // from the left
  s.erase( s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));

  // from the right
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

/**
 * Split string at delimiter. Trailing and leading whitespace is removed from each token.
 * Empty tokens are not put in the output list.
 *
 * \param  str           input string
 * \param  delim         token delimiter
 * \return               list of tokens
 */
inline std::vector<std::string> splitstring(const std::string& str, char delim = ' ')
{
  using namespace std;

  vector<string> r;

  stringstream ss(str);
  string token;
  while (getline(ss, token, delim))
  {
    trimstring(token);
    if (token != "") r.push_back(token);
  }

  return r;
}


/**
 * The Head class reflects the header of Monte-Carlo event files, which consists of keys (also referred to as "tags") and values.
 */
struct Head : public TObject, std::map<std::string, std::string>
{
  struct tags {
    static constexpr const char* const UUID = "UUID";
  };
  

  /**
   * Check availability of data with the given key.
   *
   * \param  key           key
   * \return               true if data are available; else false
   */
  bool have_line (std::string key ) const
  {
    return count( key ) != 0;
  }

  /**
   * Get data with the given key.\n
   * This method throws a run-time exception if no data are available.
   *
   * \param  key           key
   * \return               data
   */
  const std::string& get_line( std::string key ) const
  {
    return this->at(key);
  }

  /**
   * Get data with the given key.\n
   * This method throws a run-time exception if no data are available.
   *
   * \param  key           key
   * \return               data
   */
  std::string& get_line( std::string key )
  {
    return this->at(key);
  }


  /**
  * In case of duplicate keys, they are internally stored in the map
  * with a suffix "_n". This function returns all the keys that start
  * with 'key' and end in "_n", with n an integer
  *
  * \param tag            tag (without suffix)
  */

  std::vector< std::string> matching_keys( const std::string& tag ) const
  {
    std::vector< std::string> r;

    auto match = [&] (const std::string & key) {

      if (key == tag) return true;

      if ( key.find( tag ) != 0 ) return false;

      // what is left should be of the form _d(ddd)
      std::string left = key.substr( tag.length(), key.length() );
      if (left.length() < 2 || left[0] != '_' ) return false ;
      for ( unsigned i = 1; i < left.length(); i++ )
      {
        if (!std::isdigit( left[i] )) return false ;
      }
      return true;
    };

    for ( auto& p : *this )
    {
      if ( match( p.first ) ) r.push_back( p.first );
    }

    return r;
  }




  /**
  * Get all data compatible with the given key. This means all data
  * that is internally stored with "key_n", with n an integer \n
  * This method throws a run-time exception if no data are available.
  *
  * \param  tag           tag (without suffix)
  * \return               data
  */
  std::vector< std::string > get_lines( const std::string& tag ) const
  {
    std::vector< std::string > r;

    for ( auto& key : matching_keys( tag ) ) {
      r.push_back( get_line( key ) );
    }

    return r;
  }


  /**
   * Set data with the given tag. The function will return the actual key that
   * is used internally to store the result, which is equal to the tag with an
   * optional "_n" added to ensure uniqueness.
   *
   * \param  tag           tag
   * \param  line          data
   * \param  ensure_unique add '_n' (with n an integer) to the tag if it would overwrite an existing key.
   */

  std::string set_line( std::string tag, std::string line , bool ensure_unique = true )
  {
    std::string k = tag;

    if (ensure_unique)
      for (int i = 1; find(k) != end() ; i++)
      {
        k = tag + "_" + std::to_string(i);
      }

    std::map<std::string, std::string>::operator[]( k ) = line;
    return k;
  }

  /**
   * Get data with the given key at given index.\n
   * This method throws a run-time exception if no data are available.
   *
   * \param  key           key
   * \param  idx           index
   * \return               data
   */
  std::string get_field( std::string key, int idx ) const
  {
    using namespace std;

    vector<string> v = splitstring( get_line(key) );

    if ( idx < 0 || idx >= int ( v.size() ) )
    {
      THROW(Exception, "Cannot find word number " << idx << " in line " << get_line(key) << " for key: " << key);
    }
    return v[idx];
  }

  /**
   * Get index of data with the given key at given field.\n
   *
   * Note that this method uses the dictionary define in method Head::_hdr_dict.
   *
   * \param  key           key
   * \param  field         field
   * \return               index (-1 if not present)
   */
  int get_index_of_field(std::string key, std::string field) const
  {
    auto& d = _hdr_dict();
    if ( d.count(key) == 0 ) return -1;
    auto v = d.at(key);
    auto i = std::find (v.begin(), v.end(), field );
    if (i == v.end()) return -1;
    return i - v.begin();
  }

  /**
   * Get data with the given key at given field.\n
   * This method throws a run-time exception if no field is available.
   *
   * Note that this method uses the dictionary define in method Head::_hdr_dict.
   *
   * \param  key           key
   * \param  field         field
   * \return               data
   */
  std::string get_field( std::string key, std::string field ) const
  {
    int idx = get_index_of_field(key, field);

    if ( idx == -1 )
    {
      THROW(Exception, "Failed to find" << key << " " << field);
    }

    return get_field( key, idx );
  }


  /**
   * Set data with the given key at given field.\n
   * This method throws a run-time exception if no field available.
   *
   * Note that this method uses the dictionary define in method Head::_hdr_dict.
   *
   * \param  key           key
   * \param  field         field
   * \param  value         vakue
   */
  void set_field( std::string key, std::string field, std::string value )
  {
    using namespace std;

    if ( field == "" ) get_line( key ) = value;

    int idx = get_index_of_field( key, field );

    if ( idx < 0 )
    {
      THROW(Exception, "GFailed to find field in header line: " << key << " " << field);
    }

    vector<string> vals = splitstring( get_line( key ) );

    // if the fields before do not exist, add padding
    while ( int( vals.size() ) <= idx ) vals.push_back("0");

    vals[idx] = value;
    ostringstream ss;

    for (unsigned i = 0; i < vals.size() ; i++ )
    {
      ss << vals[i];
      if ( i != vals.size() - 1) ss << " ";
    }
    set_line( key, ss.str() );

  }

  /**
   * Print header.
   *
   * \param  out           output stream
   */
  void print ( std::ostream& out = std::cout ) const
  {
    if (count("start_run")) out << "start_run: " << at("start_run") << std::endl;

    for ( auto& p : *this )
    {
      if ( p.first == "start_run" || p.first == "end_event" ) continue;
      out << p.first << ": " << p.second << std::endl ;
    }
    out << "end_event:" << std::endl;
  }

  /**
   * Get internal description of the known lines in header.
   *
   * \return               internal dictionary
   */
  static const std::map<std::string, std::vector<std::string> >& _hdr_dict() 
  {
    using namespace std;

    // map with, for each tag (key), a vector of field-names

    static map<string, vector<string> > r;
    if ( r.size() > 0 ) return r;

    string desc =
      "DAQ:livetime\n"
      "cut_primary cut_seamuon cut_in cut_nu:Emin Emax cosTmin cosTmax\n"
      "generator physics simul:program version date time\n"
      "seed:program level iseed\n"
      "PM1_type_area:type area TTS\n"
      "PDF:i1 i2\n"
      "model:interaction muon scattering numberOfEnergyBins\n"
      "can:zmin zmax r\n"
      "genvol:zmin zmax r volume numberOfEvents\n"
      "merge:time gain\n"
      "coord_origin:x y z\n"
      "translate:x y z\n"
      "genhencut:gDir Emin\n"
      "k40:rate time\n"               // note lower-case k  
      "K40:livetime\n"                // note capital K
      "norma:primaryFlux numberOfPrimaries\n"
      "livetime:numberOfSeconds errorOfSeconds\n"
      "flux:type key file_1 file_2\n"
      "spectrum:alpha\n"
      "fixedcan:xcenter ycenter zmin zmax radius\n"
      "start_run:run_id";

    for ( auto line : splitstring(desc, '\n') )
    {
      auto v = splitstring( line, ':');

      vector< string > fields = splitstring( v[1] );
      for ( auto key : splitstring( v[0] ) )
      {
        r[key] = fields;
      }
    }
    return r;
  }


  /**
   * Get the number of generated events needed for computing event rates.
   *
   * \return               number of events
   */
  double ngen() const
  {
    return stod ( get_field("genvol", "numberOfEvents") );
  }

  /**
   * Get the the live time provided by the DAQ sytstem (=number of processed timeslices * frametime).
   *
   * \return               live time [s]
   */
  double daq_livetime() const
  {
    return stod ( get_field("DAQ", "livetime") );
  }


  /**
   * Get the Monte Carlo live time
   *
   * \return               live time [s]
   */
  double mc_livetime() const
  {
    return stod ( get_field("livetime", "numberOfSeconds") );
  }

  /**
   * Get coordinate origin.
   *
   * \return               position
   */
  Vec coord_origin() const
  {
    return Vec( stod( get_field("coord_origin", "x") ),
                stod( get_field("coord_origin", "y") ),
                stod( get_field("coord_origin", "z") ));
  }

  /**
   * Get coordinate translation.
   *
   * \return               translation
   */
  Vec translate() const
  {
    return Vec( stod( get_field("translate", "x") ),
                stod( get_field("translate", "y") ),
                stod( get_field("translate", "z") ));
  }

  virtual ~Head() {}

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

  ClassDef(Head, 2 );
};


/**
 * Print header.
 *
 * \param  out         output stream
 * \param  h           header
 * \return             output stream
 */
inline std::ostream& operator<<(std::ostream& out, const Head& h)
{
  h.print(out);
  return out;
}


#endif
