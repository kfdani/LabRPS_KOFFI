/***************************************************************************
    File                 : ImportASCIIDialog.cpp
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006,2007 by Ion Vasilief,
                           Tilman Benkert, Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Import ASCII file(s) dialog

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
#include "PreCompiled.h"

#include "ImportASCIIDialog.h"
//#include "MainWindow.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRegExp>
#include <QMessageBox>
#include <QLabel>
#include <QCloseEvent>



ImportASCIIDialog::ImportASCIIDialog(bool import_mode_enabled, bool strip_spaces, bool simplify_spaces,
                                     int ignoredLines, bool renameColumns, QString columnSeparator, bool convert_to_numeric,
                                     QLocale ASCII_import_locale,QWidget* parent, bool extended,
                                     Qt::WindowFlags flags)
    : ExtensibleFileDialog(parent, extended, flags) {
  setWindowTitle(tr("Import ASCII File(s)"));

  QStringList filters;
  filters << tr("All files") + QString::fromLatin1(" (*)");
  filters << tr("Text files") + QString::fromLatin1(" (*.TXT *.txt)");
  filters << tr("Data files") + QString::fromLatin1(" (*.DAT *.dat)");
  filters << tr("Comma Separated Values") + QString::fromLatin1(" (*.CSV *.csv)");
  filters << tr("Tab Separated Values") + QString::fromLatin1(" (*.TSV *.tsv)");
  setNameFilters(filters);

  setFileMode(QFileDialog::ExistingFiles);

  initAdvancedOptions();
  d_import_mode->setEnabled(import_mode_enabled);
  setExtensionWidget(d_advanced_options);

  // get rembered option values
  d_strip_spaces->setChecked(strip_spaces);
  d_simplify_spaces->setChecked(simplify_spaces);
  d_ignored_lines->setValue(ignoredLines);
  d_rename_columns->setChecked(renameColumns);
  setColumnSeparator(columnSeparator);

  if (ASCII_import_locale.name() == QLocale::c().name())
    boxDecimalSeparator->setCurrentIndex(1);
  else if (ASCII_import_locale.name() == QLocale(QLocale::German).name())
    boxDecimalSeparator->setCurrentIndex(2);
  else if (ASCII_import_locale.name() == QLocale(QLocale::French).name())
    boxDecimalSeparator->setCurrentIndex(3);
  boxDecimalSeparator->setEnabled(convert_to_numeric);
  d_convert_to_numeric->setChecked(convert_to_numeric);

  connect(d_import_mode, SIGNAL(currentIndexChanged(int)), this,
          SLOT(updateImportMode(int)));//koffa the way
  /*connect(d_import_mode, qOverload<int>(&QComboBox::currentIndexChanged), this,
          &ImportASCIIDialog::updateImportMode);*/
}

