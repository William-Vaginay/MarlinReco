#include "SimDigitalGeom.h"

#include "SimDigital.h"

#include <AIDA/ITuple.h>
#include <AIDA/ITupleFactory.h>
#include <marlin/AIDAProcessor.h>
#include <marlin/Global.h>

#include <DD4hep/DetectorSelector.h>
#include <DDRec/CellIDPositionConverter.h>
#include <DDSegmentation/BitField64.h>

#include <limits>

using namespace lcio;
using namespace marlin;

SimDigitalGeomCellId_Base::SimDigitalGeomCellId_Base() : _normal(), _Iaxis(), _Jaxis() {}

SimDigitalGeomCellId_Base::~SimDigitalGeomCellId_Base() {}

void SimDigitalGeomCellId_Base::linkSteps(std::vector<StepAndCharge>& vec) {
  if (vec.size() < 2)
    return;

  float time = std::numeric_limits<float>::max();
  float totalLength = 0;
  LCVector3D pos(0, 0, 0);

  for (const auto& step : vec) {
    totalLength += step.stepLength;
    pos += step.step * step.stepLength;
    time = std::min(time, step.time);
  }
  pos /= totalLength;

  totalLength = static_cast<float>((vec.front().step - vec.back().step).mag() +
                                   0.5f * (vec.front().stepLength + vec.back().stepLength));

  vec.clear();
  vec.push_back(StepAndCharge(pos, totalLength, time));
}

template <typename InputTraits>
SimDigitalGeomCellId<InputTraits>::SimDigitalGeomCellId(LCCollection* inputCol, LCCollectionVec* outputCol)
    : SimDigitalGeomCellId_Base(), _decoder(inputCol), _encoder(inputCol->getParameters().getStringVal(LCIO::CellIDEncoding), outputCol) {
  outputCol->parameters().setValue(LCIO::CellIDEncoding, inputCol->getParameters().getStringVal(LCIO::CellIDEncoding));
  _cellIDEncodingString = inputCol->getParameters().getStringVal(LCIO::CellIDEncoding);
}

template <typename InputTraits>
SimDigitalGeomCellIdLCGEO<InputTraits>::SimDigitalGeomCellIdLCGEO(LCCollection* inputCol, LCCollectionVec* outputCol)
    : SimDigitalGeomCellId<InputTraits>(inputCol, outputCol)
//	  theDetector()
{
  streamlog_out(DEBUG) << "we will use lcgeo!" << std::endl;
}

template <typename InputTraits>
SimDigitalGeomCellIdPROTO<InputTraits>::SimDigitalGeomCellIdPROTO(LCCollection* inputCol, LCCollectionVec* outputCol)
    : SimDigitalGeomCellId<InputTraits>(inputCol, outputCol)
//	  theDetector()
{
  streamlog_out(DEBUG) << "we will use proto!" << std::endl;

  this->_normal.set(0, 0, 1);
  this->_Iaxis.set(1, 0, 0);
  this->_Jaxis.set(0, 1, 0);

  this->_currentHCALCollectionCaloLayout = CHT::endcap;
}

template <typename InputTraits>
SimDigitalGeomCellId<InputTraits>::~SimDigitalGeomCellId() {}

template <typename InputTraits>
SimDigitalGeomCellIdLCGEO<InputTraits>::~SimDigitalGeomCellIdLCGEO() {}

template <typename InputTraits>
SimDigitalGeomCellIdPROTO<InputTraits>::~SimDigitalGeomCellIdPROTO() {}

