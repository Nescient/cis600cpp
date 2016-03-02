/// Copyright © Sam Savage 2016
// compile with g++ -std=c++14 cistest.cpp
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

void increment_count(std::map<int, size_t> &map, int value)
{
   auto itr = map.find(value);
   map[value] = (itr == map.end()) ? 1 : itr->second + 1;
   return;
}

void count_helper(std::map<int, size_t> &map, int start, int count)
{
   if (count >= 0)
   {
      increment_count(map, start - 1);
      increment_count(map, start + 1);
      count_helper(map, start - 1, count - 1);
      count_helper(map, start + 1, count - 1);
   }
   return;
}

int main(int argc, char* argv[])
{
   int max_count = 10;
   if (argc > 1)
   {
      try
      {
         max_count = std::stoi(argv[1]);
      }
      catch(...) { std::cerr << "! cannot convert " << argv[1] << " to a number." << std::endl; }
   }
   std::map<int, size_t> map;
   count_helper(map, 0, max_count);
   std::cout << " Value : Count " << std::endl;
//   std::cout << std::setw(max_count/10 + 2);
   for(auto &pair : map)
   {
      if ((max_count % 2) != (std::abs(pair.first) % 2))
      {
         std::cout<<std::setw(max_count/10 + 2) << pair.first << " : " << pair.second << std::endl;
      }
   }
	return EXIT_SUCCESS;
}
