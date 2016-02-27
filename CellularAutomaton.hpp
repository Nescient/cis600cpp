/// Copyright © Sam Savage 2016
#pragma once

#include "ColumnCount.hpp"

/// This object represents a cellular automoton system.
class CellularAutomaton_c
{
public:
   /// This constructs a CA according to a,b constants and an initial condition.
   /// \param initRowCount The number of rows to throw away at the start.
   /// \param maxNumRows The maximum number of rows to create after the inital number of rows.
   /// \param a The constant a in the polynomial.
   /// \param b The constant b in the polynomial.
   /// \param firstRow The inital row for this CA.
   CellularAutomaton_c(const size_t initRowCount, const size_t maxNumRows,
      const double a, const double b, const std::vector<double> &firstRow);

   /// Creates a new row if fewer than the maximum have been created.
   /// \returns True if a row was created, otherwise this CA is done.
   bool CreateNewRow();

   /// Gets the current value of the a constant.
   /// \returns The value for the a polynomial constant.
   double A() const;

   /// Gets the current value of the b constant.
   /// \returns The value for the b polynomial constant.
   double B() const;

   /// Calculates the average entropy across the entire Cellular Automaton.
   /// \returns The CA average Shannon's entropy.
   double CalculateAverageEntropy() const;

   /// Calculates the standard deviation for the entropy across the entire Cellular Automaton.
   /// \returns The standard deviation for the Shannon's entropy.
   double CalculateEntropySigma() const;

   /// Gets the variance for the cellular automaton.
   /// \returns The variance of this CA.
   double GetAverageVariance() const;

private:
   /// Gets the next value given a previous and its neighbors.
   /// \param prev The previous left neighbor.
   /// \param curr The previous value.
   /// \param next The previous right neighbor.
   /// \returns The next value for the polynomial.
   double CalculatePolynomial(const double prev, const double curr, const double next) const;

   /// Sets the last row, adds column counts, and updates entropy.
   /// \param newRow The new Last Row.
   void ChangeLastRow(std::vector<double> &newRow);

private:
   const size_t mInitRowCount;
   const size_t mMaxRowCount;
   const double mA;
   const double mB;
   bool mInInitialState = true;
   size_t mRowCount = 0;
   std::vector<double> mLastRow;
   std::vector<ColumnCount_c> mColumnCounts;
};