template <typename InputTraits>
void SimDigitalGeomCellId<InputTraits>::createStepAndChargeVec(SimCalorimeterHit* hit, std::vector<StepAndCharge>& vec, bool link) {
  LCVector3D hitPos;
  if (NULL != _hitPosition)
    hitPos.set(_hitPosition[0], _hitPosition[1], _hitPosition[2]);

  std::map<unsigned int, std::vector<StepAndCharge>> stepMap;

  if (hit->getNMCContributions() == 0)
    return;

  float currentTime = hit->getTimeCont(0);
  PotentialSameTrackID id(hit->getPDGCont(0), hit->getParticleCont(0)->getPDG());
  unsigned int currentNum = 0;

  for (int imcp = 0; imcp < hit->getNMCContributions(); imcp++) {
    LCVector3D stepPos;
    const float* pos = hit->getStepPosition(imcp);
    if (NULL != pos) {
      stepPos.set(pos[0], pos[1], pos[2]);
      stepPos -= hitPos;
      stepPos = LCVector3D(stepPos * _Iaxis, stepPos * _Jaxis, stepPos * _normal);

      PotentialSameTrackID stepID(hit->getPDGCont(imcp), hit->getParticleCont(imcp)->getPDG());

      if (std::abs(stepPos.z()) < std::numeric_limits<float>::epsilon()) // step in cell center so I assume it is unique
                                                                         // and do not have to be linked
      {
        vec.push_back(StepAndCharge(stepPos, hit->getLengthCont(imcp), hit->getTimeCont(imcp)));
        currentNum++;
      } else // put it on the list of steps to be linked
      {
        if (hit->getTimeCont(imcp) >= currentTime && (stepID == id))
          stepMap[currentNum].push_back(StepAndCharge(stepPos, hit->getLengthCont(imcp), hit->getTimeCont(imcp)));
        else {
          currentNum++;
          stepMap[currentNum].push_back(StepAndCharge(stepPos, hit->getLengthCont(imcp), hit->getTimeCont(imcp)));
        }
      }

      currentTime = hit->getTimeCont(imcp);
      id.PDGStep = hit->getPDGCont(imcp);
      id.PDGParent = hit->getParticleCont(imcp)->getPDG();
    } else
      streamlog_out(WARNING) << "DIGITISATION : STEP POSITION IS (0,0,0)" << std::endl;
  }

  if (link) {
    for (auto& it : stepMap)
      linkSteps(it.second);
  }

  for (const auto& it : stepMap)
    for (const auto& step : it.second)
      vec.push_back(step);

  if (vec.empty())
    streamlog_out(MESSAGE) << "no Steps in hit" << std::endl;
}

template <typename InputTraits>
void SimDigitalGeomCellIdLCGEO<InputTraits>::processGeometry(SimCalorimeterHit* hit) {
  this->_cellIDvalue = this->_decoder(hit).getValue();

  this->_trueLayer = this->_decoder(hit)[this->_encodingString.at(0)] - 1;
  this->_stave = this->_decoder(hit)[this->_encodingString.at(1)]; // +1
  this->_module = this->_decoder(hit)[this->_encodingString.at(2)];

  if (_encodingString.at(3).size() != 0)
    this->_tower = this->_decoder(hit)[_encodingString.at(3)];

  this->_Iy = this->_decoder(hit)[_encodingString.at(4)];
  try {
    this->_Jz = this->_decoder(hit)[_encodingString.at(5)];
  } catch (lcio::Exception&) {
    _encodingString.at(5) = "z";

    try {
      this->_Jz = this->_decoder(hit)[_encodingString.at(5)];
    } catch (lcio::Exception&) {
      _encodingString.at(5) = "y";
      this->_Jz = this->_decoder(hit)[_encodingString.at(5)];
    }
  }

  // _slice     = _decoder( hit )["slice"];
  this->_hitPosition = hit->getPosition();
  if (abs(this->_Iy) < 1 && abs(this->_Iy) != 0.0)
    streamlog_out(DEBUG) << "_Iy, _Jz:" << this->_Iy << " " << this->_Jz << std::endl;
  // if(_module==0||_module==6) streamlog_out( DEBUG )<<"tower "<<_tower<<" layer "<<_trueLayer<<" stave "<<_stave<<"
  // module "<<_module<<std::endl;
  //<<" Iy " << _Iy <<"  Jz "<<_Jz<<" hitPosition "<<_hitPosition<<std::endl
  //<<" _hitPosition[0] "<<_hitPosition[0]<<" _hitPosition[1] "<<_hitPosition[1]<<" _hitPosition[2]
  //"<<_hitPosition[2]<<std::endl;

  dd4hep::Detector& ild = dd4hep::Detector::getInstance();
  dd4hep::rec::CellIDPositionConverter idposConv(ild);

  dd4hep::BitField64 idDecoder(this->_cellIDEncodingString);

  const dd4hep::CellID id0 = hit->getCellID0();
  const dd4hep::CellID id1 = hit->getCellID1();

  idDecoder.setValue(id0, id1);

  const dd4hep::CellID id = idDecoder.getValue();
  dd4hep::Position pos_0 = idposConv.position(id);

#if 0
	const float* hitPos = hit->getPosition();

	streamlog_out( DEBUG ) << "hit pos: " << hitPos[0] << " " << hitPos[1] << " " << hitPos[2] << std::endl;
	streamlog_out( DEBUG ) << "cell pos: " << pos_0.X() << " " << pos_0.Y() << " " << pos_0.Z() << std::endl;

	streamlog_out( DEBUG ) << "layer: "    << idDecoder[_encodingStrings[_encodingType][0]]
			<< ", stave: "  << idDecoder[_encodingStrings[_encodingType][1]]
			<< ", module: " << idDecoder[_encodingStrings[_encodingType][2]]
			<< ", tower: "  << idDecoder[_encodingStrings[_encodingType][3]]
			<< ", x: "      << idDecoder[_encodingStrings[_encodingType][4]]
			<< ", y: "      << idDecoder[_encodingStrings[_encodingType][5]] << std::endl;
#endif

  double const epsilon = 1.e-3;

  ///// for direction x
  dd4hep::Position pos_i_plus_1;
  dd4hep::Position dir_i;

  std::string xEncoding = _encodingString.at(4);
  std::string yEncoding = _encodingString.at(5);

  idDecoder[xEncoding] = idDecoder[xEncoding] + 1;
  pos_i_plus_1 = idposConv.position(idDecoder.getValue());

  dir_i = pos_i_plus_1 - pos_0;

  if (dir_i.R() < epsilon) {
    idDecoder[xEncoding] = idDecoder[xEncoding] - 2;
    pos_i_plus_1 = idposConv.position(idDecoder.getValue());
    dir_i = -(pos_i_plus_1 - pos_0);
  }

  // reset
  idDecoder.setValue(id0, id1);

  ////// for direction y
  dd4hep::Position pos_j_plus_1;
  dd4hep::Position dir_j;

  idDecoder[yEncoding] = idDecoder[yEncoding] + 1;
  pos_j_plus_1 = idposConv.position(idDecoder.getValue());

  dir_j = pos_j_plus_1 - pos_0;

  if (dir_j.R() < epsilon) {
    idDecoder[yEncoding] = idDecoder[yEncoding] - 2;
    pos_j_plus_1 = idposConv.position(idDecoder.getValue());
    dir_j = -(pos_j_plus_1 - pos_0);
  }

  dd4hep::Position dir_layer = dir_i.Cross(dir_j);

  dir_layer = -dir_layer.Unit();

  // streamlog_out( DEBUG ) << "layer dir: " << dir_layer.X() << " " << dir_layer.Y() << " " << dir_layer.Z() <<
  // std::endl;

  dir_i = dir_i.Unit();
  dir_j = dir_j.Unit();

  this->_normal.set(dir_layer.X(), dir_layer.Y(), dir_layer.Z());
  this->_Iaxis.set(dir_i.X(), dir_i.Y(), dir_i.Z());
  this->_Jaxis.set(dir_j.X(), dir_j.Y(), dir_j.Z());
}

