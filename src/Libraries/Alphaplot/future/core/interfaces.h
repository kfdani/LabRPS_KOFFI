/***************************************************************************
    File                 : interfaces.h
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2008-2009 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Interfaces the kernel uses to talk to modules

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef INTERFACES_H
#define INTERFACES_H

//#include "future/lib/ConfigPageWidget.h"
#include "future/core/AbstractAspect.h"
#include "future/lib/XmlStreamReader.h"
#include <Libraries/Alphaplot/AlphaplotGlobal.h>

#include <QtPlugin>

class AbstractPart;
class QAction;
class QMenu;
class ProjectWindow;
class AbstractFilter;
class AbstractImportFilter;
class AbstractExportFilter;
class ActionManager;

//! Factory for AbstractPart objects.
class PartMaker {
 public:
  virtual ~PartMaker() {}
  //! The factory method.
  virtual AbstractPart *makePart() = 0;
  //! The action to be used for making new parts.
  /**
   * The caller takes care of connecting the action. If the parent argument is
   * zero, it
   * also recieves ownership of the action.
   * Implementations should only set things like name and icon.
   */
  virtual QAction *makeAction(QObject *parent) = 0;
};

Q_DECLARE_INTERFACE(PartMaker, "net.sf.AlphaPlot.partmaker/0.1")

//! Factory for filters.
/**
 * A FilterMaker introduces one or more filters to the kernel.
 */
class FilterMaker {
 public:
  virtual ~FilterMaker() {}
  virtual AbstractFilter *makeFilter(int id = 0) = 0;
  virtual int filterCount() const { return 1; }
  virtual QAction *makeAction(QObject *parent, int id = 0) = 0;
};

Q_DECLARE_INTERFACE(FilterMaker, "net.sf.AlphaPlot.filtermaker/0.1")

//! Factory for import/export filters.
class FileFormat {
 public:
  virtual ~FileFormat() {}
  virtual AbstractImportFilter *makeImportFilter() = 0;
  virtual AbstractExportFilter *makeExportFilter() = 0;
};

Q_DECLARE_INTERFACE(FileFormat, "net.sf.AlphaPlot.fileformat/0.1")

//! A module (typically a PartMaker) that has an ActionManager
class ActionManagerOwner {
 public:
  //! Return the action manager of the module
  virtual ActionManager *actionManager() = 0;
  //! Method that contains initialization that has to be done after loading the
  //! plugin
  virtual void initActionManager() {}
};

Q_DECLARE_INTERFACE(ActionManagerOwner,
                    "net.sf.AlphaPlot.actionmanagerowner/0.1")

//! A module with application-wide settings
class ConfigPageMaker {
 public:
  //virtual ConfigPageWidget *makeConfigPage() = 0;
  virtual QString configPageLabel() = 0;
  virtual void loadSettings() = 0;
  virtual void saveSettings() = 0;
  // TODO (maybe): icons instead of tabs to select the pages
  //		virtual QIcon icon() = 0;
};

Q_DECLARE_INTERFACE(ConfigPageMaker, "net.sf.AlphaPlot.configpagemaker/0.1")

//! Factory that creates an aspect out of an XML element.
class XmlElementAspectMaker {
 public:
  virtual ~XmlElementAspectMaker() {}
  //! Determine whether the loader can handle the given element.
  virtual bool canCreate(const QString &element_name) = 0;
  //! The factory method.
  virtual AbstractAspect *createAspectFromXml(XmlStreamReader *reader) = 0;
};

Q_DECLARE_INTERFACE(XmlElementAspectMaker,
                    "net.sf.AlphaPlot.xmlelementaspectmaker/0.1")

#endif  // INTERFACES_H
