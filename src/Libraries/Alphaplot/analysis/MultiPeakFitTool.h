/***************************************************************************
    File                 : MultiPeakFitTool.h
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006,2007 by Ion Vasilief,
                           Tilman Benkert, Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Plot tool for doing multi-peak fitting.

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
#ifndef MULTI_PEAK_FIT_TOOL
#define MULTI_PEAK_FIT_TOOL

#include <QObject>
#include "MultiPeakFit.h"

namespace Gui {
    class MainWindow;
}

/*! Plot tool for doing multi-peak fitting.
 *
 * This class can be seen as a user-interface wrapper around MultiPeakFit,
 * providing functionality for visually
 * selecting estimated peak positions and finally executing the fit.
 *
 * Maybe some other parts of the multi-peak fitting process (namely, specifying
 * the number of peaks and optionally
 * the peak profile) could be moved here as well.
 */

class  AlphaplotExport PlotToolInterface {
 public:
  enum RTTI {
    DataPicker,
    ScreenPicker,
    LineProfile,
    MultiPeak,
    RangeSelector,
    TranslateCurve
  };
  virtual RTTI rtti() const = 0;

  PlotToolInterface(AxisRect2D *axisrect);
  virtual ~PlotToolInterface();

 protected:
  AxisRect2D *axisrect_;
};

class AlphaplotExport MultiPeakFitTool : public QObject, public PlotToolInterface {
  Q_OBJECT
 public:
  MultiPeakFitTool(AxisRect2D *axisrect, Gui::MainWindow *app,
                   MultiPeakFit::PeakProfile profile, int num_peaks,
                   const QObject *status_target, const char *status_slot);
  virtual RTTI rtti() const { return MultiPeak; }
  virtual ~MultiPeakFitTool();
 Q_SIGNALS:
  /*! Emitted whenever a new message should be presented to the user.
   *
   * You don't have to connect to this signal if you alreay specified a reciever
   * during initialization.
   */
  void statusText(const QString &);
 protected Q_SLOTS:
  void selectPeak(PlotData::AssociatedData *associateddata, int point_index);

 private:
  void finalize();
  MultiPeakFit::PeakProfile d_profile;
  int d_num_peaks, d_selected_peaks;
  //DataPickerTool *d_picker_tool;
  MultiPeakFit *d_fit;
};

#endif  // MULTI_PEAK_FIT_TOOL
