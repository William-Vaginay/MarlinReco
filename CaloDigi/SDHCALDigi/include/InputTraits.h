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
};

namespace SimDigital_Data {
  ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  void addCollection(ILCInputTraits::eventType* evt, ILCInputTraits::collectionVecType* col, const std::string& name) { evt->addCollection(col, name.c_str()); }

  int getNumberOfElements(const ILCInputTraits::collectionType* col) { return col->getNumberOfElements(); }
  ILCInputTraits::objectType* getElementAt(const ILCInputTraits::collectionType* col, int index) { return col->getElementAt(index); }

  void setFlag(ILCInputTraits::collectionVecType* col, const int& flag) { col->setFlag(flag); }
  ILCInputTraits::parametersType& parameters(ILCInputTraits::collectionVecType* col) { return col->parameters(); }
  void addElement(ILCInputTraits::collectionVecType* col, ILCInputTraits::objectType* obj) {col->addElement(obj); }

  int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }

  void setRawHit(ILCInputTraits::calohitType* hit, ILCInputTraits::objectType* rawHit) { hit->setRawHit(rawHit); }
  float getEnergy(const ILCInputTraits::calohitType* hit) { return hit->getEnergy(); }
  void setEnergy(ILCInputTraits::calohitType* hit, float energy) { hit->setEnergy(energy); }
  int getCellID0(const ILCInputTraits::calohitType* hit) { return hit->getCellID0(); }
  int getCellID1(const ILCInputTraits::calohitType* hit) { return hit->getCellID1(); }
  void setTime(ILCInputTraits::calohitType* hit, float time) { hit->setTime(time); }

  void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
};

#endif