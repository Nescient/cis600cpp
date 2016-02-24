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
   mCurrentA(config.a_start), mCurrentB(config.b_start),
   mInitialCondition(get_randoms(config.row_count))
{
   return;
}

void AutomatonCollection_c::Run()
{
   using namespace std::literals;
   std::vector<std::thread> threads;

   for(size_t i = 0; i < 4; ++i)
   {
      this->AddThread(threads);
   }
   while(threads.size() > 0)
   {
      for(auto itr = std::begin(threads); itr != std::end(threads); ++itr)
      {
         if (itr->joinable())
         {
            itr->join();
            itr = threads.erase(itr);
            --itr;
            this->AddThread(threads);
         }
      }
      std::this_thread::sleep_for(2s);
   }
   return;
}

const std::vector<AutomatonCollection_c::CA_Info_s>& AutomatonCollection_c::GetAutomatonResults() const
{
   return this->mAutomatons;
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

bool AutomatonCollection_c::AddThread(std::vector<std::thread> &threads)
{
   if (this->mCurrentB <= this->mConfig.b_stop)
   {
      threads.push_back(std::thread(&AutomatonCollection_c::RunAutomaton, this,
         this->mCurrentA, this->mCurrentB));
      this->mCurrentA += this->mConfig.a_increment;
      if (this->mCurrentA > this->mConfig.a_stop)
      {
         this->mCurrentA = 0;
         this->mCurrentB += this->mConfig.b_increment;
      }
      return true;
   }
   return false;
}
