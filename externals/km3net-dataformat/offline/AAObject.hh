#ifndef AAOBJECTINCLUDED
#define AAOBJECTINCLUDED

#include "TObject.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "km3net-dataformat/offline/Exception.hh"


/*! \brief AAObject is a base class for I/O-classes that adds the possibility
  to add 'user' information which will also be stored in the ROOT file.
*/

struct AAObject : public TObject
{
  std::vector<double>      usr;              ///< user data
  std::vector<std::string> usr_names;        ///< user keys

  /**
   * Get index in user data of the item with given key.
   *
   * \param  key           key
   * \return               index (-1 if key does not exists)
   */
  int idx( const std::string& key ) const
  {
    auto i = std::find (usr_names.begin(), usr_names.end(), key );
    if (i == usr_names.end()) return -1;
    return i - usr_names.begin();
  }

  /**
   * Check availability of user data of the item with given key.
   *
   * \param  key           key
   * \return               true if available; else false
   */
  bool haveusr( const std::string& key ) const
  {
    return idx( key ) >= 0;
  }

  /**
   * Get index in user data of the item with given key.\n
   * This method throws a run-time exception if no user data are available.
   *
   * \param  key           key
   * \return               index (-1 if key does not exists)
   */
  int idxusr_checked( const std::string& key ) const
  {
    int r = idx( key );
    if (r < 0)
    {
      THROW(Exception, "No user data for key " << key << " in aanet object of type " << this -> ClassName());
    }
    return r;
  }


  /**
   * Get user data item with given key.\n
   * This method throws a run-time exception if no user data are available.
   *
   * \param  key           key
   * \return               value
   */
  double getusr(const std::string& key) const
  {
    const int i = idx( key );

    if ( i  < 0 )
    {
      THROW(Exception, "No user data for key " << key << " in aanet object of type " << this -> ClassName());
    }

    if ( unsigned(i) >= usr.size() )
    {
      THROW(Exception, "Warning: inconsistent user data " << i << " >= " << usr.size());
    }

    return usr[i];
  }

  /**
   * Set user data item with given key.\n
   *
   * \param  key           key
   * \param  value         value
   */
  void setusr(const std::string & key, double value )
  {
    int i = idx( key );
    if (i < 0)
    {
      if ( usr.size() < usr_names.size() )
      {
        // this should not happen, but let's just add empty data
        usr.resize( usr_names.size() );
      }
      else
      {
        // this is possible, add empty ("") names
        usr_names.resize( usr.size() );
      }

      usr_names.push_back( key );
      usr.push_back( value );
    }
    else
    {
      usr[i] = value;
    }
  }

  /**
   * Remove (first) user data item with given key.\n
   *
   * \param  key           key
   * \return               true if data have been removed; else false
   */
  bool delusr( const std::string& key )
  {
    int i = idx( key );
    if ( i < 0 ) return false;

    usr.erase ( usr.begin() + i );
    usr_names.erase( usr_names.begin() + i );
    return true;
  }

  /**
   * Clear user data.
   */
  void clearusr()
  {
    usr.resize(0);
    usr_names.resize(0);
  }

  /**
   * Print user data (i.e. list of all pairs of keys and values).
   *
   * \param  out           output stream
   */
  void printusr(std::ostream& out = std::cout )
  {
    unsigned n = std::max( usr.size(), usr_names.size() );

    for (unsigned i = 0; i < n ; i++)
    {
      std::string name = "(unnamed)";
      if ( i < usr_names.size() && usr_names[i] != "" ) name = usr_names[i];
      out << i << " \t " << name << " : \t ";
      if ( i < usr.size() ) out << usr[i] << std::endl;
      else                  out << "(none)" << std::endl;
    }
  }

  /**
   * Default constructor.
   */
  AAObject() : any(NULL) {}
  ~AAObject() {}


  TObject* any;                              ///< Pointer to "any" user data.

  ClassDef(AAObject, 6)
};

#endif
