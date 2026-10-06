#ifndef InputTraits_h
#define InputTraits_h

#include "EVENT/LCEvent.h"
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/SimCalorimeterHit.h>
#include <UTIL/CellIDDecoder.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <UTIL/CellIDEncoder.h>
#include <IMPL/LCFlagImpl.h>
#include <EVENT/LCParameters.h>
#include <IMPL/LCRelationImpl.h>

struct ILCInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = EVENT::SimCalorimeterHit;
  using mcparticleType = EVENT::MCParticle;
  using celliddecoderType = UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>;
  using calohitType = IMPL::CalorimeterHitImpl;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = EVENT::LCParameters;
  using objectType = EVENT::LCObject;
};

struct WGGSimCalorimeterHit {
  WGGSimCalorimeterHit(EVENT::SimCalorimeterHit &simhit) : _simhit(&simhit) {}
  EVENT::SimCalorimeterHit *_simhit;
};

struct WGGMCParticle {
  WGGMCParticle(EVENT::MCParticle &mcparticle) : _mcparticle(&mcparticle) {}
  EVENT::MCParticle *_mcparticle;
};

/*struct WGGCellIDDecoder {
  WGGCellIDDecoder(UTIL::CellIDDecoder<EVENT::SimCalorimeterHit> &decoder) : _decoder(&decoder) {}
  UTIL::CellIDDecoder<EVENT::SimCalorimeterHit> *_decoder;
};*/

/*struct WGGCellIDDecoder {
  WGGCellIDDecoder(EVENT::LCCollection *col) : _decoder(col) {}
  UTIL::CellIDDecoder<EVENT::SimCalorimeterHit> _decoder;
};*/

struct WGGInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  //using simcalohitType = EVENT::SimCalorimeterHit;
  using simcalohitType = WGGSimCalorimeterHit;
  using mcparticleType = WGGMCParticle;
  using celliddecoderType = UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>;
 // using celliddecoderType = WGGCellIDDecoder;
  using calohitType = IMPL::CalorimeterHitImpl;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = EVENT::LCParameters;
  using objectType = EVENT::LCObject;
};

namespace SimDigital_Data {

  // For ILCInputTraits

  inline ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  inline void addCollection(ILCInputTraits::eventType* evt, ILCInputTraits::collectionVecType* col, const std::string& name) { evt->addCollection(col, name.c_str()); }

  inline int getNumberOfElements(const ILCInputTraits::collectionType* col) { return col->getNumberOfElements(); }
  inline ILCInputTraits::simcalohitType* getElementAt(const ILCInputTraits::collectionType* col, int index) { return dynamic_cast<ILCInputTraits::simcalohitType*>(col->getElementAt(index)); }

  inline void setFlag(ILCInputTraits::collectionVecType* col, const int& flag) { col->setFlag(flag); }
  inline ILCInputTraits::parametersType& parameters(ILCInputTraits::collectionVecType* col) { return col->parameters(); }
  inline void addElement(ILCInputTraits::collectionVecType* col, ILCInputTraits::objectType* obj) {col->addElement(obj); }

  inline const float* getPosition(const ILCInputTraits::simcalohitType* hit) { return hit->getPosition(); }
  inline int getCellID0(const ILCInputTraits::simcalohitType* hit) { return hit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::simcalohitType* hit) { return hit->getCellID1(); }
  inline int getNMCContributions(const ILCInputTraits::simcalohitType* hit) { return hit->getNMCContributions(); }
  inline float getTimeCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getTimeCont(index); }
  inline int getPDGCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getPDGCont(index); }
  inline ILCInputTraits::mcparticleType* getParticleCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getParticleCont(index); }
  inline const float* getStepPosition(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getStepPosition(index); }
  inline float getLengthCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getLengthCont(index); }

  inline int getPDG(const ILCInputTraits::mcparticleType* particle) {return particle->getPDG(); }

  inline lcio::long64 getValue(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* hit) { return decoder(hit).getValue(); }
  inline int getField(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* hit, const std::string& fieldName) { return decoder(hit)[fieldName]; }

  inline void setRawHit(ILCInputTraits::calohitType* hit, ILCInputTraits::objectType* rawHit) { hit->setRawHit(rawHit); }
  inline float getEnergy(const ILCInputTraits::calohitType* hit) { return hit->getEnergy(); }
  inline void setEnergy(ILCInputTraits::calohitType* hit, float energy) { hit->setEnergy(energy); }
  inline int getCellID0(const ILCInputTraits::calohitType* hit) { return hit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::calohitType* hit) { return hit->getCellID1(); }
  inline void setTime(ILCInputTraits::calohitType* hit, float time) { hit->setTime(time); }

  inline int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }

  inline void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }

  // For WGGInputTraits

  inline const float* getPosition(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getPosition(); }
  inline int getCellID0(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getCellID0(); }
  inline int getCellID1(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getCellID1(); }
  inline int getNMCContributions(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getNMCContributions(); }
  inline float getTimeCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getTimeCont(index); }
  inline int getPDGCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getPDGCont(index); }
  //inline WGGInputTraits::mcparticleType* getParticleCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getParticleCont(index); }
  inline ILCInputTraits::mcparticleType* getParticleCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getParticleCont(index); }
  inline const float* getStepPosition(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getStepPosition(index); }
  inline float getLengthCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getLengthCont(index); } 

  inline int getPDG(const WGGInputTraits::mcparticleType* particle) {return particle->_mcparticle->getPDG(); }
 
  inline lcio::long64 getValue(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* hit) { return decoder(hit->_simhit).getValue(); }
  inline int getField(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* hit, const std::string& fieldName) { return decoder(hit->_simhit)[fieldName]; }
};


#endif