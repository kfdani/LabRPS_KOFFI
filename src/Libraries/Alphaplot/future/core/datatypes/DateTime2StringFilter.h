/***************************************************************************
    File                 : DateTime2StringFilter.h
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Benkert,
                           Knut Franke
    Email (use @ for *)  : thzs*gmx.net, knut.franke*gmx.de
    Description          : Conversion filter QDateTime -> QString.

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
#ifndef DATE_TIME2STRING_FILTER_H
#define DATE_TIME2STRING_FILTER_H

#include <QDateTime>
#include <QRegExp>

#include <Libraries/Alphaplot/AlphaplotGlobal.h>

#include "future/core/AbstractSimpleFilter.h"

class DateTime2StringFilterSetFormatCmd;

//! Conversion filter QDateTime -> QString.
class AlphaplotExport DateTime2StringFilter: public AbstractSimpleFilter
{
  Q_OBJECT

 public:
  //! Standard constructor.
  explicit DateTime2StringFilter(QString format = QString::fromLatin1("yyyy-MM-dd hh:mm:ss.zzz"))
      : d_format(format) {}
  //! Set the format string to be used for conversion.
  void setFormat(const QString& format);

  //! Return the format string
  /**
   * The default format string is "yyyy-MM-dd hh:mm:ss.zzz".
   * \sa QDate::toString()
   */
  QString format() const { return d_format; }

  //! Return the data type of the column
  virtual AlphaPlot::ColumnDataType dataType() const {
    return AlphaPlot::TypeString;
  }

 Q_SIGNALS:
  void formatChanged();

 private:
  friend class DateTime2StringFilterSetFormatCmd;
  //! The format string.
  QString d_format;

 public:
  virtual QString textAt(int row) const {
    if (!d_inputs.value(0)) return QString();
    QDateTime input_value = d_inputs.value(0)->dateTimeAt(row);
    if (!input_value.date().isValid() && input_value.time().isValid())
      input_value.setDate(QDate(1900, 1, 1));
    return input_value.toString(d_format);
  }

  //! \name XML related functions
  //@{
  virtual void writeExtraAttributes(QXmlStreamWriter* writer) const;
  virtual bool load(XmlStreamReader* reader);
  //@}

 protected:
  //! Using typed ports: only DateTime inputs are accepted.
  virtual bool inputAcceptable(int, const AbstractColumn* source) {
    return source->dataType() == AlphaPlot::TypeDateTime;
  }
};

class AlphaplotExport DateTime2StringFilterSetFormatCmd: public QUndoCommand
{
 public:
  DateTime2StringFilterSetFormatCmd(DateTime2StringFilter* target,
                                    const QString& new_format);

  virtual void redo();
  virtual void undo();

 private:
  DateTime2StringFilter* d_target;
  QString d_other_format;
};

#endif  // DATE_TIME2STRING_FILTER_H
