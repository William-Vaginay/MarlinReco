#ifndef DebugGeomStep_h
#define DebugGeomStep_h

#include <marlin/Processor.h>

#include <EVENT/SimCalorimeterHit.h>
#include <AIDA/ITuple.h>

class SimDigitalGeomCellId;
struct StepAndCharge;

class DebugGeomStep {
public:
    static void fill(SimDigitalGeomCellId* geomCellId, SimCalorimeterHit* hit, const std::vector<StepAndCharge>& stepsInIJZcoord);
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