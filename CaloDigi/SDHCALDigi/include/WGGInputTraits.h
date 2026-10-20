#ifndef WGGInputTraits_h
#define WGGInputTraits_h

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

struct WGGEvent {
  WGGEvent() : _evt(nullptr) {}
  EVENT::LCEvent *_evt;
};

struct WGGParameters {
  WGGParameters(EVENT::LCParameters &params) : _params(&params) {}
  EVENT::LCParameters *_params;
};

struct WGGCollection {
  WGGCollection(EVENT::LCCollection &col) : _col(&col), _parameters(_col->parameters()) {}
  EVENT::LCCollection *_col;
  WGGParameters _parameters;
};

struct WGGCollectionVec {
  WGGCollectionVec(const std::string& type) : _colVec(new IMPL::LCCollectionVec(type)), _parameters(_colVec->parameters()) {}
  IMPL::LCCollectionVec *_colVec;
  WGGParameters _parameters;
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
  WGGRelation(IMPL::CalorimeterHitImpl* caloHit, EVENT::SimCalorimeterHit* simHit, float weight) : _relation(new IMPL::LCRelationImpl(caloHit, simHit, weight)) {}
  IMPL::LCRelationImpl *_relation;
};

struct WGGCellIDDecoder {
  WGGCellIDDecoder(WGGCollection* col) : _decoder(col->_col) {}
  UTIL::CellIDDecoder<EVENT::SimCalorimeterHit> _decoder;
};

struct WGGCalorimeterHit {
  WGGCalorimeterHit(IMPL::CalorimeterHitImpl* calohit) : _calohit(calohit) {}
  std::unique_ptr<IMPL::CalorimeterHitImpl> _calohit;
};

struct WGGCellIDEncoder {
  WGGCellIDEncoder(const std::string& cellIDEncoding, WGGCollectionVec* colVec) : _encoder(cellIDEncoding, colVec->_colVec) {}
  UTIL::CellIDEncoder<IMPL::CalorimeterHitImpl> _encoder;
};

struct WGGFlag {
  WGGFlag() : _flag() {}
  IMPL::LCFlagImpl _flag;
};

struct WGGInputTraits {
  using eventType = WGGEvent;
  using collectionType = WGGCollection;
  using collectionVecType = WGGCollectionVec;
  using simcalohitType = WGGSimCalorimeterHit;
  using mcparticleType = WGGMCParticle;
  using celliddecoderType = WGGCellIDDecoder;
  using calohitType = WGGCalorimeterHit;
  using cellidencoderType = WGGCellIDEncoder;
  using flagType = WGGFlag;
  using parametersType = WGGParameters;
  using relationType = WGGRelation;
  using long64Type = lcio::long64;
  using exceptionType = lcio::Exception;
};

