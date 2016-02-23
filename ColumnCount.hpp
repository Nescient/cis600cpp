/// Copyright © Sam Savage 2016
#pragma once

#include <map>
#include <string>
#include <vector>

/// This object holds a collection of counts for a specific column and can thus calculate its Shannon's entropy.
class ColumnCount_c
{
public:
   /// Initializes this object to a specific column number.
   /// \param column The column this object represents.
   ColumnCount_c(int column);

   /// Adds a string to the count collection.
   /// \param value A string found in the history of this column.
   void IncrementCount(const std::string &value);

   /// Updates the entropy for the latest row.
   /// \rowCount The number of rows that have been processed.
   void UpdateEntropy(const size_t rowCount);

   /// Returns the average entropy for this column.
   /// \returns The average Shannon's entropy.
   double GetAverageEntropy() const;

   /// Returns the entropy variance for this column.
   /// \returns The Shannon's entropy variance for the whole column.
   double GetEntropyVariance() const;

private:
   /// Returns the entropy for this column.
   /// \returns The Shannon's entropy for all rows so far.
   double CalculateEntropy(const size_t rowCount) const;

private:
   const size_t mColumnNumber;
   std::map<std::string, size_t> mCount;
   std::vector<double> mEntropy;
};
