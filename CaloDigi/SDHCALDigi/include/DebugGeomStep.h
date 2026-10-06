#ifndef DebugGeomStep_h
#define DebugGeomStep_h

#include "InputTraits.h"

#include <marlin/Processor.h>

#include <EVENT/SimCalorimeterHit.h>
#include <AIDA/ITuple.h>

template <typename InputTraits>
class SimDigitalGeomCellId;
struct StepAndCharge;

template <typename InputTraits>
class DebugGeomStep {
public:
  using simcalohitType =    typename InputTraits::simcalohitType;

  static void fill(SimDigitalGeomCellId<InputTraits>* geomCellId, simcalohitType* hit, const std::vector<StepAndCharge>& stepsInIJZcoord);
  static void bookTuples(const marlin::Processor* proc);

private:
  static AIDA::ITuple* _tupleStep;

  enum {
    TS_CHTLAYOUT,
    TS_HITCELLID,
    TS_NSTEP,
    TS_HITX,
    TS_HITY,
    TS_HITZ,
    TS_STEPX,
    TS_STEPY,
    TS_STEPZ,
    TS_DELTAI,
    TS_DELTAJ,
    TS_DELTALAYER,
    TS_TIME
  };
};

#endif