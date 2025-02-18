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

#ifndef IRPSWLSTANDARDDEVIATION_H
#define IRPSWLSTANDARDDEVIATION_H

#include "WindLabSimulationData.h"
#include <Mod/WindLabAPI/App/WindLabFeatureStandardDeviation.h>

namespace WindLabAPI {

/**
 * @class IrpsWLStandardDeviation
 * @brief An abstract class representing a standard deviation.
 *
 * This is a pure virtual class (interface) that defines the interface for standard deviations.
 * In the context of random phenomenon simulation, standard deviation is a statistical measure 
 * that quantifies the amount of variation or spread in a set of random values. It is an important 
 * concept because it provides insight into how much individual values of a random variable deviate 
 * from the mean (or expected value) of the distribution. 
 * Derived classes must implement all its methods. 
 */
class IrpsWLStandardDeviation : public WindLabAPI::WindLabFeatureStandardDeviation
{
public:

    /**
     * @brief Virtual destructor for IrpsWLStandardDeviation class.
     * Provides proper cleanup in case a derived class object is destroyed.
     */
    virtual ~IrpsWLStandardDeviation() {};

    /** Compute the standard deviation value for given time and location.
     * @param Data         the simulation data containing all the simulation parameters input by the user.
     * @param location     a location (simulation point represented by 3D position vector) where wind velocity time series is desired.
     * @param dTime        the time instant at which the standard deviation value will be computed.
     * @param dValue       a value to be updated. This is the computed standard deviation value.
     * @return             return true if the computation is successful and false in case of failure.
     */	
    virtual bool ComputeStandardDeviationValue(const WindLabSimulationData &Data, const Base::Vector3d &location, const double &dTime, double &dValue) = 0;

    /** Compute the standard deviation values at a given time instant and for all locations (simulation points).
     * @param Data         the simulation data containing all the simulation parameters input by the user.
     * @param dTime        the time instant at which the standard deviation value will be computed.
     * @param dVarVector   a vector to be updated. It should contains all the location numbers used to compute each value stored in dValVector.
     * @param dValVector   a vector to be updated. It should contain all the values computed for each location stored in dVarVector.
     * @return             return true if the computation is successful and false in case of failure.
     */
    virtual bool ComputeStandardDeviationVectorP(const WindLabSimulationData &Data, const double &dTime, vec &dVarVector, vec &dValVector) = 0;

    /** Compute the standard deviation values at a given location (simulation point) and for all time increments.
     * @param Data         the simulation data containing all the simulation parameters input by the user.
     * @param location     a location (simulation point represented by 3D position vector) where wind velocity time series is desired.
     * @param dVarVector   a vector to be updated. It should contains all the time increments used to compute each value stored in dValVector.
     * @param dValVector   a vector to be updated. It should contain all the values computed for each time increment stored in dVarVector.
     * @return             return true if the computation is successful and false in case of failure.
     */
    virtual bool ComputeStandardDeviationVectorT(const WindLabSimulationData &Data, const Base::Vector3d &location, vec &dVarVector, vec &dValVector) = 0;

    /** Allows to do any initial taks before any of the other methods is called.
     * @param Data         the simulation data containing all the simulation parameters input by the user.
     * @return             return true if the computation is successful and false in case of failure.
     */
    virtual bool OnInitialSetting(const WindLabSimulationData &Data) = 0;

};

} //namespace WindLabAPI

#endif  // IRPSWLSTANDARDDEVIATION_H
