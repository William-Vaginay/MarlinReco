#include "DebugGeomHit.h"
#include "SimDigitalGeom.h"

#include <marlin/AIDAProcessor.h>
#include <AIDA/ITupleFactory.h>

AIDA::ITuple* DebugGeomHit::_tupleHit = NULL;

void DebugGeomHit::fill(SimDigitalGeomCellId_Base* geomCellId) {
  if (_tupleHit != nullptr) {
    _tupleHit->fill(TH_CHTLAYOUT, int(geomCellId->getCaloLayout()));
    _tupleHit->fill(TH_MODULE, geomCellId->module());
    _tupleHit->fill(TH_TOWER, geomCellId->tower());
    _tupleHit->fill(TH_STAVE, geomCellId->stave());
    _tupleHit->fill(TH_LAYER, geomCellId->K());
    _tupleHit->fill(TH_I, geomCellId->I());
    _tupleHit->fill(TH_J, geomCellId->J());

    if (geomCellId->hitPosition() != NULL) {
      _tupleHit->fill(TH_X, geomCellId->hitPosition()[0]); // x
      _tupleHit->fill(TH_Y, geomCellId->hitPosition()[1]); // y
      _tupleHit->fill(TH_Z, geomCellId->hitPosition()[2]); // z
    } else {
      float notset = -88888;
      _tupleHit->fill(TH_X, notset);
      _tupleHit->fill(TH_Y, notset);
      _tupleHit->fill(TH_Z, notset);
    }

    for (int i = 0; i < 3; i++) {
        _tupleHit->fill(TH_NORMALX + i, float(geomCellId->normalToRPCPlane()[i]));
        _tupleHit->fill(TH_IX + i, float(geomCellId->Iaxis()[i]));
        _tupleHit->fill(TH_JX + i, float(geomCellId->Jaxis()[i]));
    }
    _tupleHit->addRow();
  }
}

void DebugGeomHit::bookTuples(const marlin::Processor* proc) {
  _tupleHit = marlin::AIDAProcessor::tupleFactory(proc)->create(
      "SimDigitalGeom", "SimDigital_Debug",
      "int chtlayout,module,tower,stave,layer,I,J, float x,y,z, normalx,normaly,normalz, Ix,Iy,Iz,Jx,Jy,Jz");
  streamlog_out(DEBUG) << "Tuple for Hit has been initialized to " << _tupleHit << std::endl;
  streamlog_out(DEBUG) << "it has " << _tupleHit->columns() << " columns" << std::endl;
}