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
#endif

#include "ViewProviderSeaLabFeatureLocationDistribution.h"
#include "ViewProviderSeaLabFeatureLocationDistributionPy.h"
#include "ViewProviderSeaLabSimulation.h"
#include <App/Application.h>
#include <Mod/SeaLab/App/SeaLabSimulation.h>
#include <Mod/SeaLab/Gui/DlgSeaLabFeaturesCreation.h>
#include <Gui/Control.h>
#include <Mod/SeaLab/Gui/SeaLabSimulationObserver.h>
#include <App/Document.h>
#include <Gui/ActionFunction.h>
#include "ViewProviderSeaLabSimulation.h"
#include <Gui/Application.h>
#include <Gui/Document.h>
#include <Mod/SeaLabAPI/App/IrpsSeLLocationDistribution.h>
#include <Mod/SeaLabAPI/App/RPSSeaLabFramework.h>
#include <Mod/SeaLab/App/SeaLabUtils.h>
#include <Gui/MainWindow.h>
#include <Gui/AlphaPlot.h>
#include <Gui/ViewParams.h>
#include <CXX/Objects.hxx>

using namespace SeaLabGui;
namespace bp = boost::placeholders;

//**************************************************************************
// Construction/Destruction

PROPERTY_SOURCE(SeaLabGui::ViewProviderSeaLabFeatureLocationDistribution, Gui::ViewProviderDocumentObject)

App::PropertyFloatConstraint::Constraints ViewProviderSeaLabFeatureLocationDistribution::sizeRange = {1.0, 64.0, 1.0};

