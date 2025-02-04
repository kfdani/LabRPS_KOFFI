#ifndef BRETSCHENEIDER_SPECTRUM_H
#define BRETSCHENEIDER_SPECTRUM_H

#include <Mod/SeaLabTools/SeaLabToolsGlobal.h>
#include "SeaSurfaceSpectrum.h"
#include <Base/SmartPtrPy.h>

namespace SeaLabTools {

class SeaLabToolsExport BretschneiderSpectrum : public SeaSurfaceSpectrum
{
public:
    BretschneiderSpectrum();
    ~BretschneiderSpectrum();

    //this computes the along wind spectrum
    double computeSpectrum(double frequency, double significantWaveHeight, double modalFrequency, double c1 = 0.3125, double c2 = -1.25);
    
    virtual PyObject* getPyObject(void);

    /// python object of this class
protected:// attributes
    Py::SmartPtr PythonObject;
};

}

#endif //BRETSCHENEIDER_SPECTRUM_H
