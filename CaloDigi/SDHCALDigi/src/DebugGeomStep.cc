#include "DebugGeomStep.h"
#include "SimDigitalGeom.h"
#include "SimDigital.h"

#include <marlin/AIDAProcessor.h>
#include <AIDA/ITupleFactory.h>

template <typename InputTraits>
AIDA::ITuple* DebugGeomStep<InputTraits>::_tupleStep = NULL;

template <typename InputTraits>
void DebugGeomStep<InputTraits>::fill(SimDigitalGeomCellId<InputTraits>* geomCellId, simcalohitType* hit, const std::vector<StepAndCharge>& stepsInIJZcoord) {
  if (_tupleStep != nullptr) {
    int nsteps = SimDigital_Data::getNMCContributions(hit);
    float notset = -88888;
    for (int imcp = 0; imcp < nsteps; imcp++) {
      _tupleStep->fill(TS_CHTLAYOUT, int(geomCellId->getCaloLayout()));
      _tupleStep->fill(TS_HITCELLID, SimDigital_Data::getCellID0(hit));
      _tupleStep->fill(TS_NSTEP, SimDigital_Data::getNMCContributions(hit));
      const float* steppos = SimDigital_Data::getStepPosition(hit, imcp);
      for (int i = 0; i < 3; i++) {
        if (geomCellId->hitPosition() != NULL)
           _tupleStep->fill(TS_HITX + i, geomCellId->hitPosition()[i]);
        else
          _tupleStep->fill(TS_HITX + i, notset);

        if (steppos != NULL)
          _tupleStep->fill(TS_STEPX + i, steppos[i]);
        else
          _tupleStep->fill(TS_STEPX + i, notset);

        if (imcp < (int)stepsInIJZcoord.size())
          _tupleStep->fill(TS_DELTAI + i, float(stepsInIJZcoord[imcp].step[i]));
        else
          _tupleStep->fill(TS_DELTAI + i, notset);
      }
      if (imcp < (int)stepsInIJZcoord.size())
        _tupleStep->fill(TS_TIME, SimDigital_Data::getTimeCont(hit, imcp));
      else
        _tupleStep->fill(TS_TIME, notset);

    _tupleStep->addRow();
    }
  }
}

template <typename InputTraits>
void DebugGeomStep<InputTraits>::bookTuples(const marlin::Processor* proc) {
  _tupleStep = AIDAProcessor::tupleFactory(proc)->create(
      "SimDigitalStep", "SimDigital_DebugStep",
      "int chtlayout,hitcellid,nstep, float hitx,hity,hitz,stepx,stepy,stepz,deltaI,deltaJ,deltaLayer,time");
  streamlog_out(DEBUG) << "Tuple for Step has been initialized to " << _tupleStep << std::endl;
  streamlog_out(DEBUG) << "it has " << _tupleStep->columns() << " columns" << std::endl;
}

template class DebugGeomStep<ILCInputTraits>; 
template class DebugGeomStep<WGGInputTraits>;