/// Copyright © Sam Savage 2016
#include <cstdlib>

#include "CellularAutomaton.hpp"

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

int main(int argc, char* argv[])
{
   Config_s config;
   return EXIT_SUCCESS;
}
