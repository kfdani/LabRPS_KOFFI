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

#ifndef RPSUSERLABALLFEATURECOMPUTATION_H
#define RPSUSERLABALLFEATURECOMPUTATION_H

#include <QObject>
#include <QComboBox>
#include <QThread>
#include <QTimer>

#include <Mod/UserLab/App/UserLabSimulation.h>
#include "UserLabSimulationWorker.h"

namespace UserLabGui
{

class UserLabAllFeaturesComputation: public QObject
{
    Q_OBJECT

public:
    UserLabAllFeaturesComputation(UserLab::UserLabSimulation* sim, QObject* parent = nullptr);
    ~UserLabAllFeaturesComputation();
   
Q_SIGNALS:
    void stopped();

public Q_SLOTS:

    void slotDisplayResultInTable(QString str, int what);
    void slotDisplayResultInMatrix(QString str, int what);

 
private:
    //// wind velocity simulation 
    UserLab::UserLabSimulation* m_sim;

    QThread* simulationThread = nullptr;
    RPSUserLabSimulationWorker* simulationWorker = nullptr;

public:
    int complexRrealImag;

public:
    RPSUserLabSimulationWorker *GetUserLabSimulationWorker();
    QThread* getUserLabSimulationThread();
    QString logSimulationInfo(bool status, const QString& name);
    void startSimulationWorker(QString function, const char* complexNumberDisplay = "Real");
    void setComplexNumberDisplay(const char* displayType);
};

}//namespace UserLabGui


#endif// RPSUSERLABSIMULATIONWORKER_H
