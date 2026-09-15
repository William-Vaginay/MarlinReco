#ifndef DebugGeomHit_h
#define DebugGeomHit_h

#include <marlin/Processor.h>
#include "marlinutil/LCGeometryTypes.h"

#include <AIDA/ITuple.h>

class SimDigitalGeomCellId_Base;

class DebugGeomHit {
public: 

    static void fill(SimDigitalGeomCellId_Base* geomCellId);

    static void bookTuples(const marlin::Processor* proc);

private:
  static AIDA::ITuple* _tupleHit;

  enum {
    TH_CHTLAYOUT,
    TH_MODULE,
    TH_TOWER,
    TH_STAVE,
    TH_LAYER,
    TH_I,
    TH_J,
    TH_X,
    TH_Y,
    TH_Z,
    TH_NORMALX,
    TH_NORMALY,
    TH_NORMALZ,
    TH_IX,
    TH_IY,
    TH_IZ,
    TH_JX,
    TH_JY,
    TH_JZ
  };

};

#endif