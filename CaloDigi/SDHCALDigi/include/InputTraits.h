#ifndef InputTraits_h
#define InputTraits_h

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

struct WGGInputTraits {
  using eventType = EVENT::LCEvent;
  using collectionType = WGGCollection;
  using collectionVecType = WGGCollectionVec;
  using simcalohitType = WGGSimCalorimeterHit;
  using mcparticleType = WGGMCParticle;
  using celliddecoderType = WGGCellIDDecoder;
  using calohitType = WGGCalorimeterHit;
  using cellidencoderType = WGGCellIDEncoder;
  using flagType = IMPL::LCFlagImpl;
  using parametersType = WGGParameters;
  using relationType = WGGRelation;
};

namespace SimDigital_Data {

  //// ILCInputTraits ////

  // Event
  inline ILCInputTraits::collectionType* getCollection(const ILCInputTraits::eventType* evt, const std::string& name, ILCInputTraits*) { return evt->getCollection(name.c_str()); }
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
  inline lcio::long64 getValue(ILCInputTraits::celliddecoderType& decoder, ILCInputTraits::simcalohitType* simhit) { return decoder(simhit).getValue(); }
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
  inline void setValue(ILCInputTraits::cellidencoderType& encoder, lcio::long64 value) { encoder.setValue(value); }
  inline void setField(ILCInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder[fieldName] = value; }
  // Flag
  inline int getFlag(const ILCInputTraits::flagType& flag) { return flag.getFlag(); }
  // Parameters
  inline void setValue(ILCInputTraits::parametersType& params, const std::string& key, const std::string& value) { params.setValue(key, value); }
  inline const std::string& getStringVal(const ILCInputTraits::parametersType& params, const std::string& key) { return params.getStringVal(key); }
  // Relation
  inline void createRelation(ILCInputTraits::relationType* &ptr, ILCInputTraits::calohitType* calohit, ILCInputTraits::simcalohitType* simhit, float weight) { ptr = new ILCInputTraits::relationType(calohit, simhit, weight); }

  //// WGGInputTraits ////

  // Event
  inline WGGInputTraits::collectionType* getCollection(const WGGInputTraits::eventType* evt, const std::string& name, WGGInputTraits*) { return new WGGInputTraits::collectionType(*(evt->getCollection(name.c_str()))); }
  inline void addCollection(ILCInputTraits::eventType* evt, WGGInputTraits::collectionVecType* colVec, const std::string& name) { evt->addCollection(colVec->_colVec, name.c_str()); }
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
  inline lcio::long64 getValue(WGGInputTraits::celliddecoderType &decoder, WGGInputTraits::simcalohitType* simhit) { return decoder._decoder(simhit->_simhit).getValue(); }
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
  inline void setValue(WGGInputTraits::cellidencoderType& encoder, lcio::long64 value) { encoder._encoder.setValue(value); }
  inline void setField(WGGInputTraits::cellidencoderType& encoder, const std::string& fieldName, int value) { encoder._encoder[fieldName] = value; }
  // Flag
  // Parameters
  inline void setValue(WGGInputTraits::parametersType& parameters, const std::string& key, const std::string& value) { parameters._params->setValue(key, value); }
  inline const std::string& getStringVal(const WGGInputTraits::parametersType& parameters, const std::string& key) { return parameters._params->getStringVal(key); }
  // Relation
  inline void createRelation(WGGInputTraits::relationType* &ptr, WGGInputTraits::calohitType* calohit, WGGInputTraits::simcalohitType* simhit, float weight) { ptr = new WGGInputTraits::relationType(calohit->_calohit.get(), simhit->_simhit, weight); }
};

#endif