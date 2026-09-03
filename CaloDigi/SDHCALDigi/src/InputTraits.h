#ifndef InputTraits_h
#define InputTraits_h

#include "EVENT/LCEvent.h"
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/SimCalorimeterHit.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <IMPL/LCFlagImpl.h>

struct ILCInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = EVENT::SimCalorimeterHit;
  using calohitType = IMPL::CalorimeterHitImpl;
  using flagType = IMPL::LCFlagImpl;
};

#endif