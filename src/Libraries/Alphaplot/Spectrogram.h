/***************************************************************************
        File                 : Spectrogram.h
        Project              : AlphaPlot
--------------------------------------------------------------------
        Copyright            : (C) 2006 by Ion Vasilief
        Email (use @ for *)  : ion_vasilief*yahoo.fr
        Description          : AlphaPlot's Spectrogram Class
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

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <Libraries/Alphaplot/Matrix.h>
#include <cmath>
#include <limits>

class MatrixData;

class Spectrogram {
 public:
  Spectrogram();

  enum ColorMapPolicy { GrayScale, Default, Custom };

  Spectrogram *copy();
  Matrix *matrix() { return d_matrix; }

  int levels() { return  1; }
  void setLevelsNumber(int levels);

  bool hasColorScale();
  int colorScaleAxis() { return color_axis; }
  void showColorScale(int axis, bool on = true);

  int colorBarWidth();
  void setColorBarWidth(int width);

  void setGrayScale();
  void setDefaultColorMap();

  void updateData(Matrix *m);

  //! Used when saving a project file
  QString saveToString();

  ColorMapPolicy colorMapPolicy() { return color_map_policy; }

 protected:
  //! Pointer to the source data matrix
  Matrix *d_matrix;

  //! Axis used to display the color scale
  int color_axis;

  //! Flags
  ColorMapPolicy color_map_policy;
};

class MatrixData {
 public:
  MatrixData(Matrix *m) : d_matrix(m) {
    n_rows = d_matrix->numRows();
    n_cols = d_matrix->numCols();

    d_m = new double *[n_rows];
    for (int l = 0; l < n_rows; ++l) d_m[l] = new double[n_cols];

    min_z = std::numeric_limits<double>::max();
    max_z = -std::numeric_limits<double>::max();
    for (int i = 0; i < n_rows; i++)
      for (int j = 0; j < n_cols; j++) {
        // replace NaNs and Infs with average of neighbours
        if (std::isfinite(d_matrix->cell(i, j)))
          d_m[i][j] = d_matrix->cell(i, j);
        else {
          double av = 0;
          unsigned cnt = 0;
          for (int ii = -1; ii <= 1; ii += 2)
            for (int jj = -1; jj <= 1; jj += 2)
              if (std::isfinite(d_matrix->cell(i + ii, j + jj))) {
                av += d_matrix->cell(i + ii, j + jj);
                cnt++;
              }
          if (cnt > 0) av /= cnt;
          d_m[i][j] = av;
        }
        min_z = std::min(min_z, d_m[i][j]);
        max_z = std::max(max_z, d_m[i][j]);
      }

    // m->range(&min_z, &max_z);

    x_start = d_matrix->xStart();
    dx = (d_matrix->xEnd() - x_start) / (double)n_cols;

    y_start = d_matrix->yStart();
    dy = (d_matrix->yEnd() - y_start) / (double)n_rows;
  }

  ~MatrixData() {
    for (int i = 0; i < n_rows; i++) delete[] d_m[i];

    delete[] d_m;
  }

 private:
  //! Pointer to the source data matrix
  Matrix *d_matrix;

  //! Vector used to store in memory the data from the source matrix window
  double **d_m;

  //! Data size
  int n_rows, n_cols;

  //! Min and max values in the source data matrix
  double min_z, max_z;

  //! Data resolution in x(columns) and y(rows)
  double dx, dy;

  //! X axis left value in the data matrix
  double x_start;

  //! Y axis bottom value in the data matrix
  double y_start;
};

#endif  // SPECTROGRAM_H
