/***************************************************************************
    File                 : OpenProjectDialog.cpp
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke, Ion Vasilief
    Email (use @ for *)  : knut.franke*gmx.de, ion_vasilief*yahoo.fr
    Description          : Dialog for opening project files.

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
#include "Precompiled.h"

#include "OpenProjectDialog.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>

OpenProjectDialog::OpenProjectDialog(QWidget *parent, bool extended,
                                     Qt::WindowFlags flags)
    : ExtensibleFileDialog(parent, extended, flags) {
  setWindowTitle(tr("Open Project"));
  setFileMode(ExistingFile);
  QStringList filters;
  filters << tr("AlphaPlot project") + QString::fromLatin1(" (*.aproj)")
          << tr("Compressed AlphaPlot project") + QString::fromLatin1(" (*.aproj.gz)")
          << tr("Backup files") + QString::fromLatin1(" (*.aproj~ *.aproj.gz~)")
          //<< tr("Python Source") + " (*.py *.PY)"
          << tr("All files") + QString::fromLatin1(" (*)");
  setNameFilters(filters);

  QWidget *advanced_options = new QWidget();
  QHBoxLayout *advanced_layout = new QHBoxLayout();
  advanced_options->setLayout(advanced_layout);
  advanced_layout->addWidget(new QLabel(tr("Open As")));
  d_open_mode = new QComboBox();
  // Important: Keep this is sync with enum OpenMode.
  d_open_mode->addItem(tr("New Project Window"));
  d_open_mode->addItem(tr("New Folder"));
  advanced_layout->addWidget(d_open_mode);
  setExtensionWidget(advanced_options);

  connect(this, SIGNAL(filterSelected(const QString &)), this,
          SLOT(updateAdvancedOptions(const QString &)));
  updateAdvancedOptions(selectedNameFilter());
}

void OpenProjectDialog::updateAdvancedOptions(const QString &filter) {
  if (filter.contains(QString::fromLatin1("*.ogm")) || filter.contains(QString::fromLatin1("*.ogw"))) {
    d_extension_toggle->setChecked(false);
    d_extension_toggle->setEnabled(false);
    return;
  }
  d_extension_toggle->setEnabled(true);
}

void OpenProjectDialog::closeEvent(QCloseEvent *e) {
  /*if (isExtendable()) {
    Gui::MainWindow *app = (Gui::MainWindow *)this->parent();
    if (app) app->d_extended_open_dialog = this->isExtended();
  }*/

  e->accept();
}