ViewProviderSeaLabFeatureLocationDistribution::ViewProviderSeaLabFeatureLocationDistribution()
{
  sPixmap = ":/icons/SeaLabFeatures/SeaLab_Feature_LocationObj.svg";

  //// get default line color
  //unsigned long lcol = Gui::ViewParams::instance()->getDefaultShapeLineColor();// dark grey (25,25,25)
  //float lr, lg, lb;
  //lr = ((lcol >> 24) & 0xff) / 255.0;
  //lg = ((lcol >> 16) & 0xff) / 255.0;
  //lb = ((lcol >> 8) & 0xff) / 255.0;
 
  //// get default vertex color
  //unsigned long vcol = Gui::ViewParams::instance()->getDefaultShapeVertexColor();
  //float vr, vg, vb;
  //vr = ((vcol >> 24) & 0xff) / 255.0;
  //vg = ((vcol >> 16) & 0xff) / 255.0;
  //vb = ((vcol >> 8) & 0xff) / 255.0;
  //int lwidth = Gui::ViewParams::instance()->getDefaultShapeLineWidth();
  //int psize = Gui::ViewParams::instance()->getDefaultShapePointSize();

  //static const char* osgroup = "Object Style";

  //App::Material lmat;
  //lmat.ambientColor.set(0.2f, 0.2f, 0.2f);
  //lmat.diffuseColor.set(lr, lg, lb);
  //lmat.specularColor.set(0.0f, 0.0f, 0.0f);
  //lmat.emissiveColor.set(0.0f, 0.0f, 0.0f);
  //lmat.shininess = 1.0f;
  //lmat.transparency = 0.0f;

  //App::Material vmat;
  //vmat.ambientColor.set(0.4f, 0.30f, 0.15f);
  //vmat.diffuseColor.set(vr, vg, vb);
  //vmat.specularColor.set(0.0f, 0.0f, 0.0f);
  //vmat.emissiveColor.set(0.0f, 0.0f, 0.0f);
  //vmat.shininess = 1.0f;
  //vmat.transparency = 0.0f;

  //ADD_PROPERTY_TYPE(LineMaterial, (lmat), osgroup, App::Prop_None, "Object line material.");
  //ADD_PROPERTY_TYPE(PointMaterial, (vmat), osgroup, App::Prop_None, "Object point material.");
  //ADD_PROPERTY_TYPE(LineColor, (lmat.diffuseColor), osgroup, App::Prop_None,"Set object line color.");
  //ADD_PROPERTY_TYPE(PointColor, (vmat.diffuseColor), osgroup, App::Prop_None,"Set object point color");
  //ADD_PROPERTY_TYPE(PointColorArray, (PointColor.getValue()), osgroup, App::Prop_None,"Object point color array.");
  //ADD_PROPERTY_TYPE(LineColorArray, (LineColor.getValue()), osgroup, App::Prop_None, "Object line color array.");
  //ADD_PROPERTY_TYPE(LineWidth, (lwidth), osgroup, App::Prop_None, "Set object line width.");
  //
  //LineWidth.setConstraints(&sizeRange);
  //PointSize.setConstraints(&sizeRange);
  //
  //ADD_PROPERTY_TYPE(PointSize, (psize), osgroup, App::Prop_None, "Set object point size.");
  //ADD_PROPERTY_TYPE(ShowGround, (true), "Ground", App::Prop_None,"Whether to show the ground or not.");
  //ADD_PROPERTY_TYPE(GroundLength, (100), "Ground", App::Prop_None, "Set ground length.");
  //ADD_PROPERTY_TYPE(GroundWidth, (100), "Ground", App::Prop_None, "Set ground width.");
  //ADD_PROPERTY_TYPE(GroundColor, (vmat.ambientColor), "Ground", App::Prop_None,"Set object ground color");
  //ADD_PROPERTY_TYPE(GroundOpacity, (0.6), "Ground", App::Prop_None, "Set ground opacity.");
  //ADD_PROPERTY_TYPE(ShowGrid, (true), "Grid", App::Prop_None,"Whether to show the grid or not.");
  //ADD_PROPERTY_TYPE(ShowAxis, (true), "Coordinate Axis", App::Prop_None,"Whether to show the axis or not.");
  //ADD_PROPERTY_TYPE(AxisScale, (0.5), "Coordinate Axis", App::Prop_None, "The scale of the axis.");
  //ADD_PROPERTY_TYPE(ShowPointNumbering, (true), "Point Numbering", App::Prop_None, "Whether to show the point numbers or not.");
  //ADD_PROPERTY_TYPE(NumberingOffsetX, (0.5), "Point Numbering", App::Prop_None,"The offset of the number in x direction.");
  //ADD_PROPERTY_TYPE(NumberingOffsetY, (0.5), "Point Numbering", App::Prop_None, "The offset of the number in y direction.");
  //ADD_PROPERTY_TYPE(NumberingOffsetZ, (0.5), "Point Numbering", App::Prop_None,"The offset of the number in z direction.");

  //QFont font;
  //font.setFamily(QString::fromLatin1(App::GetApplication()
  //                                       .GetUserParameter()
  //                                       .GetGroup("BaseApp")
  //                                       ->GetGroup("Preferences")
  //                                       ->GetGroup("Editor")
  //                                       ->GetASCII("Font", font.family().toLatin1())
  //                                       .c_str()));
  //font.setPointSize(App::GetApplication()
  //                      .GetUserParameter()
  //                      .GetGroup("BaseApp")
  //                      ->GetGroup("Preferences")
  //                      ->GetGroup("Editor")
  //                      ->GetInt("FontSize", font.pointSize()));

  //ADD_PROPERTY_TYPE(FontSize, (font.pointSize()), "Font", App::Prop_None, "Font size");
  //ADD_PROPERTY_TYPE(FontName, ((const char*)font.family().toLatin1()), "Font", App::Prop_None, "Font name");

   ADD_PROPERTY_TYPE(ShowSimulationPoints, (false), "Points", App::Prop_Hidden, "Whether to redraw the simulation point or not.");
}

ViewProviderSeaLabFeatureLocationDistribution::~ViewProviderSeaLabFeatureLocationDistribution()
{

}

bool ViewProviderSeaLabFeatureLocationDistribution::doubleClicked(void)
{
    ActivateFeature();
    Gui::Application::Instance->activeDocument()->setEdit(this);
    OnInitialSetting();
    return true;
}

