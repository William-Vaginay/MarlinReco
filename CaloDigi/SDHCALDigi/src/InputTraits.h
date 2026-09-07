#ifndef InputTraits_h
#define InputTraits_h

#include "EVENT/LCEvent.h"
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/SimCalorimeterHit.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <IMPL/LCFlagImpl.h>
#include <EVENT/LCParameters.h>
#include <IMPL/LCRelationImpl.h>

struct ILCInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = EVENT::SimCalorimeterHit;
  using calohitType = IMPL::CalorimeterHitImpl;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = EVENT::LCParameters;
  using objectType = EVENT::LCObject;

  static collectionType* getCollection(const eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  static void addCollection(eventType* evt, collectionVecType* col, const std::string& name) { evt->addCollection(col, name.c_str()); }

  static int getNumberOfElements(const collectionType* col) { return col->getNumberOfElements(); }
  static objectType* getElementAt(const collectionType* col, int index) { return col->getElementAt(index); }

  static void setFlag(collectionVecType* col, const int& flag) { col->setFlag(flag); }
  static parametersType& parameters(collectionVecType* col) { return col->parameters(); }
  static void addElement(collectionVecType* col, objectType* obj) {col->addElement(obj); }

  static int getFlag(const flagType& flag) { return flag.getFlag(); }

  static void setRawHit(calohitType* hit, objectType* rawHit) { hit->setRawHit(rawHit); }
  static float getEnergy(const calohitType* hit) { return hit->getEnergy(); }
  static void setEnergy(calohitType* hit, float energy) { hit->setEnergy(energy); }
  static int getCellID0(const calohitType* hit) { return hit->getCellID0(); }
  static int getCellID1(const calohitType* hit) { return hit->getCellID1(); }
  static void setTime(calohitType* hit, float time) { hit->setTime(time); }
  
  static void setValue(LCParameters& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
};

#endif