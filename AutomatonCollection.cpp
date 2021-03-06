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

bool AutomatonCollection_c::InitThreadPool(std::vector<std::thread> &threads, size_t numThreads)
{
   bool rval = true;
   for(size_t i = 0; i < numThreads && rval; ++i)
   {
      rval &= this->AddThread(threads);
   }
   return rval;
}

bool AutomatonCollection_c::RunSegment(std::vector<std::thread> &threads)
{
   if (threads.size() > 0)
   {
      for(auto itr = std::begin(threads); itr != std::end(threads); ++itr)
      {
         if (itr->joinable())
         {
            itr->join();
            itr = threads.erase(itr);
            --itr;
            this->AddThread(threads);
            return threads.size() > 0;
         }
      }
      return true;
   }
   return false;
}

const std::vector<double>& AutomatonCollection_c::GetInitialConditions() const
{
   return this->mInitialCondition;
}

const std::vector<AutomatonCollection_c::CA_Info_s>& AutomatonCollection_c::GetAutomatonResults() const
{
   return this->mAutomatons;
}

const AutomatonCollection_c::CA_Info_s& AutomatonCollection_c::GetResult(size_t index) const
{
   return this->mAutomatons[index];  // no check, throws?
}

void AutomatonCollection_c::RunAutomaton(double a, double b)
{
   static CellularAutomaton_c::Config_s cfg {
      this->mConfig.ignore_count,
      this->mConfig.max_time_step,
      0,
      0,
      this->mConfig.lyapunov_error,
      this->mConfig.lyapunov_angle
   };
   cfg.init_row_count = this->mConfig.ignore_count;
   cfg.max_row_count = this->mConfig.max_time_step;
   cfg.a = a;
   cfg.b = b;
   cfg.error = this->mConfig.lyapunov_error;
   cfg.angle = this->mConfig.lyapunov_angle;
   CellularAutomaton_c ca(cfg, this->mInitialCondition);
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
   results.lyapunov_exponent = ca.GetAverageLyapunovExponent();
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
