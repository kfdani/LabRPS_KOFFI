#include "PreCompiled.h"
#include "SimuSpectrum.h"
#include <math.h>
#include <Mod/WindLabTools/App/spectrum/SimuSpectrumPy.h>

using namespace WindLabTools;

SimuSpectrum::SimuSpectrum()
{

}

SimuSpectrum::~SimuSpectrum()
{

}

double  SimuSpectrum::computeAlongWindAutoSpectrum(const double &frequency, const double &height, const double &meanSpeed, const double &shearVelocity, double Par1, double Par2)
{
    double dBuf1, dBuf2, f;

    double dPSD = 0.0;

    f = frequency * height / (meanSpeed);

    dBuf1 = 1.0 + Par2 * f;

    dBuf2 = pow(dBuf1, 5.0 / 3.0);

    dPSD = Par1 * shearVelocity * shearVelocity * f / frequency;

    dPSD /= dBuf2;

    return dPSD;
}

double SimuSpectrum::computeAcrossWindAutoSpectrum(const double &frequency, const double &height, const double &meanSpeed, const double &shearVelocity, double Par1, double Par2)
{
    double dBuf1, dBuf2, f;

    double dPSD = 0.0;

    f = frequency * height / (meanSpeed);

    dBuf1 = 1.0 + Par2 * f;

    dBuf2 = pow(dBuf1, 5.0 / 3.0);

    dPSD = Par1 * shearVelocity * shearVelocity * f / frequency;

    dPSD /= dBuf2;			

    return dPSD;
}

double SimuSpectrum::computeVerticalWindAutoSpectrum(const double &frequency, const double &height, const double &meanSpeed, const double &shearVelocity, double Par1, double Par2)
{
    double dBuf2, f;
   
    double dPSD = 0.0;

    f = frequency * height / (meanSpeed);

    dBuf2 = 1.0 + Par2 *pow(f, 5.0 / 3.0);

    dPSD = Par1 * shearVelocity * shearVelocity * f / frequency;

    dPSD /= dBuf2;			

    return dPSD;
}

PyObject* SimuSpectrum::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new SimuSpectrumPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}