void ImportASCIIDialog::initAdvancedOptions() {
  d_advanced_options = new QGroupBox();
  QVBoxLayout *main_layout = new QVBoxLayout(d_advanced_options);
  QGridLayout *advanced_layout = new QGridLayout();
  main_layout->addLayout(advanced_layout);

  advanced_layout->addWidget(new QLabel(tr("Import each file as: ")), 0, 0);
  d_import_mode = new QComboBox();
  // Important: Keep this in sync with the ImportMode enum.
  d_import_mode->addItem(tr("New Table"));
  d_import_mode->addItem(tr("New Columns"));
  d_import_mode->addItem(tr("New Rows"));
  d_import_mode->addItem(tr("Overwrite Current Table"));
  advanced_layout->addWidget(d_import_mode, 0, 1);

  QLabel *label_column_separator = new QLabel(tr("Separator:"));
  advanced_layout->addWidget(label_column_separator, 1, 0);
  d_column_separator = new QComboBox();
  d_column_separator->addItem(tr("TAB"));
  d_column_separator->addItem(tr("SPACE"));
  d_column_separator->addItem(QString::fromLatin1(";") + tr("TAB"));
  d_column_separator->addItem(QString::fromLatin1(",") + tr("TAB"));
  d_column_separator->addItem(QString::fromLatin1(";") + tr("SPACE"));
  d_column_separator->addItem(QString::fromLatin1(",") + tr("SPACE"));
  d_column_separator->addItem(QString::fromLatin1(";"));
  d_column_separator->addItem(QString::fromLatin1(","));
  d_column_separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  d_column_separator->setEditable(true);
  advanced_layout->addWidget(d_column_separator, 1, 1);
  // context-sensitive help
  QString help_column_separator =
      tr("The column separator can be customized. \nThe following special "
         "codes can be used:\n\\t for a TAB character \n\\s for a SPACE");
  help_column_separator +=
      QString::fromLatin1("\n") +
      tr("The separator must not contain the following characters: \n0-9eE.+-");
  d_column_separator->setWhatsThis(help_column_separator);
  label_column_separator->setToolTip(help_column_separator);
  d_column_separator->setToolTip(help_column_separator);
  label_column_separator->setWhatsThis(help_column_separator);

  QLabel *label_ignore_lines = new QLabel(tr("Ignore first"));
  advanced_layout->addWidget(label_ignore_lines, 2, 0);
  d_ignored_lines = new QSpinBox();
  d_ignored_lines->setRange(0, 10000);
  d_ignored_lines->setSuffix(QString::fromLatin1(" ") + tr("lines"));
  d_ignored_lines->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  advanced_layout->addWidget(d_ignored_lines, 2, 1);

  d_rename_columns = new QCheckBox(tr("Use first row to &name columns"));
  advanced_layout->addWidget(d_rename_columns, 0, 2, 1, 2);

  d_strip_spaces = new QCheckBox(tr("&Remove white spaces from line ends"));
  advanced_layout->addWidget(d_strip_spaces, 1, 2, 1, 2);
  // context-sensitive help
  QString help_strip_spaces =
      tr("By checking this option all white spaces will be \nremoved from the "
         "beginning and the end of \nthe lines in the ASCII file.",
         "when translating this check the what's this functions and tool tips "
         "to place the '\\n's correctly");
  help_strip_spaces +=
      QString::fromLatin1("\n\n") +
      tr("Warning: checking this option leads to column \noverlaping if the "
         "columns in the ASCII file don't \nhave the same number of rows.");
  help_strip_spaces +=
      QString::fromLatin1("\n") + tr("To avoid this problem you should precisely \ndefine the "
                "column separator using TAB and \nSPACE characters.",
                "when translating this check the what's this functions and "
                "tool tips to place the '\\n's correctly");
  d_strip_spaces->setWhatsThis(help_strip_spaces);
  d_strip_spaces->setToolTip(help_strip_spaces);

  d_simplify_spaces = new QCheckBox(tr("&Simplify white spaces"));
  advanced_layout->addWidget(d_simplify_spaces, 2, 2, 1, 2);
  // context-sensitive help
  QString help_simplify_spaces =
      tr("By checking this option all white spaces will be \nremoved from the "
         "beginning and the end of the \nlines and each sequence of internal "
         "\nwhitespaces (including the TAB character) will \nbe replaced with "
         "a single space.",
         "when translating this check the what's this functions and tool tips "
         "to place the '\\n's correctly");
  help_simplify_spaces +=
      QString::fromLatin1("\n\n") + tr("Warning: checking this option leads to column \noverlaping "
                  "if the columns in the ASCII file don't \nhave the same "
                  "number of rows.",
                  "when translating this check the what's this functions and "
                  "tool tips to place the '\\n's correctly");
  help_simplify_spaces +=
      QString::fromLatin1("\n") + tr("To avoid this problem you should precisely \ndefine the "
                "column separator using TAB and \nSPACE characters.",
                "when translating this check the what's this functions and "
                "tool tips to place the '\\n's correctly");
  d_simplify_spaces->setWhatsThis(help_simplify_spaces);
  d_simplify_spaces->setToolTip(help_simplify_spaces);

  d_convert_to_numeric = new QCheckBox(tr("&Numeric data"));
  advanced_layout->addWidget(d_convert_to_numeric, 3, 0, 1, 2);

  advanced_layout->addWidget(new QLabel(tr("Decimal Separators")), 3, 1);
  boxDecimalSeparator = new QComboBox();
  boxDecimalSeparator->addItem(
      tr("default") + QString::fromLatin1(" (") + QLocale::system().toString(1000.0, 'f', 1) + QString::fromLatin1(")"));
  boxDecimalSeparator->addItem(QLocale::c().toString(1000.0, 'f', 1));
  boxDecimalSeparator->addItem(
      QLocale(QLocale::German).toString(1000.0, 'f', 1));
  boxDecimalSeparator->addItem(
      QLocale(QLocale::French).toString(1000.0, 'f', 1));
  connect(d_convert_to_numeric, SIGNAL(toggled(bool)), boxDecimalSeparator,
          SLOT(setEnabled(bool)));
  advanced_layout->addWidget(boxDecimalSeparator, 3, 2);

  QHBoxLayout *meta_options_layout = new QHBoxLayout();
  d_remember_options = new QCheckBox(tr("Re&member the above options"));
  meta_options_layout->addWidget(d_remember_options);
  d_help_button = new QPushButton(tr("&Help"));
  connect(d_help_button, SIGNAL(clicked()), this, SLOT(displayHelp()));
  meta_options_layout->addStretch();
  meta_options_layout->addWidget(d_help_button);
  main_layout->addLayout(meta_options_layout);
}

