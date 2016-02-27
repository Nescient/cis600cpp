/// Copyright © Sam Savage 2016
#include "AutomatonCollection.hpp"
#include <cstdlib>
#include <fstream>

int main(int argc, char* argv[])
{
   Config_s config;
   AutomatonCollection_c hw3(config);

   size_t index = 0;
   using namespace std::literals;
   std::vector<std::thread> threads;
   if (!hw3.InitThreadPool(threads)) { return EXIT_FAILURE; }

   std::ofstream fout(argc > 1 ? argv[1] : "cis600hw3out.csv");
   fout << "a,b,entropy_width,avg_entropy,entropy_stdev,variance,row_length,time_steps_dropped,total_time_steps,time_taken(seconds)" << std::endl;

   while (hw3.RunSegment(threads))
   {
      auto &ca = hw3.GetResult(index++);
      fout << ca.a << ",";
      fout << ca.b << ",";
      fout << ca.entropy_width << ",";
      fout << ca.avg_entropy << ",";
      fout << ca.entropy_stdev << ",";
      fout << ca.variance << ",";
      fout << ca.row_length << ",";
      fout << ca.time_steps_dropped << ",";
      fout << ca.total_time_steps << ",";
      fout << ca.time_taken << ",";
      fout << std::endl;
      std::this_thread::sleep_for(2s);
   }
   fout.close();
   return EXIT_SUCCESS;
}