bool ViewProviderSeaLabFeatureLocationDistribution::ActivateFeature()
{
auto doc = App::GetApplication().getActiveDocument();
    if (!doc)
        return false;
    auto obj = doc->getObject(this->getObject()->getNameInDocument());
    if (!obj)
        return false;
    std::string parentSimName = this->getObject()->getGroup()->getNameInDocument();
    
    //get the corresponding view provider
    SeaLabGui::ViewProviderSeaLabSimulation* vp = dynamic_cast<SeaLabGui::ViewProviderSeaLabSimulation*>(Gui::Application::Instance->getViewProvider(this->getObject()->getGroup()));

    if (!vp) {
        return false;
    }

    //activate the paraent simulation
    vp->doubleClicked();

    SeaLab::SeaLabSimulation* parentSim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->getSimulation(parentSimName));
    if (!parentSim) {Base::Console().Warning("The parent simulation of the selected feature couldn't be actived.\n");return false;}

    bool isfeatureStationary = static_cast<SeaLabAPI::SeaLabFeature*>(obj)->IsStationary.getValue();
    if (parentSim->Stationarity.getValue() != isfeatureStationary)
    {
        Base::Console().Warning("Simulation stationarity and that of the feature does not match. The feature couldn't be activated.\n");
        return false;
    }

    SeaLabGui::SeaLabSimulationObserver::instance()->setActiveSimulation(parentSim);

    parentSim->SpatialDistribution.setValue(obj->getNameInDocument());
    return true;
}

bool ViewProviderSeaLabFeatureLocationDistribution::setEdit(int ModNum)
{
    Q_UNUSED(ModNum);

    if (ModNum == ViewProvider::Default) {

        return true;
    }
    else {
        return Gui::ViewProviderDocumentObject::setEdit(ModNum);
    }
}

void ViewProviderSeaLabFeatureLocationDistribution::unsetEdit(int ModNum)
{
    if (ModNum == ViewProvider::Default) {
        // when pressing ESC make sure to close the dialog
        Gui::Control().closeDialog();
    }
    else {
        Gui::ViewProviderDocumentObject::unsetEdit(ModNum);
    }
}

void ViewProviderSeaLabFeatureLocationDistribution::setupContextMenu(QMenu* menu, QObject*, const char*)
{
    // toggle command to display components
    Gui::ActionFunction* func = new Gui::ActionFunction(menu);

    QAction* cvector = menu->addAction(QObject::tr("Compute Location Array"));
    func->trigger(cvector, boost::bind(&ViewProviderSeaLabFeatureLocationDistribution::ComputeLocationCoordinateMatrixP3, this));

    QAction* init = menu->addAction(QObject::tr("Setup Feature"));
    func->trigger(init, boost::bind(&ViewProviderSeaLabFeatureLocationDistribution::OnInitialSetting, this));

    QAction* act = menu->addAction(QObject::tr("Activate Feature"));
    func->trigger(act, boost::bind(&ViewProviderSeaLabFeatureLocationDistribution::ActivateFeature, this));
}


bool ViewProviderSeaLabFeatureLocationDistribution::ComputeLocationCoordinateMatrixP3()
{

    Base::Interpreter().runString("import SeaLabUtils");
    Base::Interpreter().runString("SeaLabUtils.showSimulationPoints()");
    return runFeatureMethod(SeaLab::SeaLabUtils::ComputeLocationCoordinateMatrixP3);

    //if (ShowSimulationPoints.getValue() == true)
    //{
    //    //SeaLab::SeaLabSimulation* sim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->active());
    //    //if (sim)
    //    //{
    //    //    SeaLabAPI::SeaLabSimulationData* simData = sim->getSimulationData();
    //    //    if (simData != nullptr) {
    //    //        int nberOfSimPoint = sim->getSimulationData()->numberOfSpatialPosition.getValue();
    //    //        mat dLocCoord(nberOfSimPoint, 4);
    //    //        SeaLabAPI::CRPSSeaLabFramework::ComputeLocationCoordinateMatrixP3(*simData, dLocCoord);
    //    //        auto doc = App::GetApplication().getActiveDocument();

    //    //        std::vector<App::DocumentObject*> points;
    //    //        App::DocumentObjectGroup* group = static_cast<App::DocumentObjectGroup*>(doc->addObject("App::DocumentObjectGroup", "SimulationPoints"));

    //    //        if (doc)
    //    //        {
    //    //            for (int i = 0; i < nberOfSimPoint; i++) {
    //    //           /* Part::Vertex* point = static_cast<Part::Vertex*>(doc->addObject("Part::Vertex", "Vertex"));
    //    //            point->X.setValue(dLocCoord(i, 1));
    //    //            point->Y.setValue(dLocCoord(i, 2));
    //    //            point->Z.setValue(dLocCoord(i, 3));
    //    //            group->addObject(point);*/
    //    //            }
    //    //            sim->addObject(group);
    //    //        }
    //    //    }
    //    //}
    //}
}

