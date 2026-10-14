#ifndef MULTIHEAD_HH_INCLUDED
#define MULTIHEAD_HH_INCLUDED

#include <vector>
#include <uuid/uuid.h>

#include "km3net-dataformat/offline/Evt.hh"
#include "km3net-dataformat/offline/Head.hh"

#include "TObject.h"


/**
 * \author bjung
 */

struct MultiHead :
  public std::vector<Head>,
  public TObject
{
  /**
   * Default constructor.
   */
  MultiHead() :
    std::vector<Head>(),
    TObject()
  {}
    
    
  /**
   * Virtual destructor.
   */
  virtual ~MultiHead()
  {}
    

  /**
   * Find header with given UUID.\n
   * Note: The parameter useCache can be toggled on for faster lookup.\n
   *       This should not be used if your `MultiHead` object is modified between look-ups.
   *       
   *
   * \param  uuid           header UUID
   * \param  useCache       use caching for faster look-up
   * \return                header with given UUID
   */
  const_iterator find(const uuid_t& uuid,
		      const bool    useCache = false) const
  {
    using namespace std;

    static struct Cache
    {
      Cache() { uuid_clear(this->uuid); }
      
      const_iterator it;
      uuid_t         uuid;
    } cache;
    
    if (!useCache) {

      for (cache.it = this->cbegin(); cache.it != this->cend(); ++cache.it) {

	const Head& head     = *cache.it;
	string      uuid_str = head.at(Head::tags::UUID);

	uuid_str.erase(remove_if(uuid_str.begin(), uuid_str.end(), ::isspace), uuid_str.end());
	
	uuid_parse(uuid_str.c_str(), cache.uuid);
	
	if (uuid_compare(uuid, cache.uuid) == 0) {
	  return cache.it;
	}
      }
      
      return this->end();
      
    } else {

      if (uuid_compare(uuid, cache.uuid) == 0) {
	return cache.it;
      } else {
	return find(uuid, false);
      }
    }
  }


  /** 
   * Find the header corresponding to the given event.
   * Note: The parameter useCache can be toggled on for faster lookup.\n
   *       This should not be used if your `MultiHead` object is modified between look-ups.
   *
   * \param  event          event
   * \param  useCache       use caching for faster look-up
   * \return                header corresponding to the given event
   */
  const_iterator find(const Evt& event,
		      const bool useCache = false) const
  {
    return find(event.header_uuid, useCache);
  }


  /**
   * Insert the given header.
   *
   * \param  header         header
   * \return                true if insertion was successful; else false
   */
  bool insert(const Head& header)
  {
    using namespace std;
    
    string uuid_str = header.at(Head::tags::UUID);
    uuid_str.erase(remove_if(uuid_str.begin(), uuid_str.end(), ::isspace), uuid_str.end());

    uuid_t uuid;
    uuid_parse(uuid_str.c_str(), uuid);

    if (uuid_is_null(uuid) == 0 && find(uuid) == this->cend()) {
      
      this->push_back(header);
      
      return true;
    }
    
    return false;
  }
  

  /**
   * Join given `MultiHead` object with this `MultiHead` object.
   *
   * \param  multiHead      `MultiHead` object
   * \return                 number of inserted headers
   */
  size_t join(const MultiHead& multiHead)
  {
    using namespace std;

    size_t n = 0;
    
    for (const_iterator i = multiHead.cbegin(); i != multiHead.cend(); ++i) {
      n += (size_t) this->insert(*i);
    }

    return n;
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

  ClassDef(MultiHead, 1);
};

#endif
