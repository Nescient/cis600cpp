/// Copyright © Sam Savage 2016
#include "AutomatonCollection.hpp"
#include "CellularAutomaton.hpp"
#include <algorithm>
#include <thread>

// helper, see: http://stackoverflow.com/a/2860739
struct gen_rand
{
   double operator()()
   { 
      return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
   }
};

std::vector<double> get_randoms(size_t len)
{
   std::vector<double> rval;
   rval.reserve(len);
   std::generate_n(std::back_inserter(rval), len,
      [](){ return static_cast<double>(rand()) / static_cast<double>(RAND_MAX); });
   return std::move(rval);
}

AutomatonCollection_c::AutomatonCollection_c(Config_s config) : mConfig(config),
   mInitialCondition(get_randoms(config.row_count))
{
   //std::vector<double> ic;
   //ic.reserve(this->mConfig.row_count);
   //std::generate_n(std::back_inserter(ic), this->mConfig.row_length, gen_rand());
   //std::generate_n(std::back_inserter(ic), this->mConfig.row_length, () -> {return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);});
   //this->mInitialCondition = std::vector<double>(ic);
   return;
}

void AutomatonCollection_c::Run()
{
   std::thread t;
   return;
}

void AutomatonCollection_c::RunAutomaton(double a, double b)
{
  // CellularAutomaton_c ca(this->mConfig.ignore_count, this->mConfig.max_time_step,
    //                       a, b, this->mInitialCondition);
   return;
}