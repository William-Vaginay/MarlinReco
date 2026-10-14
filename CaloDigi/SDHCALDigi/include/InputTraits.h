#ifndef InputTraits_h
#define InputTraits_h

#include "EVENT/LCEvent.h"
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/SimCalorimeterHit.h>
#include <EVENT/MCParticle.h>
#include <UTIL/CellIDDecoder.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <UTIL/CellIDEncoder.h>
#include <IMPL/LCFlagImpl.h>
#include <EVENT/LCParameters.h>
#include <EVENT/LCGenericObject.h>
#include <IMPL/LCRelationImpl.h>

struct ILCInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = EVENT::SimCalorimeterHit;
  using mcparticleType = EVENT::MCParticle;
  using celliddecoderType = UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>;
  using calohitType = IMPL::CalorimeterHitImpl;
  using cellidencoderType = UTIL::CellIDEncoder<IMPL::CalorimeterHitImpl>;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = EVENT::LCParameters;
  using objectType = EVENT::LCObject;
  using relationType = IMPL::LCRelationImpl;
};

struct WGGSimCalorimeterHit {
  WGGSimCalorimeterHit(EVENT::SimCalorimeterHit &simhit) : _simhit(&simhit) {}
  EVENT::SimCalorimeterHit *_simhit;
};

struct WGGMCParticle {
  WGGMCParticle(EVENT::MCParticle &mcparticle) : _mcparticle(&mcparticle) {}
  EVENT::MCParticle *_mcparticle;
};

struct WGGRelation {
  WGGRelation(ILCInputTraits::calohitType* caloHit, ILCInputTraits::simcalohitType* simHit, float weight) : _relation(new ILCInputTraits::relationType(caloHit, simHit, weight)) {}
  IMPL::LCRelationImpl *_relation;
};

struct WGGCellIDDecoder {
  WGGCellIDDecoder(EVENT::LCCollection* col) : _decoder(col) {}
  UTIL::CellIDDecoder<EVENT::SimCalorimeterHit> _decoder;
};

struct WGGCalorimeterHit {
  WGGCalorimeterHit(IMPL::CalorimeterHitImpl* calohit) : _calohit(calohit) {}
  std::unique_ptr<IMPL::CalorimeterHitImpl> _calohit;
};

struct WGGCellIDEncoder {
  WGGCellIDEncoder(const std::string& cellIDEncoding, EVENT::LCCollection* col) : _encoder(cellIDEncoding, col) {}
  UTIL::CellIDEncoder<IMPL::CalorimeterHitImpl> _encoder;
};

struct WGGInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = EVENT::LCCollection;
  using collectionVecType = IMPL::LCCollectionVec;
  using simcalohitType = WGGSimCalorimeterHit;
  using mcparticleType = WGGMCParticle;
  using celliddecoderType = WGGCellIDDecoder;
  using calohitType = WGGCalorimeterHit;
  using cellidencoderType = WGGCellIDEncoder;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = EVENT::LCParameters;
  using relationType = WGGRelation;
};

namespace SimDigital_Data {

  //// ILCInputTraits ////

