/* PIPS-IPM                                                           *
 * Author:  Cosmin G. Petra                                           *
 * (C) 2012 Argonne National Laboratory. See Copyright Notification.  */

#ifndef STOCH_TREE_IMPL
#define STOCH_TREE_IMPL

#include "sTree.h"
#include "stochasticInput.hpp"

/** A full implementation of sTree that is currently used, the other implementation
 *  being sTreeCallbacks (obsolete)
 * 
 */

class sTreeImpl : public sTree
{
 public:
  sTreeImpl(stochasticInput &in, MPI_Comm comm=MPI_COMM_WORLD);
 private: 
  sTreeImpl(int idx, stochasticInput &in);
 public:
  virtual ~sTreeImpl();

  StochSymMatrix*   createQ() const;
  StochVector*      createc() const;

  StochVector*      createxlow()  const;
  StochVector*      createixlow() const;
  StochVector*      createxupp()  const;
  StochVector*      createixupp() const;


  StochGenMatrix*   createA() const;
  StochVector*      createb() const;


  StochGenMatrix*   createC() const;
  StochVector*      createclow()  const;
  StochVector*      createiclow() const;
  StochVector*      createcupp()  const;
  StochVector*      createicupp() const;

  int nx() const;
  int my() const; 
  int mz() const; 
  int id() const; 

  void computeGlobalSizes();
  void loadLocalSizes();
 private:
  int m_id;
  stochasticInput& in;

  sTreeImpl* parent;

  size_t m_nx, m_my, m_mz;

  //int compute_nFirstStageEq();
  void splitConstraints_stage1();
  void splitConstraints_stage2(int scen);
  //int compute_nSecondStageEq(int scen);
};



#endif
