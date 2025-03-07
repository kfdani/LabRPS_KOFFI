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

#ifndef _PreComp_
# include <sstream>
#endif

#include <Base/Console.h>
#include <Base/Exception.h>
#include "SeismicLabFeatureCorrelation.h"

using namespace SeismicLabAPI;

RPS_LOG_LEVEL_INIT("SeismicLabFeatureCorrelation",true,true)

PROPERTY_SOURCE(SeismicLabAPI::SeismicLabFeatureCorrelation, SeismicLabAPI::SeismicLabFeature)


SeismicLabFeatureCorrelation::SeismicLabFeatureCorrelation(void)
{
    this->OutputUnitString.setValue("Dimensionless");
}

SeismicLabFeatureCorrelation::~SeismicLabFeatureCorrelation()
{
}

short SeismicLabFeatureCorrelation::mustExecute(void) const
{
    return SeismicLabFeature::mustExecute();
}

App::DocumentObjectExecReturn * SeismicLabFeatureCorrelation::recompute(void)
{
    try {
        return SeismicLabAPI::SeismicLabFeature::recompute();
    }
    catch (Base::Exception& e) {

        App::DocumentObjectExecReturn* ret = new App::DocumentObjectExecReturn(e.what());
        if (ret->Why.empty()) ret->Why = "Unknown exception";
        return ret;
    }
}

App::DocumentObjectExecReturn * SeismicLabFeatureCorrelation::execute(void)
{
    return SeismicLabFeature::execute();
}

void SeismicLabFeatureCorrelation::onChanged(const App::Property* prop)
{

    SeismicLabFeature::onChanged(prop);
}