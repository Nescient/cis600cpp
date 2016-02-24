/// Copyright © Sam Savage 2016
#include "AutomatonCollection.hpp"
#include "CellularAutomaton.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

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
   this->mCurrentA = this->mConfig.a_start;
   this->mCurrentB = this->mConfig.b_start;
   return;
}

void AutomatonCollection_c::Run()
{
   using namespace std::literals;
   std::vector<std::thread> threads;
   threads.push_back(std::thread(&AutomatonCollection_c::RunAutomaton, this,
      this->mCurrentA.fetch_add(this->mConfig.a_increment),
      this->mCurrentB.fetch_add(this->mConfig.b_increment)));
   while(threads.size() > 0)
   {
      for(std::thread &t : threads)
      {
         if (t.joinable())
         {
            t.join();
            //push_back new one if a < b etc etc
         }
      }
      std::this_thread::sleep_for(2s);
   }
   return;
}

void AutomatonCollection_c::RunAutomaton(double a, double b)
{
   CellularAutomaton_c ca(this->mConfig.ignore_count, this->mConfig.max_time_step,
                           a, b, this->mInitialCondition);
   const std::clock_t start = std::clock();
   while(ca.CreateNewRow()) {};
   const std::clock_t end = std::clock();
   CA_Info_s results{};
   results.a = ca.A();
   results.b = ca.B();
   results.entropy_width = 3;
   results.avg_entropy = ca.CalculateAverageEntropy();
   results.entropy_stdev = ca.CalculateEntropySigma();
   results.variance = ca.GetAverageVariance();
   results.row_length = this->mInitialCondition.size();
   results.time_steps_dropped = this->mConfig.ignore_count;
   results.total_time_steps = this->mConfig.max_time_step;
   results.time_taken = static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC);
   this->mAutomatonMutex.lock();
   this->mAutomatons.push_back(results);
   this->mAutomatonMutex.unlock();
   return;
}