bool ViewProviderSeaLabFeatureLocationDistribution::OnInitialSetting()
{
    ActivateFeature();
    SeaLab::SeaLabSimulation* sim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}
    sim->seaLabFeatureInitalSetting(SeaLab::SeaLabUtils::objGroupLocationDistribution, QString::fromUtf8(sim->getSimulationData()->spatialDistribution.getValue()));
    return true;
}

void ViewProviderSeaLabFeatureLocationDistribution::onChanged(const App::Property* prop)
{
    //if (prop == &LineWidth) {
    //    //pcLineStyle->lineWidth = LineWidth.getValue();
    //}
    //else if (prop == &PointSize) {
    //    //pcPointStyle->pointSize = PointSize.getValue();
    //}
    //else if (prop == &LineColor) {
    //   /* const App::Color& c = LineColor.getValue();
    //    pcLineMaterial->diffuseColor.setValue(c.r, c.g, c.b);
    //    if (c != LineMaterial.getValue().diffuseColor)
    //        LineMaterial.setDiffuseColor(c);
    //    LineColorArray.setValue(LineColor.getValue());*/
    //}
    //else if (prop == &PointColor) {
    //    /*const App::Color& c = PointColor.getValue();
    //    pcPointMaterial->diffuseColor.setValue(c.r, c.g, c.b);
    //    if (c != PointMaterial.getValue().diffuseColor)
    //        PointMaterial.setDiffuseColor(c);
    //    PointColorArray.setValue(PointColor.getValue());*/
    //}
    //else if (prop == &LineMaterial) {
    //   /* const App::Material& Mat = LineMaterial.getValue();
    //    if (LineColor.getValue() != Mat.diffuseColor)
    //        LineColor.setValue(Mat.diffuseColor);
    //    pcLineMaterial->ambientColor.setValue(Mat.ambientColor.r, Mat.ambientColor.g,
    //                                          Mat.ambientColor.b);
    //    pcLineMaterial->diffuseColor.setValue(Mat.diffuseColor.r, Mat.diffuseColor.g,
    //                                          Mat.diffuseColor.b);
    //    pcLineMaterial->specularColor.setValue(Mat.specularColor.r, Mat.specularColor.g,
    //                                           Mat.specularColor.b);
    //    pcLineMaterial->emissiveColor.setValue(Mat.emissiveColor.r, Mat.emissiveColor.g,
    //                                           Mat.emissiveColor.b);
    //    pcLineMaterial->shininess.setValue(Mat.shininess);
    //    pcLineMaterial->transparency.setValue(Mat.transparency);*/
    //}
    //else if (prop == &PointMaterial) {
    //   /* const App::Material& Mat = PointMaterial.getValue();
    //    if (PointColor.getValue() != Mat.diffuseColor)
    //        PointColor.setValue(Mat.diffuseColor);
    //    pcPointMaterial->ambientColor.setValue(Mat.ambientColor.r, Mat.ambientColor.g,
    //                                           Mat.ambientColor.b);
    //    pcPointMaterial->diffuseColor.setValue(Mat.diffuseColor.r, Mat.diffuseColor.g,
    //                                           Mat.diffuseColor.b);
    //    pcPointMaterial->specularColor.setValue(Mat.specularColor.r, Mat.specularColor.g,
    //                                            Mat.specularColor.b);
    //    pcPointMaterial->emissiveColor.setValue(Mat.emissiveColor.r, Mat.emissiveColor.g,
    //                                            Mat.emissiveColor.b);
    //    pcPointMaterial->shininess.setValue(Mat.shininess);
    //    pcPointMaterial->transparency.setValue(Mat.transparency);*/
    //}
    //else if (prop == &PointColorArray) {
    //    //setHighlightedPoints(PointColorArray.getValues());
    //}
    //else if (prop == &LineColorArray) {
    //    //setHighlightedEdges(LineColorArray.getValues());
    //}
    //else if (prop == &ShowGround) {

    //}
    //else if (prop == &GroundLength) {}
    //else if (prop == &GroundWidth) {}
    //else if (prop == &GroundColor) {}
    //else if (prop == &GroundOpacity) {}
    //else if (prop == &ShowGrid) {}
    //else if (prop == &ShowAxis) {}
    //else if (prop == &AxisScale) {}
    //else if (prop == &ShowPointNumbering) {}
    //else if (prop == &NumberingOffsetX) {}
    //else if (prop == &NumberingOffsetY) {}
    //else if (prop == &NumberingOffsetZ) {}
    //else if (prop == &FontSize) {}
    //else if (prop == &FontName) {}

    ViewProviderDocumentObject::onChanged(prop);
}