void ImportASCIIDialog::setColumnSeparator(const QString &sep) {
  if (sep == QString::fromLatin1("\t"))
    d_column_separator->setCurrentIndex(0);
  else if (sep == QString::fromLatin1(" "))
    d_column_separator->setCurrentIndex(1);
  else if (sep == QString::fromLatin1(";\t"))
    d_column_separator->setCurrentIndex(2);
  else if (sep == QString::fromLatin1(",\t"))
    d_column_separator->setCurrentIndex(3);
  else if (sep == QString::fromLatin1("; "))
    d_column_separator->setCurrentIndex(4);
  else if (sep == QString::fromLatin1(", "))
    d_column_separator->setCurrentIndex(5);
  else if (sep == QString::fromLatin1(";"))
    d_column_separator->setCurrentIndex(6);
  else if (sep == QString::fromLatin1(","))
    d_column_separator->setCurrentIndex(7);
  else {
    QString separator = sep;
    d_column_separator->setEditText(
        separator.replace(QString::fromLatin1(" "), QString::fromLatin1("\\s").replace(QString::fromLatin1("\t"), QString::fromLatin1("\\t"))));
  }
}

const QString ImportASCIIDialog::columnSeparator() const {
  QString sep = d_column_separator->currentText();

  if (d_simplify_spaces->isChecked())
    sep.replace(tr("TAB"), QString::fromLatin1(" "), Qt::CaseInsensitive);
  else
    sep.replace(tr("TAB"), QString::fromLatin1("\t"), Qt::CaseInsensitive);

  sep.replace(tr("SPACE"), QString::fromLatin1(" "), Qt::CaseInsensitive);
  sep.replace(QString::fromLatin1("\\s"), QString::fromLatin1(" "));
  sep.replace(QString::fromLatin1("\\t"), QString::fromLatin1("\t"));

  /* TODO
  if (sep.contains(QRegExp("[0-9.eE+-]")))
          QMessageBox::warning(this, tr("Import options error"),
                          tr("The separator must not contain the following
  characters: 0-9eE.+-"));
  */

  return sep;
}

void ImportASCIIDialog::displayHelp() {
  QString s =
      tr("The column separator can be customized. The following special codes "
         "can be used:\n\\t for a TAB character \n\\s for a SPACE");
  s += QString::fromLatin1("\n") +
       tr("The separator must not contain the following characters: 0-9eE.+-") +
       QString::fromLatin1("\n\n");
  s += tr("Remove white spaces from line ends") + QString::fromLatin1(":\n");
  s += tr("By checking this option all white spaces will be removed from the "
          "beginning and the end of the lines in the ASCII file.") +
       QString::fromLatin1("\n\n");
  s += tr("Simplify white spaces") + QString::fromLatin1(":\n");
  s +=
      tr("By checking this option each sequence of internal whitespaces "
         "(including the TAB character) will be replaced with a single space.");
  s +=
      tr("By checking this option all white spaces will be removed from the "
         "beginning and the end of the lines and each sequence of internal "
         "whitespaces (including the TAB character) will be replaced with a "
         "single space.");

  s += QString::fromLatin1("\n\n") +
       tr("Warning: using these two last options leads to column overlaping if "
          "the columns in the ASCII file don't have the same number of rows.");
  s += QString::fromLatin1("\n") + tr("To avoid this problem you should precisely define the column "
                 "separator using TAB and SPACE characters.");

  QMessageBox::about(this, tr("Help"), s);
}

void ImportASCIIDialog::updateImportMode(int mode) {
  if (mode == Overwrite)
    setFileMode(QFileDialog::ExistingFile);
  else
    setFileMode(QFileDialog::ExistingFiles);
}

void ImportASCIIDialog::closeEvent(QCloseEvent *e) {
 /* Gui::MainWindow *app = (Gui::MainWindow *)this->parent();
  if (app) {
    app->d_extended_import_ASCII_dialog = this->isExtended();
    app->d_ASCII_file_filter = this->selectedNameFilter();
  }

  e->accept();*///koffa the way
}

QLocale ImportASCIIDialog::decimalSeparators() {
  QLocale locale;
  switch (boxDecimalSeparator->currentIndex()) {
    case 0:
      locale = QLocale::system();
      break;
    case 1:
      locale = QLocale::c();
      break;
    case 2:
      locale = QLocale(QLocale::German);
      break;
    case 3:
      locale = QLocale(QLocale::French);
      break;
  }
  return locale;
}
