// Copyright 2009-2021 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2021, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


// This include is ***REQUIRED*** 
// for ALL SST implementation files
#include "sst_config.h"

#include "basicStatistics.h"


using namespace SST;
using namespace SST::simpleElementExample;

/* 
 * During construction the component should prepare for simulation
 * - Read parameters
 * - Register its clock
 * - Initialize the RNG
 * - Register statistics
 */
basicStatistics::basicStatistics(ComponentId_t id, Params& params) : Component(id) {

    // Get parameters from the Python input
    unsigned int mars_z = params.find<unsigned int>("marsagliaZ", 42);
    unsigned int mars_w = params.find<unsigned int>("marsagliaW", 3498);
    unsigned int mers = params.find<unsigned int>("mersenne", 194785);
    runcycles = params.find<Cycle_t>("run_cycles", 10000);
    unsigned int subids = params.find<unsigned int>("subids", 5);

    // Tell the simulation not to end until we're ready
    registerAsPrimaryComponent();
    primaryComponentDoNotEndSim();

    //set our clock. The simulator will call 'clockTic' at a 4GHz frequency
    registerClock("4 GHz", new Clock::Handler<basicStatistics>(this, &basicStatistics::clockTic));

    // Initialize the random number generators
    rng0 = new SST::RNG::MarsagliaRNG(mars_z, mars_w);
    rng1 = new SST::RNG::MersenneRNG(mers);

    // Register the statistics to link our variables to the documented statistic name

    // These statistics demonstrate different types
    stat_U64 = registerStatistic<uint64_t>("UINT64_statistic"); // Counts uint64_t generated by rng0
    stat_I32 = registerStatistic<int32_t>("INT32_statistic");   // Counts int32_t generated by rng0
    stat_I64 = registerStatistic<int64_t>("INT64_statistic");   // Counts int64_t generated by rng0
    
    // These statistics demonstrate mapping multiple statistic pointers to the same name 
    // (more useful in complex Components and SubComponents that share statistics)
    stat_U32 = registerStatistic<uint32_t>("UINT32_statistic_duplicate");           // Counts uint32_t generated by rng0, records to UINT32_statistic_duplicate
    stat_U32_duplicate = registerStatistic<uint32_t>("UINT32_statistic_duplicate"); // Counts uint32_t generated by rng1, records to UINT32_statistic_duplicate
    stat_U32_single = registerStatistic<uint32_t>("UINT32_statistic");              // Also counts uint32_t generated by rng0, but records UINT32_statistic

    // This statistic demonstrates the use of SubIDs to generate multiple instances of the same statistic
    // In the registration we provide a unique string to identify each instance
    for (unsigned int i = 0; i < subids; i++) {
        stat_subid.push_back( registerStatistic<double>("SUBID_statistic", std::to_string(i)));
    }
}


/*
 * Destructor, clean up our RNGs
 */
basicStatistics::~basicStatistics()
{
    delete rng0;
    delete rng1;
}


/* 
 * On each clock cycle we will generate some new random numbers and use them
 * to add data to our statistics. 
 * When cycleCount reaches runcycles, we will stop simulation
 */
bool basicStatistics::clockTic( Cycle_t cycleCount)
{

    // Generate some numbers
    // Scale them so that we're working with smaller numbers
    uint32_t u32_rng0 = rng0->generateNextUInt32() / 10000000;
    uint32_t u32_rng1 = rng1->generateNextUInt32() / 10000000;
    int32_t  i32_rng0 = rng0->generateNextInt32()  / 10000000;
    uint64_t u64_rng0 = rng0->generateNextUInt64() / 1000000000000000;
    int64_t  i64_rng0 = rng0->generateNextInt64()  / 1000000000000000;

    // Add data to our statistics
    stat_U32_single->addData(u32_rng0);
    stat_U32->addData(u32_rng0);
    stat_U32_duplicate->addData(u32_rng1);

    stat_U64->addData(u64_rng0);
    stat_I32->addData(i32_rng0);
    stat_I64->addData(i64_rng0);

    // Add data to the SubID statistics in a loop
    for (size_t i = 0; i < stat_subid.size(); i++) {
        double value = rng1->nextUniform();
        stat_subid[i]->addData(value);
    }

    // Check if the exit condition is met
    if (cycleCount > runcycles) {
        
        // Tell SST that it's OK to end the simulation (once all primary components agree, simulation will end)
        primaryComponentOKToEndSim(); 

        // Return true to indicate that this clock handler should be disabled
        return true;
    }

    // Return false to indicate the clock handler should not be disabled
    return false;
}