template <typename InputTraits>
void SimDigitalGeomCellIdPROTO<InputTraits>::processGeometry(SimCalorimeterHit* hit) {
  this->_cellIDvalue = this->_decoder(hit).getValue();

  this->_trueLayer = this->_decoder(hit)[_encodingString.at(0)];
  this->_Iy = this->_decoder(hit)[_encodingString.at(4)];
  this->_Jz = this->_decoder(hit)[_encodingString.at(5)];

  this->_hitPosition = hit->getPosition();
}

template <typename InputTraits>
std::vector<StepAndCharge> SimDigitalGeomCellId<InputTraits>::decode(SimCalorimeterHit* hit, bool link) {
  this->processGeometry(hit);

  std::vector<StepAndCharge> stepsInIJZcoord;

  this->createStepAndChargeVec(hit, stepsInIJZcoord, link);

  DebugGeomHit::fill(this);
  DebugGeomStep<InputTraits>::fill(this, hit, stepsInIJZcoord);

  return stepsInIJZcoord;
}

template <typename InputTraits>
std::unique_ptr<CalorimeterHitImpl> SimDigitalGeomCellIdLCGEO<InputTraits>::encode(int delta_I, int delta_J) {
  this->_encoder.setValue(this->_cellIDvalue);

  int RealIy = this->_Iy + delta_I;
  int RealJz = this->_Jz + delta_J;

  this->_encoder[_encodingString.at(4)] = RealIy;
  this->_encoder[_encodingString.at(5)] = RealJz;

  std::unique_ptr<CalorimeterHitImpl> hit(new CalorimeterHitImpl);
  this->_encoder.setCellID(hit.get());

  hit->setType(CHT(CHT::had, CHT::hcal, this->_currentHCALCollectionCaloLayout, this->_trueLayer));

  float posB[3];
  posB[0] = static_cast<float>(this->_hitPosition[0] + getCellSize() * (delta_I * this->_Iaxis.x() + delta_J * this->_Jaxis.x()));
  posB[1] = static_cast<float>(this->_hitPosition[1] + getCellSize() * (delta_I * this->_Iaxis.y() + delta_J * this->_Jaxis.y()));
  posB[2] = static_cast<float>(this->_hitPosition[2] + getCellSize() * (delta_I * this->_Iaxis.z() + delta_J * this->_Jaxis.z()));
  hit->setPosition(posB);

  return hit;
}

