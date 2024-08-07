/***************************************************************************
    File                 : InterpolationDialog.h
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Interpolation options dialog

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
#ifndef INTERPOLATIONDIALOG_H
#define INTERPOLATIONDIALOG_H

#include <QDialog>
#include <Libraries/Alphaplot/AlphaplotGlobal.h>

class QPushButton;
class QLineEdit;
class QComboBox;
class QSpinBox;
class AxisRect2D;
class ColorBox;

namespace Gui {
    class MainWindow;
}

//! Interpolation options dialog
class AlphaplotExport InterpolationDialog : public QDialog {
  Q_OBJECT

 public:
  InterpolationDialog(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::Widget);
  ~InterpolationDialog() {}

  QPushButton* buttonFit;
  QPushButton* buttonCancel;
  QComboBox* boxName;
  QComboBox* boxMethod;
  QSpinBox* boxPoints;
  QLineEdit* boxStart;
  QLineEdit* boxEnd;
  ColorBox* boxColor;

 public Q_SLOTS:
  void activateCurve(const QString& curveName);
  void setAxisRect(AxisRect2D* axisrect);
  void interpolate();
  void changeDataRange();

 private:
  AxisRect2D *axisrect_;
  Gui::MainWindow *app_;
  double xmin_;
  double xmax_;
};

#endif  // INTERPOLATIONDIALOG_H
