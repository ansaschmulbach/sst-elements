// Copyright 2009-2013 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2013, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _SST_SIMPLE_DISTRIB_COMPONENT_H
#define _SST_SIMPLE_DISTRIB_COMPONENT_H

#include <sst/core/event.h>
#include <sst/core/sst_types.h>
#include <sst/core/component.h>
#include <sst/core/link.h>
#include <sst/core/timeConverter.h>

#include <sst/core/rng/distrib.h>
#include <sst/core/rng/expon.h>

#include <sst/core/rng/sstrand.h>
#include <sst/core/rng/mersenne.h>
#include <sst/core/rng/marsaglia.h>

//#include <cstring>
#include <string>

using namespace SST;
using namespace SST::RNG;

namespace SST {
namespace SimpleDistribComponent {

class SimpleDistribComponent : public SST::Component {
public:

  SimpleDistribComponent(SST::ComponentId_t id, SST::Params& params);
  void setup()  { }
  void finish() { }

private:
  SimpleDistribComponent();  // for serialization only
  SimpleDistribComponent(const SimpleDistribComponent&); // do not implement
  void operator=(const SimpleDistribComponent&); // do not implement

  virtual bool tick( SST::Cycle_t );

  SSTRandom* rng;
  std::string rng_type;
  int rng_max_count;
  int rng_count;

  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_NVP(rng_count);
    ar & BOOST_SERIALIZATION_NVP(rng_max_count);
    ar & BOOST_SERIALIZATION_NVP(rng_type);
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_NVP(rng_count);
    ar & BOOST_SERIALIZATION_NVP(rng_max_count);
    ar & BOOST_SERIALIZATION_NVP(rng_type);
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

};

}
}

#endif /* _simpleDistribComponent_H */
