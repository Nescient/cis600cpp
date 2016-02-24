/// Copyright © Sam Savage 2016
#include "CellularAutomaton.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>

//http://stackoverflow.com/a/17323608
#define WRAPPED_MOD(n, m) ((n % m) + m) % m

std::string gToHexString(double first, double second, double third)
{
   std::stringstream ss;
   ss << std::setfill('0') << std::hex;
   ss << std::setw(6) << (static_cast<uint32_t>(first * 0xFFFFFF) & 0xFFFFFF);
   ss << std::setw(6) << (static_cast<uint32_t>(second * 0xFFFFFF) & 0xFFFFFF);
   ss << std::setw(6) << (static_cast<uint32_t>(third * 0xFFFFFF) & 0xFFFFFF);
   return ss.str();
}

CellularAutomaton_c::CellularAutomaton_c(const size_t initRowCount, const size_t maxNumRows,
   const double a, const double b, const std::vector<double> &firstRow) :
      mInitRowCount(initRowCount), mMaxRowCount(maxNumRows), mA(a), mB(b),
      mLastRow(firstRow)
{
   for(size_t i = 0; i < this->mLastRow.size(); ++i)
   {
      this->mColumnCounts.push_back(ColumnCount_c(i));
   }
   return;
}

bool CellularAutomaton_c::CreateNewRow()
{
   if (this->mInInitialState && (this->mRowCount > this->mInitRowCount))
   {
      this->mInInitialState = false;
      this->mRowCount = 0;
   }
   // if we are half way done and still have 0 entropy, shortcut out
   if (this->mRowCount == (this->mMaxRowCount / 2) && (this->CalculateAverageEntropy() == 0))
   {
      return false;
   }

   std::vector<double> new_row(this->mLastRow.size());
   for(size_t i = 0; i < new_row.size(); ++i)
   {
      const double previous = (i == 0) ? this->mLastRow[WRAPPED_MOD(i - 1, this->mLastRow.size())] : this->mLastRow[i-1];
      const double current = this->mLastRow[i];
      const double next = (i+1 == this->mLastRow.size()) ? this->mLastRow[WRAPPED_MOD(i + 1, this->mLastRow.size())] : this->mLastRow[i+1];
      new_row[i] = this->CalculatePolynomial(previous, current, next);
   }
   this->ChangeLastRow(new_row);

   // the caller could technically keep asking us to continue if they want
   return ++this->mRowCount < this->mMaxRowCount;
}

double CellularAutomaton_c::A() const
{
   return this->mA;
}

double CellularAutomaton_c::B() const
{
   return this->mB;
}

double CellularAutomaton_c::CalculateAverageEntropy() const
{
   double avg = 0;
   for(auto &column : this->mColumnCounts)
   {
      avg += column.GetAverageEntropy();
   }
   return avg / this->mColumnCounts.size();
}

double CellularAutomaton_c::CalculateEntropySigma() const
{
   return sqrt(this->GetAverageVariance());
}

double CellularAutomaton_c::GetAverageVariance() const
{
   double avg = 0;
   for(auto &column : this->mColumnCounts)
   {
      avg += column.GetEntropyVariance();
   }
   return avg / this->mColumnCounts.size();
}

double CellularAutomaton_c::CalculatePolynomial(const double u/*prev*/, const double x/*curr*/, const double v/*next*/) const
{
   // taken from Blair's ALife1Dim Java program
   return (0.5 - 0.5 * cos(M_PI * (this->mA + (this->mA + this->mB) * u 
      +(this->mA - this->mB) * v +this->mB * u * v -2 * u * x * v)));
}

void CellularAutomaton_c::ChangeLastRow(std::vector<double> &newRow)
{
   this->mLastRow = std::move(newRow);
   if (!this->mInInitialState)
   {
      for(size_t i = 0; i < this->mLastRow.size(); ++i)
      {
         const double previous = (i == 0) ? this->mLastRow[WRAPPED_MOD(i - 1, this->mLastRow.size())] : this->mLastRow[i-1];
         const double current = this->mLastRow[i];
         const double next = (i+1 == this->mLastRow.size()) ? this->mLastRow[WRAPPED_MOD(i + 1, this->mLastRow.size())] : this->mLastRow[i+1];
         const std::string value = gToHexString(previous, current, next);
         this->mColumnCounts[i].IncrementCount(value);
         this->mColumnCounts[i].UpdateEntropy(this->mRowCount);
      }
   }
   return;
}
