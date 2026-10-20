#ifndef SimDigitalGeom_h
#define SimDigitalGeom_h

#include <utility>

#include <marlin/Processor.h>

#include "CalorimeterHitType.h" //in MarlinUtil
#include "marlinutil/LCGeometryTypes.h"

#include "DD4hep/DD4hepUnits.h"
#include "DD4hep/DetType.h"
#include "DD4hep/Factories.h"
#include "DDRec/DDGear.h"
#include "DDRec/DetectorData.h"
#include "DDRec/DetectorSurfaces.h"
#include "DDRec/MaterialManager.h"

namespace AIDA {
class ITuple;
}

struct StepAndCharge;

struct PotentialSameTrackID {
  PotentialSameTrackID(int _pdgStep, int _pdgParent) : PDGStep{_pdgStep}, PDGParent{_pdgParent} {}

  int PDGStep;
  int PDGParent;

  bool operator<(const PotentialSameTrackID& b) const {
    return std::tie(PDGStep, PDGParent) < std::tie(b.PDGStep, b.PDGParent);
  }
  bool operator==(const PotentialSameTrackID& b) const {
    return std::tie(PDGStep, PDGParent) == std::tie(b.PDGStep, b.PDGParent);
  }
};

class SimDigitalGeomCellId_Base {
public:
  SimDigitalGeomCellId_Base();
  virtual ~SimDigitalGeomCellId_Base();

  void setCellSize(float size) { this->_cellSize = size; }
  virtual float getCellSize() = 0;

  int I() const { return _Iy; }
  int J() const { return _Jz; }
  int K() const { return _trueLayer; }
  int stave() const { return _stave; }
  int module() const { return _module; }
  int tower() const { return _tower; }

  const LCVector3D& normalToRPCPlane() const { return _normal; }
  const LCVector3D& Iaxis() const { return _Iaxis; }
  const LCVector3D& Jaxis() const { return _Jaxis; }

  const float* hitPosition() const { return _hitPosition; }

  CHT::Layout getCaloLayout() const { return _currentHCALCollectionCaloLayout; }

  SimDigitalGeomCellId_Base(const SimDigitalGeomCellId_Base& toCopy) = delete;
  void operator=(const SimDigitalGeomCellId_Base& toCopy) = delete;

protected:
  void linkSteps(std::vector<StepAndCharge>& vec);

  float _cellSize = 0.0f;

  int _trueLayer = -999;
  int _stave = -999;
  int _module = -999;
  int _tower = -999;
  int _Iy = -999;
  int _Jz = -999;

  LCVector3D _normal;
  LCVector3D _Iaxis;
  LCVector3D _Jaxis;

  const float* _hitPosition = nullptr;

  CHT::Layout _currentHCALCollectionCaloLayout = CHT::any;
};

template <typename InputTraits>
class SimDigitalGeomCellId : public SimDigitalGeomCellId_Base {
public:
  using collectionType =    typename InputTraits::collectionType;
  using collectionVecType = typename InputTraits::collectionVecType;
  using simcalohitType =    typename InputTraits::simcalohitType;
  using celliddecoderType = typename InputTraits::celliddecoderType;
  using calohitType =       typename InputTraits::calohitType;
  using cellidencoderType = typename InputTraits::cellidencoderType;

  SimDigitalGeomCellId(collectionType* inputCol, collectionVecType* outputCol);
  virtual ~SimDigitalGeomCellId();

  virtual void setLayerLayout(CHT::Layout layout) = 0;
  std::vector<StepAndCharge> decode(simcalohitType* hit, bool link);
  std::unique_ptr<calohitType> encode(int delta_I, int delta_J);

  SimDigitalGeomCellId(const SimDigitalGeomCellId& toCopy) = delete;
  void operator=(const SimDigitalGeomCellId& toCopy) = delete;

protected:
  virtual void setEncodingString() = 0;  
  virtual void processGeometry(simcalohitType* hit) = 0;
  void createStepAndChargeVec(simcalohitType* hit, std::vector<StepAndCharge>& vec, bool link);
  virtual bool isOffLimits(int Real_Iy, int Real_Jz) = 0;

  dd4hep::CellID _cellIDvalue = 0;
  celliddecoderType _decoder;
  cellidencoderType _encoder;
  std::string _cellIDEncodingString = "";
  std::vector<std::string> _encodingString;
};

template <typename InputTraits>
class SimDigitalGeomCellIdLCGEO : public SimDigitalGeomCellId<InputTraits> {
public:
  using collectionType = typename InputTraits::collectionType;
  using collectionVecType = typename InputTraits::collectionVecType;
  using simcalohitType = typename InputTraits::simcalohitType;
  using calohitType = typename InputTraits::calohitType;

  SimDigitalGeomCellIdLCGEO(collectionType* inputCol, collectionVecType* outputCol);
  virtual ~SimDigitalGeomCellIdLCGEO();

  virtual float getCellSize();
  virtual void setLayerLayout(CHT::Layout layout);

  SimDigitalGeomCellIdLCGEO(const SimDigitalGeomCellIdLCGEO& toCopy) = delete;
  void operator=(const SimDigitalGeomCellIdLCGEO& toCopy) = delete;

protected:
  virtual void setEncodingString() {this->_encodingString = {"layer", "stave", "module", "tower", "x", "y"};}
  virtual void processGeometry(simcalohitType* hit);
  virtual bool isOffLimits(int /*Real_Iy*/, int /*Real_Jz*/)  { return false; }

  dd4hep::rec::LayeredCalorimeterData* _caloData = nullptr;
};

template <typename InputTraits>
class SimDigitalGeomCellIdPROTO : public SimDigitalGeomCellId<InputTraits> {
public:
  using collectionType = typename InputTraits::collectionType;
  using collectionVecType = typename InputTraits::collectionVecType;
  using simcalohitType = typename InputTraits::simcalohitType;
  using calohitType = typename InputTraits::calohitType;

  SimDigitalGeomCellIdPROTO(collectionType* inputCol, collectionVecType* outputCol);
  virtual ~SimDigitalGeomCellIdPROTO();

  virtual float getCellSize() { return this->_cellSize; }
  virtual void setLayerLayout(CHT::Layout layout);

  SimDigitalGeomCellIdPROTO(const SimDigitalGeomCellIdPROTO& toCopy) = delete;
  void operator=(const SimDigitalGeomCellIdPROTO& toCopy) = delete;

protected:
  virtual void setEncodingString() {this->_encodingString = {"K-1", "", "", "", "I", "J"};}
  virtual void processGeometry(simcalohitType* hit);
  virtual bool isOffLimits(int Real_Iy, int Real_Jz)  { return Real_Iy < 0 || Real_Jz < 0; }
};

#endif // SimDigitalGeom_h
