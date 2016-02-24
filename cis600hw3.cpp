/// Copyright © Sam Savage 2016
#include "AutomatonCollection.hpp"
#include <cstdlib>
#include <fstream>

int main(int argc, char* argv[])
{
   Config_s config;
   AutomatonCollection_c hw3(config);
   hw3.Run();
   std::ofstream fout(argc > 1 ? argv[1] : "cis600hw3out.csv");
   fout << "a,b,entropy_width,avg_entropy,entropy_stdev,variance,row_length,time_steps_dropped,total_time_steps,time_taken(seconds)" << std::endl;
   for (auto &r : hw3.GetAutomatonResults())
   {
      fout << r.a << ",";
      fout << r.b << ",";
      fout << r.entropy_width << ",";
      fout << r.avg_entropy << ",";
      fout << r.entropy_stdev << ",";
      fout << r.variance << ",";
      fout << r.row_length << ",";
      fout << r.time_steps_dropped << ",";
      fout << r.total_time_steps << ",";
      fout << r.time_taken << ",";
      fout << std::endl;
   }
   fout.close();
   return EXIT_SUCCESS;
}