PyObject* ViewProviderSeaLabFeatureLocationDistribution::getPyObject(void)
{
    if (PythonObject.is(Py::_None())) {
        // ref counter is set to 1
        PythonObject = Py::Object(new ViewProviderSeaLabFeatureLocationDistributionPy(this), true);
    }
    return Py::new_reference_to(PythonObject);
}

bool ViewProviderSeaLabFeatureLocationDistribution::getComputationResultVariableVector(vec& resultVectorVar)
{
    if (!seaLabAllFeaturesComputation)
        return false;
    resultVectorVar = seaLabAllFeaturesComputation->GetSeaLabSimulationWorker()->m_ResultVectorVar;
    return true;
}
bool ViewProviderSeaLabFeatureLocationDistribution::getComputationResultValueVector(vec& resultVectorVal)
{
    if (!seaLabAllFeaturesComputation)
        return false;
    resultVectorVal = seaLabAllFeaturesComputation->GetSeaLabSimulationWorker()->m_ResultVectorVal;
    return true;
}
bool ViewProviderSeaLabFeatureLocationDistribution::getComputationResultComplexValueVector(cx_vec& resultVectorVal_cx)
{
    if (!seaLabAllFeaturesComputation)
        return false;
    resultVectorVal_cx = seaLabAllFeaturesComputation->GetSeaLabSimulationWorker()->m_ResultVectorVal_cx;
    return true;
}
bool ViewProviderSeaLabFeatureLocationDistribution::getComputationResultMatrix(mat& resultMatrix)
{
     if (!seaLabAllFeaturesComputation)
        return false;
     resultMatrix = seaLabAllFeaturesComputation->GetSeaLabSimulationWorker()->m_ResultMatrix;
    return true;
}
bool ViewProviderSeaLabFeatureLocationDistribution::getComputationResultComplexMatrix(cx_mat& resultMatrix_cx)
{
     if (!seaLabAllFeaturesComputation)
        return false;
     resultMatrix_cx = seaLabAllFeaturesComputation->GetSeaLabSimulationWorker()->m_ResultMatrix_cx;
    return true;
}

bool ViewProviderSeaLabFeatureLocationDistribution::runFeatureMethod(const QString function, const char* complexNumberDisplay)
{  
    ActivateFeature();
    SeaLab::SeaLabSimulation* sim = static_cast<SeaLab::SeaLabSimulation*>(SeaLabGui::SeaLabSimulationObserver::instance()->active());
    if (!sim) {Base::Console().Warning("No valide active simulation found.\n");return false;}
    seaLabAllFeaturesComputation = new SeaLabAllFeaturesComputation(sim);
    seaLabAllFeaturesComputation->startSimulationWorker(function, complexNumberDisplay);
    seaLabAllFeaturesComputation->getSeaLabSimulationThread()->start();
    return true;
}

