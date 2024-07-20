/***************************************************************************
 *   Copyright (c) 2024 Koffi Daniel <kfdani@labrps.com>                   *
 *                                                                         *
 *   This file is part of the LabRPS development system.                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"

#include "ViewProviderSeismicLabFeatureLocationDistributionPy.h"
#include "ViewProviderSeismicLabFeatureLocationDistributionPy.cpp"
#include <CXX/Objects.hxx>

#include "ViewProviderSeismicLabFeatureLocationDistribution.h"
#include <boost/python/copy_const_reference.hpp>

using namespace SeismicLabGui;

// returns a string which represents the object e.g. when printed in python
std::string ViewProviderSeismicLabFeatureLocationDistributionPy::representation(void) const
{
    return std::string("<ViewProviderSeismicLabFeatureLocationDistribution object>");
}

PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::computeLocationCoordinateMatrixP3(PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return nullptr;

    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
    if (vp)
    {
        bool ok = vp->ComputeLocationCoordinateMatrixP3();
        return Py::new_reference_to(Py::Boolean(ok));
    }

    Py_RETURN_NONE;
}

PyObject *ViewProviderSeismicLabFeatureLocationDistributionPy::getCustomAttributes(const char* /*attr*/) const
{
    return nullptr;
}

PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::getComputationResultVariableVector(PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return nullptr;
   
    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
   
    vec dVarVector;

    if (vp)
    {
        bool ok = vp->getComputationResultVariableVector(dVarVector);

        if (!ok)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    Py::List resList;
    for (int j = 0; j < dVarVector.cols(); j++) {
        resList.append(Py::Float((dVarVector.coeff(j))));
    }

    return boost::python::incref(resList.ptr());
}
PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::getComputationResultValueVector(PyObject* args)
{
     if (!PyArg_ParseTuple(args, ""))
        return nullptr;
   
    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
   
    vec dVarVeltor;

    if (vp)
    {
        bool ok = vp->getComputationResultValueVector(dVarVeltor);

        if (!ok)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    Py::List resList;
    for (int j = 0; j < dVarVeltor.cols(); j++) {
        resList.append(Py::Float((dVarVeltor.coeff(j))));
    }

    return boost::python::incref(resList.ptr());
}
PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::getComputationResultComplexValueVector(PyObject* args)
{
     if (!PyArg_ParseTuple(args, ""))
        return nullptr;
   
    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
   
    cx_vec dVarVeltor_cx;

    if (vp)
    {
        bool ok = vp->getComputationResultComplexValueVector(dVarVeltor_cx);

        if (!ok)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

    Py::List resList;
    for (int j = 0; j < dVarVeltor_cx.cols(); j++) {
        resList.append(Py::Float((dVarVeltor_cx(j).real())));
        resList.append(Py::Float((dVarVeltor_cx(j).imag())));
    }

    return boost::python::incref(resList.ptr());
}
PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::getComputationResultMatrix(PyObject* args)
{
     if (!PyArg_ParseTuple(args, ""))
        return nullptr;
   
    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
   
    mat resultMatrix;

    if (vp)
    {
        bool ok = vp->getComputationResultMatrix(resultMatrix);

        if (!ok)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

   Py::List resList;
    for (int i = 0; i < resultMatrix.rows(); i++) {
        Py::List row;
        for (int j = 0; j < resultMatrix.cols(); j++) {
            row.append(Py::Float((resultMatrix.coeff(i, j))));
        }
        resList.append(row);
    }
    return boost::python::incref(resList.ptr());
}
PyObject* ViewProviderSeismicLabFeatureLocationDistributionPy::getComputationResultComplexMatrix(PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return nullptr;
   
    ViewProviderSeismicLabFeatureLocationDistribution* vp = this->getViewProviderSeismicLabFeatureLocationDistributionPtr();
   
    cx_mat resultMatrix_cx;

    if (vp)
    {
        bool ok = vp->getComputationResultComplexMatrix(resultMatrix_cx);

        if (!ok)
        {
            Py_INCREF(Py_None);
            return Py_None;
        }
    }

   Py::List resList;
    for (int i = 0; i < resultMatrix_cx.rows(); i++) {
        Py::List row;
        for (int j = 0; j < resultMatrix_cx.cols(); j++) {
            row.append(Py::Float((resultMatrix_cx.coeff(i, j).real())));
        }
        resList.append(row);
    }
    return boost::python::incref(resList.ptr());
}

int ViewProviderSeismicLabFeatureLocationDistributionPy::setCustomAttributes(const char* /*attr*/, PyObject* /*obj*/)
{
    return 0;
}