  // Event
  inline ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name) { return evt->getCollection(name.c_str()); }
  inline void addCollection(ILCInputTraits::eventType* evt, ILCInputTraits::collectionVecType* col, const std::string& name) { evt->addCollection(col, name.c_str()); }
  // Collection
  inline int getNumberOfElements(const ILCInputTraits::collectionType* col) { return col->getNumberOfElements(); }
  inline ILCInputTraits::simcalohitType* getElementAt(const ILCInputTraits::collectionType* col, int index, ILCInputTraits*) { return dynamic_cast<ILCInputTraits::simcalohitType*>(col->getElementAt(index)); }
  // CollectionVec
  inline void setFlag(ILCInputTraits::collectionVecType* col, const int& flag) { col->setFlag(flag); }
  inline ILCInputTraits::parametersType& parameters(ILCInputTraits::collectionVecType* col) { return col->parameters(); }
  inline void  addElement(ILCInputTraits::collectionVecType* col, ILCInputTraits::objectType* obj) {col->addElement(obj); }
  // SimCalorimeterHit
  inline const float* getPosition(const ILCInputTraits::simcalohitType* hit) { return hit->getPosition(); }
  inline int getCellID0(const ILCInputTraits::simcalohitType* hit) { return hit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::simcalohitType* hit) { return hit->getCellID1(); }
  inline int getNMCContributions(const ILCInputTraits::simcalohitType* hit) { return hit->getNMCContributions(); }
  inline float getTimeCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getTimeCont(index); }
  inline int getPDGCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getPDGCont(index); }
  inline ILCInputTraits::mcparticleType* getParticleCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getParticleCont(index); }
  inline const float* getStepPosition(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getStepPosition(index); }
  inline float getLengthCont(const ILCInputTraits::simcalohitType* hit, int index) { return hit->getLengthCont(index); }
  // MCParticle
  inline int getPDG(const ILCInputTraits::mcparticleType* particle) {return particle->getPDG(); }
  // CellIDDecoder
  inline lcio::long64 getValue(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* hit) { return decoder(hit).getValue(); }
  inline int getField(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* hit, const std::string& fieldName) { return decoder(hit)[fieldName]; }
  // CalorimeterHit
  inline void setRawHit(ILCInputTraits::calohitType* hit, ILCInputTraits::simcalohitType* rawHit) { hit->setRawHit(rawHit); }
  inline float getEnergy(const ILCInputTraits::calohitType* hit) { return hit->getEnergy(); }
  inline void setEnergy(ILCInputTraits::calohitType* hit, float energy) { hit->setEnergy(energy); }
  inline int getCellID0(const ILCInputTraits::calohitType* hit) { return hit->getCellID0(); }
  inline int getCellID1(const ILCInputTraits::calohitType* hit) { return hit->getCellID1(); }
  inline void setTime(ILCInputTraits::calohitType* hit, float time) { hit->setTime(time); }
  inline void setType(ILCInputTraits::calohitType* hit, int type) { hit->setType(type); }
  inline void setPosition(ILCInputTraits::calohitType* hit, const float pos[3]) { hit->setPosition(pos); }
  inline void createCaloHit(std::unique_ptr<ILCInputTraits::calohitType> &ptr) { ptr = std::make_unique<ILCInputTraits::calohitType>(); }
  // CellIDEncoder
  inline void setcellID(ILCInputTraits::cellidencoderType& encoder, ILCInputTraits::calohitType* hit) { encoder.setCellID(hit); }
  inline void setValue(ILCInputTraits::cellidencoderType& encoder, lcio::long64 value) { encoder.setValue(value); }
  inline void setField(ILCInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder[fieldName] = value; }
  // Flag
  inline int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }
  // Parameters
  inline void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
  // Relation
  inline void createRelation(ILCInputTraits::relationType* &ptr, ILCInputTraits::calohitType* caloHit, ILCInputTraits::simcalohitType* simHit, float weight) { ptr = new ILCInputTraits::relationType(caloHit, simHit, weight); }

  //// WGGInputTraits ////

  // Event
  inline WGGInputTraits::simcalohitType* getElementAt(const WGGInputTraits::collectionType* col, int index, WGGInputTraits*) { return new WGGInputTraits::simcalohitType (*dynamic_cast<EVENT::SimCalorimeterHit*>(col->getElementAt(index))); }
  // Collection
  // CollectionVec
  inline void addElement(WGGInputTraits::collectionVecType* col, WGGInputTraits::relationType* rel) {col->addElement(rel->_relation); }
  inline void addElement(WGGInputTraits::collectionVecType* col, WGGInputTraits::calohitType* caloHit) {col->addElement(caloHit->_calohit.get()); }
  // SimCalorimeterHit
  inline const float* getPosition(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getPosition(); }
  inline int getCellID0(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getCellID0(); }
  inline int getCellID1(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getCellID1(); }
  inline int getNMCContributions(const WGGInputTraits::simcalohitType* hit) { return hit->_simhit->getNMCContributions(); }
  inline float getTimeCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getTimeCont(index); }
  inline int getPDGCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getPDGCont(index); }
  inline WGGInputTraits::mcparticleType* getParticleCont(const WGGInputTraits::simcalohitType* hit, int index) { return new WGGInputTraits::mcparticleType(*(hit->_simhit->getParticleCont(index))); }
  inline const float* getStepPosition(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getStepPosition(index); }
  inline float getLengthCont(const WGGInputTraits::simcalohitType* hit, int index) { return hit->_simhit->getLengthCont(index); } 
  // MCParticle
  inline int getPDG(const WGGInputTraits::mcparticleType* particle) {return particle->_mcparticle->getPDG(); }
  // CellIDDecoder
  inline lcio::long64 getValue(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* hit) { return decoder._decoder(hit->_simhit).getValue(); }
  inline int getField(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* hit, const std::string& fieldName) { return decoder._decoder(hit->_simhit)[fieldName]; }
  // CalorimeterHit
  inline void setRawHit(WGGInputTraits::calohitType* hit, WGGInputTraits::simcalohitType* rawHit) { hit->_calohit->setRawHit(rawHit->_simhit); }
  inline float getEnergy(const WGGInputTraits::calohitType* hit) { return hit->_calohit->getEnergy(); }
  inline void setEnergy(WGGInputTraits::calohitType* hit, float energy) { hit->_calohit->setEnergy(energy); }
  inline int getCellID0(const WGGInputTraits::calohitType* hit) { return hit->_calohit->getCellID0(); }
  inline int getCellID1(const WGGInputTraits::calohitType* hit) { return hit->_calohit->getCellID1(); }
  inline void setTime(WGGInputTraits::calohitType* hit, float time) { hit->_calohit->setTime(time); }
  inline void setType(WGGInputTraits::calohitType* hit, int type) { hit->_calohit->setType(type); }
  inline void setPosition(WGGInputTraits::calohitType* hit, const float pos[3]) { hit->_calohit->setPosition(pos); }
  inline void createCaloHit(std::unique_ptr<WGGInputTraits::calohitType> &ptr) { 
    IMPL::CalorimeterHitImpl* caloHit = new IMPL::CalorimeterHitImpl(); 
    WGGInputTraits::calohitType *whit = new WGGInputTraits::calohitType(caloHit); 
    caloHit = nullptr;
    std::unique_ptr<WGGInputTraits::calohitType> p(whit);
    whit = nullptr;
    ptr = std::move(p); }
  // CellIDEncoder
  inline void setcellID(WGGInputTraits::cellidencoderType& encoder, WGGInputTraits::calohitType* hit) { encoder._encoder.setCellID(hit->_calohit.get()); }
  inline void setValue(WGGInputTraits::cellidencoderType& encoder, lcio::long64 value) { encoder._encoder.setValue(value); }
  inline void setField(WGGInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder._encoder[fieldName] = value; }
  // Relation
  inline void createRelation(WGGInputTraits::relationType* &ptr, WGGInputTraits::calohitType* calorimeterHit, WGGInputTraits::simcalohitType* simHit, float weight) { ptr = new WGGInputTraits::relationType(calorimeterHit->_calohit.get(), simHit->_simhit, weight); }
};

#endif