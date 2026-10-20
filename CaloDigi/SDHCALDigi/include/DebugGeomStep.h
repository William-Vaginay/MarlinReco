#ifndef DebugGeomStep_h
#define DebugGeomStep_h

#include <vector>

namespace marlin {
  class Processor;
}
namespace AIDA {
  class ITuple;
}
template <typename InputTraits>
class SimDigitalGeomCellId;
struct StepAndCharge;

template <typename InputTraits>
class DebugGeomStep {
public:
  using simcalohitType =    typename InputTraits::simcalohitType;

  static void fill(SimDigitalGeomCellId<InputTraits>* geomCellId, simcalohitType* simhit, const std::vector<StepAndCharge>& stepsInIJZcoord);
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