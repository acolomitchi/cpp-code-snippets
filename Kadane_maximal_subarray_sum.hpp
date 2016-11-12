/*
 * Kadane_maximal_subarray_sum.hpp
 *
 *  Created on: 12 Nov 2016
 *      Author: acolomitchi
 */

#ifndef KADANE_MAXIMAL_SUBARRAY_SUM_HPP_
#define KADANE_MAXIMAL_SUBARRAY_SUM_HPP_

#include <vector>

/**
 * See: https://en.wikipedia.org/wiki/Maximum_subarray_problem
 *
 * VectT must report .size() and define `const E& operator[](std::size_t)`.
 *
 * E must be additive (a binary + set of operators must exist) and
 * the result of adding two E-s must be convertible to SumT.
 *
 * SumT must:
 * - be additive to an E and the result must have the same SumT type
 * - be additive to a SumT and the result must have the same SumT type
 * - have 'operator <' and 'operator =' defined
 * - be initializable with a value of integral zero and the value
 *   so initialized must represent the neutral additive value
 *   (that is `SumT a...; SumT zero(0); SumT b=zero+a;`
 *   must result in `(a<b || b<a) == false;`
 */

template<typename SumT> struct KadaneResult {
  std::size_t start;
  std::size_t end;
  SumT   mss; // maximum sub-array sum

  KadaneResult() :
    start(static_cast<std::size_t>(-1)),
    end(static_cast<std::size_t>(-1)),
    mss(0)
  {
  }
  KadaneResult(const KadaneResult& o) :
    start(o.start), end(o.end), mss(0)
  {
    this->mss=o.mss;
  }

  KadaneResult& operator=(const KadaneResult& o) {
    this->start=o.start;
    this->end=o.end;
    this->mss=o.mss;
    return *this;
  }
};

template <typename E, typename SumT=E, typename VectT=std::vector<E>>
SumT kadane_mss(const VectT& array, KadaneResult<SumT>* detailedResult=nullptr)
{
  SumT ret(0);
  std::size_t len=array.size();
  if(len>0) {
    SumT zero(0);
    KadaneResult<SumT> localResult;
    localResult.start=0; localResult.end=1; localResult.mss=zero+array[0];
    KadaneResult<SumT> globalResult(localResult);
    for(std::size_t i=1; i<len; i++) {
      SumT localSum=localResult.mss+array[i];
      SumT localElem=zero+array[i];
      if(localSum<localElem) {
        // the local element is better alone than when considered
        // as the end of a subarray
        localResult.mss=localElem;
        localResult.start=i;
        localResult.end=i+1;
      }
      else {
        // a sub-array extended by the current element is still better
        // than the current element alone
        localResult.end++;
        localResult.mss=localSum;
      }
      if(globalResult.mss<localResult.mss) {
        globalResult=localResult;
      }
    }
    if(nullptr!=detailedResult) {
      *detailedResult=globalResult;
    }
    ret=globalResult.mss;
  }
  return ret;
}



#endif /* KADANE_MAXIMAL_SUBARRAY_SUM_HPP_ */
