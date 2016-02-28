/// Copyright © Sam Savage 2016
#pragma once

#include "ColumnCount.hpp"

/// This object represents a cellular automoton system.
class CellularAutomaton_c
{
public:
   /// This structure represents the configuration data needed by the CA.
   struct Config_s
   {
      size_t init_row_count{0};  // The number of rows to throw away at the start.
      size_t max_row_count{500};  // The maximum number of rows to create after the inital number of rows.
      double a{0};  // The constant a in the polynomial.
      double b{0};  // The constant b in the polynomial.
      double error{0};  // The error epsilon used in the Lyapunov exponent calculation.
      double angle{0};  // The angle phi used in the Lyapunov exponent calculation.
   };

   /// This constructs a CA according to a,b constants and an initial condition.
   /// \param config The configuration values needed by this CA.
   /// \param firstRow The inital row for this CA.
   CellularAutomaton_c(const Config_s &config, const std::vector<double> &firstRow);

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

   /// Gets the average Lyapunov exponent.
   /// \returns The average Lyapunov exponent for this CA.
   double GetAverageLyapunovExponent() const;

private:
   /// Gets the next value given a previous and its neighbors.
   /// \param prev The previous left neighbor.
   /// \param curr The previous value.
   /// \param next The previous right neighbor.
   /// \returns The next value for the polynomial.
   double CalculatePolynomial(const double prev, const double curr, const double next) const;

   /// Sets the last row, adds column counts, and updates entropy.
   /// \param newRow The new Last Row.
   /// \param newError The new error row.
   void ChangeLastRow(std::vector<double> &newRow, std::vector<double> &newError);

private:
   const Config_s mConfig;
   bool mInInitialState = true;
   size_t mRowCount = 0;
   std::vector<double> mLastRow;
   std::vector<double> mErrorRow;
   std::vector<double> mErrorFactors;
   std::vector<ColumnCount_c> mColumnCounts;
};
