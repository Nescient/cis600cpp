/// Copyright © Sam Savage 2016
#pragma once

#include <mutex>
#include <thread>
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
      double time_taken;  // seconds
   };
public:
   /// Creates a new collection of automatons and starts them running.
   /// \param config The configuration values for the automotons.
   AutomatonCollection_c(Config_s config);

   /// Starts the cellular automaton process, blocking until it finishes.
   void Run();

   /// Gets the Cellular Automaton results.
   /// \return The CA result structures.
   const std::vector<CA_Info_s>& GetAutomatonResults() const;

private:
   /// Creates a new automaton and iterates through its time steps,
   /// adding it to the automaton array when done.
   /// \param a The value of the a constant for the CA.
   /// \param b The value of the b constant for the CA.
   void RunAutomaton(double a, double b);

   /// Adds a new thread to the vector if a and b allow it.
   /// \param threads The current thread pool.
   /// \return True if a thread was created.
   bool AddThread(std::vector<std::thread> &threads);

private:
   const Config_s mConfig;
   double mCurrentA;
   double mCurrentB;
   std::mutex mAutomatonMutex;
   const std::vector<double> mInitialCondition;
   std::vector<CA_Info_s> mAutomatons;
};