template <typename InputTraits>
std::unique_ptr<CalorimeterHitImpl> SimDigitalGeomCellIdPROTO<InputTraits>::encode(int delta_I, int delta_J) {
  this->_encoder.setValue(this->_cellIDvalue);

  int RealIy = this->_Iy + delta_I;
  int RealJz = this->_Jz + delta_J;

  if (RealIy < 0 || RealJz < 0)
    return std::unique_ptr<CalorimeterHitImpl>(nullptr);

  this->_encoder[_encodingString.at(4)] = RealIy;
  this->_encoder[_encodingString.at(5)] = RealJz;

  std::unique_ptr<CalorimeterHitImpl> hit(new CalorimeterHitImpl);
  this->_encoder.setCellID(hit.get());

  hit->setType(CHT(CHT::had, CHT::hcal, this->_currentHCALCollectionCaloLayout, this->_trueLayer));

  float posB[3];
  posB[0] = static_cast<float>(this->_hitPosition[0] + getCellSize() * (delta_I * this->_Iaxis.x() + delta_J * this->_Jaxis.x()));
  posB[1] = static_cast<float>(this->_hitPosition[1] + getCellSize() * (delta_I * this->_Iaxis.y() + delta_J * this->_Jaxis.y()));
  posB[2] = static_cast<float>(this->_hitPosition[2] + getCellSize() * (delta_I * this->_Iaxis.z() + delta_J * this->_Jaxis.z()));
  hit->setPosition(posB);

  return hit;
}

template <typename InputTraits>
void SimDigitalGeomCellIdLCGEO<InputTraits>::setLayerLayout(CHT::Layout layout) {
  this->_currentHCALCollectionCaloLayout = layout;

  dd4hep::Detector& ild = dd4hep::Detector::getInstance();

  if (this->_currentHCALCollectionCaloLayout == CHT::barrel) {
    const std::vector<dd4hep::DetElement>& det = dd4hep::DetectorSelector(ild).detectors(
        (dd4hep::DetType::CALORIMETER | dd4hep::DetType::HADRONIC | dd4hep::DetType::BARREL),
        (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD));

    _caloData = det.at(0).extension<dd4hep::rec::LayeredCalorimeterData>();
    // streamlog_out( DEBUG ) << "det size: " << det.size() << ", type: " << det.at(0).type() << endl;
  }

  if (this->_currentHCALCollectionCaloLayout == CHT::ring) {
    const std::vector<dd4hep::DetElement>& det = dd4hep::DetectorSelector(ild).detectors(
        (dd4hep::DetType::CALORIMETER | dd4hep::DetType::HADRONIC | dd4hep::DetType::AUXILIARY),
        dd4hep::DetType::FORWARD);

    _caloData = det.at(0).extension<dd4hep::rec::LayeredCalorimeterData>();

    // streamlog_out( DEBUG ) << "det size: " << det.size() << ", type: " << det.at(0).type() << endl;
  }

  if (this->_currentHCALCollectionCaloLayout == CHT::endcap) {
    const std::vector<dd4hep::DetElement>& det = dd4hep::DetectorSelector(ild).detectors(
        (dd4hep::DetType::CALORIMETER | dd4hep::DetType::HADRONIC | dd4hep::DetType::ENDCAP),
        (dd4hep::DetType::AUXILIARY | dd4hep::DetType::FORWARD));

    _caloData = det.at(0).extension<dd4hep::rec::LayeredCalorimeterData>();
    // streamlog_out( DEBUG ) << "det size: " << det.size() << ", type: " << det.at(0).type() << endl;
  }
}

template <typename InputTraits>
void SimDigitalGeomCellIdPROTO<InputTraits>::setLayerLayout(CHT::Layout layout) { this->_currentHCALCollectionCaloLayout = layout; }

template <typename InputTraits>
float SimDigitalGeomCellIdLCGEO<InputTraits>::getCellSize() {
  float cellSize = 0.f;
  const double CM2MM = 10.0;

  if (this->_cellSize > 0.0f)
    return this->_cellSize;

  if (_caloData != nullptr) {
    const std::vector<dd4hep::rec::LayeredCalorimeterStruct::Layer>& hcalBarrelLayers = _caloData->layers;
    cellSize = hcalBarrelLayers[this->_trueLayer].cellSize0 * CM2MM;
  }

  if (cellSize < std::numeric_limits<float>::epsilon())
    streamlog_out(WARNING) << "Cell Size is 0" << std::endl;

  return cellSize;
}

template class SimDigitalGeomCellId<ILCInputTraits>;
template class SimDigitalGeomCellIdLCGEO<ILCInputTraits>;
template class SimDigitalGeomCellIdPROTO<ILCInputTraits>;