namespace SimDigital_Data {
  // Event
  inline WGGInputTraits::collectionType* getCollection(const WGGInputTraits::eventType* evt, const std::string& name) { return new WGGInputTraits::collectionType(*(evt->_evt->getCollection(name.c_str()))); }
  inline void addCollection(WGGInputTraits::eventType* evt, WGGInputTraits::collectionVecType* colVec, const std::string& name) { evt->_evt->addCollection(colVec->_colVec, name.c_str()); }
  // Collection
  inline int getNumberOfElements(const WGGInputTraits::collectionType* col) { return col->_col->getNumberOfElements(); }
  inline WGGInputTraits::simcalohitType* getElementAt(const WGGInputTraits::collectionType* col, int index) { return new WGGInputTraits::simcalohitType(*dynamic_cast<EVENT::SimCalorimeterHit*>(col->_col->getElementAt(index))); }
  inline const WGGInputTraits::parametersType& getParameters(const WGGInputTraits::collectionType* col) { return col->_parameters; }
  // CollectionVec
  inline int getNumberOfElements(const WGGInputTraits::collectionVecType* colVec) { return colVec->_colVec->getNumberOfElements(); }
  inline void setFlag(WGGInputTraits::collectionVecType* colVec, const int& flag) { colVec->_colVec->setFlag(flag); }
  inline WGGInputTraits::parametersType& parameters(WGGInputTraits::collectionVecType* colVec) { return colVec->_parameters; }
  inline void addElement(WGGInputTraits::collectionVecType* colVec, WGGInputTraits::relationType* rel) {colVec->_colVec->addElement(rel->_relation); }
  inline void addElement(WGGInputTraits::collectionVecType* colVec, WGGInputTraits::calohitType* calohit) {colVec->_colVec->addElement(calohit->_calohit.get()); }
  inline void createcollectionVec(WGGInputTraits::collectionVecType* &ptr, const std::string& type) { ptr = new WGGInputTraits::collectionVecType(type);}
  // SimCalorimeterHit
  inline const float* getPosition(const WGGInputTraits::simcalohitType* simhit) { return simhit->_simhit->getPosition(); }
  inline int getCellID0(const WGGInputTraits::simcalohitType* simhit) { return simhit->_simhit->getCellID0(); }
  inline int getCellID1(const WGGInputTraits::simcalohitType* simhit) { return simhit->_simhit->getCellID1(); }
  inline int getNMCContributions(const WGGInputTraits::simcalohitType* simhit) { return simhit->_simhit->getNMCContributions(); }
  inline float getTimeCont(const WGGInputTraits::simcalohitType* simhit, int index) { return simhit->_simhit->getTimeCont(index); }
  inline int getPDGCont(const WGGInputTraits::simcalohitType* simhit, int index) { return simhit->_simhit->getPDGCont(index); }
  inline WGGInputTraits::mcparticleType* getParticleCont(const WGGInputTraits::simcalohitType* simhit, int index) { return new WGGInputTraits::mcparticleType(*(simhit->_simhit->getParticleCont(index))); }
  inline const float* getStepPosition(const WGGInputTraits::simcalohitType* simhit, int index) { return simhit->_simhit->getStepPosition(index); }
  inline float getLengthCont(const WGGInputTraits::simcalohitType* simhit, int index) { return simhit->_simhit->getLengthCont(index); } 
  // MCParticle
  inline int getPDG(const WGGInputTraits::mcparticleType* particle) {return particle->_mcparticle->getPDG(); }
  // CellIDDecoder
  inline WGGInputTraits::long64Type getValue(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* simhit) { return decoder._decoder(simhit->_simhit).getValue(); }
  inline int getField(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* simhit, const std::string& fieldName) { return decoder._decoder(simhit->_simhit)[fieldName]; }
  // CalorimeterHit
  inline void setRawHit(WGGInputTraits::calohitType* calohit, WGGInputTraits::simcalohitType* rawHit) { calohit->_calohit->setRawHit(rawHit->_simhit); }
  inline float getEnergy(const WGGInputTraits::calohitType* calohit) { return calohit->_calohit->getEnergy(); }
  inline void setEnergy(WGGInputTraits::calohitType* calohit, float energy) { calohit->_calohit->setEnergy(energy); }
  inline int getCellID0(const WGGInputTraits::calohitType* calohit) { return calohit->_calohit->getCellID0(); }
  inline int getCellID1(const WGGInputTraits::calohitType* calohit) { return calohit->_calohit->getCellID1(); }
  inline void setTime(WGGInputTraits::calohitType* calohit, float time) { calohit->_calohit->setTime(time); }
  inline void setType(WGGInputTraits::calohitType* calohit, int type) { calohit->_calohit->setType(type); }
  inline void setPosition(WGGInputTraits::calohitType* calohit, const float pos[3]) { calohit->_calohit->setPosition(pos); }
  inline void createCaloHit(std::unique_ptr<WGGInputTraits::calohitType> &ptr) { 
    IMPL::CalorimeterHitImpl* caloHit = new IMPL::CalorimeterHitImpl(); 
    WGGInputTraits::calohitType *whit = new WGGInputTraits::calohitType(caloHit); 
    caloHit = nullptr;
    std::unique_ptr<WGGInputTraits::calohitType> p(whit);
    whit = nullptr;
    ptr = std::move(p); }
  // CellIDEncoder
  inline void setcellID(WGGInputTraits::cellidencoderType& encoder, WGGInputTraits::calohitType* calohit) { encoder._encoder.setCellID(calohit->_calohit.get()); }
  inline void setValue(WGGInputTraits::cellidencoderType& encoder, WGGInputTraits::long64Type value) { encoder._encoder.setValue(value); }
  inline void setField(WGGInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder._encoder[fieldName] = value; }
  // Flag
  inline void setBit(WGGInputTraits::flagType& flag, int bit) { flag._flag.setBit(bit); }
  inline int getFlag(const WGGInputTraits::flagType& flag) { return flag._flag.getFlag(); }
  // Parameters
  inline void setValue(WGGInputTraits::parametersType& parameters, const std::string& key, const std::string& value) { parameters._params->setValue(key, value); }
  inline const std::string& getStringVal(const WGGInputTraits::parametersType& parameters, const std::string& key) { return parameters._params->getStringVal(key); }
  // Relation
  inline void createRelation(WGGInputTraits::relationType* &ptr, WGGInputTraits::calohitType* calohit, WGGInputTraits::simcalohitType* simhit, float weight) { ptr = new WGGInputTraits::relationType(calohit->_calohit.get(), simhit->_simhit, weight); }
  // CellIdEncoding
};

#endif