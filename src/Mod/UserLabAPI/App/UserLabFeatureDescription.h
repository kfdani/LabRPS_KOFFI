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


#ifndef ULOBJECTDESCRIPTION_H
#define ULOBJECTDESCRIPTION_H

#include "RPSUserLabAPIdefines.h"
#include <App/RPSFeature.h>

namespace UserLabAPI {

class RPS_CORE_API UserLabFeatureDescription
{
public:
    UserLabFeatureDescription();
    ~UserLabFeatureDescription();

	 App::PropertyString PluginName;
	 App::PropertyString PublicationTitle;
	 App::PropertyString PublicationLink;
	 App::PropertyString PublicationAuthor;
	 App::PropertyString PublicationDate;
	 App::PropertyString ReleaseDate;
	 App::PropertyString Author;
	 App::PropertyString Version;
	 App::PropertyString APIVersion;
	 App::PropertyString PluginVersion;
     App::PropertyString type;
     App::PropertyString group;
     App::PropertyBool Stationarity;

	App::PropertyString LabRPSVersion;
	App::PropertyString Path;
	App::PropertyString ObjectName;
	App::PropertyString Description;
    App::PropertyBool IsUniformModulationFeature;
    App::PropertyString OutputUnitString;

    /// A coefficient each feature value is multiplied by.
    App::PropertyFloat ScaleCoefficient;
    App::PropertyStringList ApplicationFields;


	bool operator==(const UserLabAPI::UserLabFeatureDescription& rhs) const
	{
        return PluginName.getValue() == rhs.PluginName.getValue()
            && Description.getValue() == rhs.Description.getValue()
            && PublicationTitle.getValue() == rhs.PublicationTitle.getValue()
            && PublicationLink.getValue() == rhs.PublicationLink.getValue()
            && PublicationAuthor.getValue() == rhs.PublicationAuthor.getValue()
            && PublicationDate.getValue() == rhs.PublicationDate.getValue()
            && ObjectName.getValue() == rhs.ObjectName.getValue()
            && ReleaseDate.getValue() == rhs.ReleaseDate.getValue()
            && Author.getValue() == rhs.Author.getValue()
            && Version.getValue() == rhs.Version.getValue()
            && LabRPSVersion.getValue() == rhs.LabRPSVersion.getValue()
            && APIVersion.getValue() == rhs.APIVersion.getValue()
            && PluginVersion.getValue() == rhs.PluginVersion.getValue()
            && Stationarity.getValue() == rhs.Stationarity.getValue()
            && type.getValue() == rhs.type.getValue()
            && group.getValue() == rhs.group.getValue()
            && IsUniformModulationFeature.getValue() == rhs.IsUniformModulationFeature.getValue()
            && OutputUnitString.getValue() == rhs.OutputUnitString.getValue()
            && ScaleCoefficient.getValue() == rhs.ScaleCoefficient.getValue()
            && ApplicationFields.getValue() == rhs.ApplicationFields.getValue();

	}
};

} //namespace UserLabAPI
#endif// ULOBJECTDESCRIPTION_H