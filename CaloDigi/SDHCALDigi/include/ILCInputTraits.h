#ifndef ILCInputTraits_h
#define ILCInputTraits_h

#include "EVENT/LCEvent.h"
#include <EVENT/LCParameters.h>
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/SimCalorimeterHit.h>
#include <EVENT/MCParticle.h>
#include <UTIL/CellIDDecoder.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <UTIL/CellIDEncoder.h>
#include <IMPL/LCFlagImpl.h>
#include <EVENT/LCGenericObject.h>
#include <IMPL/LCRelationImpl.h>
#include "lcio.h"

struct ILCInputTraits {
  using eventType = EVENT::LCEvent;
  using parametersType = EVENT::LCParameters;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = EVENT::SimCalorimeterHit;
  using mcparticleType = EVENT::MCParticle;
  using celliddecoderType = UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>;
  using calohitType = IMPL::CalorimeterHitImpl;
  using cellidencoderType = UTIL::CellIDEncoder<IMPL::CalorimeterHitImpl>;
  using flagType = IMPL::LCFlagImpl;
  using objectType = EVENT::LCObject;
  using relationType = IMPL::LCRelationImpl;
  using long64Type = lcio::long64;
  using exceptionType = lcio::Exception;
};

namespace SimDigital_Data {
  // Event
  inline ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  inline void addCollection(ILCInputTraits::eventType* evt, ILCInputTraits::collectionVecType* colVec, const std::string& name) { evt->addCollection(colVec, name.c_str()); }
  // Collection
  inline int getNumberOfElements(const ILCInputTraits::collectionType* col) { return col->getNumberOfElements(); }
  inline ILCInputTraits::simcalohitType* getElementAt(const ILCInputTraits::collectionType* col, int index) { return dynamic_cast<ILCInputTraits::simcalohitType*>(col->getElementAt(index)); }
  inline const ILCInputTraits::parametersType& getParameters(const ILCInputTraits::collectionType* col) { return col->getParameters(); }
  // CollectionVec
  inline int getNumberOfElements(const ILCInputTraits::collectionVecType* colVec) { return colVec->getNumberOfElements(); }
  inline void setFlag(ILCInputTraits::collectionVecType* colVec, const int& flag) { colVec->setFlag(flag); }
  inline ILCInputTraits::parametersType& parameters(ILCInputTraits::collectionVecType* colVec) { return colVec->parameters(); }
  inline void addElement(ILCInputTraits::collectionVecType* colVec, ILCInputTraits::objectType* obj) {colVec->addElement(obj); }
  inline void createcollectionVec(ILCInputTraits::collectionVecType* &ptr, const std::string& type) { ptr = new ILCInputTraits::collectionVecType(type);}
  // SimCalorimeterHit
  inline const float* getPosition(const ILCInputTraits::simcalohitType* simhit) { return simhit->getPosition(); }
  inline int getCellID0(const ILCInputTraits::simcalohitType* simhit) { return simhit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::simcalohitType* simhit) { return simhit->getCellID1(); }
  inline int getNMCContributions(const ILCInputTraits::simcalohitType* simhit) { return simhit->getNMCContributions(); }
  inline float getTimeCont(const ILCInputTraits::simcalohitType* simhit, int index) { return simhit->getTimeCont(index); }
  inline int getPDGCont(const ILCInputTraits::simcalohitType* simhit, int index) { return simhit->getPDGCont(index); }
  inline ILCInputTraits::mcparticleType* getParticleCont(const ILCInputTraits::simcalohitType* simhit, int index) { return simhit->getParticleCont(index); }
  inline const float* getStepPosition(const ILCInputTraits::simcalohitType* simhit, int index) { return simhit->getStepPosition(index); }
  inline float getLengthCont(const ILCInputTraits::simcalohitType* simhit, int index) { return simhit->getLengthCont(index); }
  // MCParticle
  inline int getPDG(const ILCInputTraits::mcparticleType* particle) {return particle->getPDG(); }
  // CellIDDecoder
  inline ILCInputTraits::long64Type getValue(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* simhit) { return decoder(simhit).getValue(); }
  inline int getField(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* simhit, const std::string& fieldName) { return decoder(simhit)[fieldName]; }
  // CalorimeterHit
  inline void setRawHit(ILCInputTraits::calohitType* calohit, ILCInputTraits::simcalohitType* rawHit) { calohit->setRawHit(rawHit); }
  inline float getEnergy(const ILCInputTraits::calohitType* calohit) { return calohit->getEnergy(); }
  inline void setEnergy(ILCInputTraits::calohitType* calohit, float energy) { calohit->setEnergy(energy); }
  inline int getCellID0(const ILCInputTraits::calohitType* calohit) { return calohit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::calohitType* calohit) { return calohit->getCellID1(); }
  inline void setTime(ILCInputTraits::calohitType* calohit, float time) { calohit->setTime(time); }
  inline void setType(ILCInputTraits::calohitType* calohit, int type) { calohit->setType(type); }
  inline void setPosition(ILCInputTraits::calohitType* calohit, const float pos[3]) { calohit->setPosition(pos); }
  inline void createCaloHit(std::unique_ptr<ILCInputTraits::calohitType> &ptr) { ptr = std::make_unique<ILCInputTraits::calohitType>(); }
  // CellIDEncoder
  inline void setcellID(ILCInputTraits::cellidencoderType& encoder, ILCInputTraits::calohitType* calohit) { encoder.setCellID(calohit); }
  inline void setValue(ILCInputTraits::cellidencoderType& encoder, ILCInputTraits::long64Type value) { encoder.setValue(value); }
  inline void setField(ILCInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder[fieldName] = value; }
  // Flag
  inline void setBit(ILCInputTraits::flagType& flag, int bit) { flag.setBit(bit); }
  inline int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }
  // Parameters
  inline void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
  inline const std::string& getStringVal(const ILCInputTraits::parametersType& params, const std::string& key) { return params.getStringVal(key); }
  // Relation
  inline void createRelation(ILCInputTraits::relationType* &ptr, ILCInputTraits::calohitType* calohit, ILCInputTraits::simcalohitType* simhit, float weight) { ptr = new ILCInputTraits::relationType(calohit, simhit, weight); }
  // CellIdEncoding
  inline const char* getHiCellIDEncodingString() { return lcio::LCIO::CellIDEncoding; }
};

#endif