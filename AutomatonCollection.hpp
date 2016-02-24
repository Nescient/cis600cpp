/// Copyright © Sam Savage 2016
#pragma once

#include <atomic>
#include <mutex>
#include <vector>

struct Config_s
{
   double a_start = 0;
   double b_start = 0;
   double a_stop = 2;
   double b_stop = 2;
   double a_increment = 0.001;
   double b_increment = 0.001;
   double ignore_count = 500;
   double max_time_step = 2000;
   int row_count = 400;
};

/// This object manages a set of cellular automatons,
/// running them to completion and saving the results to a file.
class AutomatonCollection_c
{
public:
   /// Represents the results of a cellular automaton.
   struct CA_Info_s
   {
      double a;
      double b;
      double entropy_width;
      double avg_entropy;
      double entropy_stdev;
      double variance;
      double row_length;
      double time_steps_dropped;
      double total_time_steps;
      double time_taken;
   };
public:
   /// Creates a new collection of automatons and starts them running.
   /// \param config The configuration values for the automotons.
   AutomatonCollection_c(Config_s config);

   /// Starts the cellular automaton process, blocking until it finishes.
   void Run();

private:
   /// Creates a new automaton and iterates through its time steps,
   /// adding it to the automaton array when done.
   /// \param a The value of the a constant for the CA.
   /// \param b The value of the b constant for the CA.
   void RunAutomaton(double a, double b);

private:
   const Config_s mConfig;
   std::atomic<double> mCurrentA{0};
   std::atomic<double> mCurrentB{0};
   std::mutex mAutomatonMutex;
   const std::vector<double> mInitialCondition;
   std::vector<CA_Info_s> mAutomatons;
};
