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
  inline ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  inline void addCollection(ILCInputTraits::eventType* evt, ILCInputTraits::collectionVecType* col, const std::string& name) { evt->addCollection(col, name.c_str()); }

  inline int getNumberOfElements(const ILCInputTraits::collectionType* col) { return col->getNumberOfElements(); }
  inline ILCInputTraits::simcalohitType* getElementAt(const ILCInputTraits::collectionType* col, int index) { return dynamic_cast<ILCInputTraits::simcalohitType*>(col->getElementAt(index)); }

  inline void setFlag(ILCInputTraits::collectionVecType* col, const int& flag) { col->setFlag(flag); }
  inline ILCInputTraits::parametersType& parameters(ILCInputTraits::collectionVecType* col) { return col->parameters(); }
  inline void addElement(ILCInputTraits::collectionVecType* col, ILCInputTraits::objectType* obj) {col->addElement(obj); }

  inline int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }

  inline void setRawHit(ILCInputTraits::calohitType* hit, ILCInputTraits::objectType* rawHit) { hit->setRawHit(rawHit); }
  inline float getEnergy(const ILCInputTraits::calohitType* hit) { return hit->getEnergy(); }
  inline void setEnergy(ILCInputTraits::calohitType* hit, float energy) { hit->setEnergy(energy); }
  inline int getCellID0(const ILCInputTraits::calohitType* hit) { return hit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::calohitType* hit) { return hit->getCellID1(); }
  inline void setTime(ILCInputTraits::calohitType* hit, float time) { hit->setTime(time); }

  inline void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
};

#endif