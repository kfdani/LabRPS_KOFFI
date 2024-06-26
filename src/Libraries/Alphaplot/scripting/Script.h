/***************************************************************************
    File                 : Script.h
    Project              : AlphaPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief,
                           Tilman Benkert,
                           Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Scripting abstraction layer

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
#ifndef SCRIPT_H
#define SCRIPT_H

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QStringList>
#include <QEvent>
#include <QDebug>

#include "customevents.h"
#include "ScriptingEnv.h"
#include <Libraries/Alphaplot/AlphaplotGlobal.h>

class QMainWindow;

//! A chunk of scripting code. Abstract.
/**
 * Script objects represent a chunk of code, possibly together with local
 * variables. The code may be changed and executed multiple times during the
 * lifetime of an object.
 */
class AlphaplotExport Script: public QObject
{
  Q_OBJECT

 public:
  Script(ScriptingEnv *env, const QString &code, QObject *context = 0,
         const QString &name = QString::fromLatin1("<input>"))
      : Env(env), Code(code), Name(name), compiled(notCompiled) {
    Env->incref();
    Context = context;
    EmitErrors = true;
  }
  ~Script() { Env->decref(); }

  //! Return the code that will be executed when calling exec() or eval()
  const QString code() const { return Code; }
  //! Return the context in which the code is to be executed.
  const QObject *context() const { return Context; }
  //! Like QObject::name, but with unicode support.
  const QString name() const { return Name; }
  //! Return whether errors / exceptions are to be emitted or silently ignored
  bool emitErrors() const { return EmitErrors; }
  //! Append to the code that will be executed when calling exec() or eval()
  virtual void addCode(const QString &code) {
    Code.append(code);
    compiled = notCompiled;
    Q_EMIT codeChanged();
    qDebug() << "code add";
  }
  //! Set the code that will be executed when calling exec() or eval()
  virtual void setCode(const QString &code) {
    Code = code;
    compiled = notCompiled;
    Q_EMIT codeChanged();
    qDebug() << "code set";
  }
  //! Set the context in which the code is to be executed.
  virtual void setContext(QObject *context) {
    Context = context;
    compiled = notCompiled;
  }
  //! Like QObject::setName, but with unicode support.
  void setName(const QString &name) {
    Name = name;
    compiled = notCompiled;
  }
  //! Set whether errors / exceptions are to be emitted or silently ignored
  void setEmitErrors(bool yes) { EmitErrors = yes; }

 public Q_SLOTS:
  //! Compile the Code. Return true if the implementation doesn't support
  //! compilation.
  virtual bool compile(bool for_eval = true);
  //! Evaluate the Code, returning QVariant() on an error / exception.
  virtual QVariant eval();
  //! Execute the Code, returning false on an error / exception.
  virtual bool exec();

  // local variables
  virtual bool setQObject(const QObject *, const char *) { return false; }
  virtual bool setInt(int, const char *) { return false; }
  virtual bool setDouble(double, const char *) { return false; }

 Q_SIGNALS:
  //! This is emitted whenever the code to be executed by exec() and eval() is
  //! changed.
  void codeChanged();
  //! signal an error condition / exception
  void error(const QString &message, const QString &scriptName, int lineNumber);
  //! output generated by the code
  void print(const QString &output);

 protected:
  ScriptingEnv *Env;
  QString Code, Name;
  QObject *Context;
  enum compileStatus { notCompiled, isCompiled, compileErr } compiled;
  bool EmitErrors;

  void emit_error(const QString &message, int lineNumber) {
    if (EmitErrors) Q_EMIT error(message, Name, lineNumber);
  }
};

//! keeps a static list of available interpreters and instantiates them on
//! demand
class AlphaplotExport ScriptingLangManager
{
 public:
  //! Return an instance of the first implementation we can find.
  static ScriptingEnv *newEnv(QMainWindow *parent);
  //! Return an instance of the implementation specified by name, NULL on
  //! failure.
  static ScriptingEnv *newEnv(const char *name, QMainWindow *parent);
  //! Return the names of available implementations.
  static QStringList languages();
  //! Return the number of available implementations.
  static int numLanguages();

 private:
  typedef ScriptingEnv *(*ScriptingEnvConstructor)(QMainWindow *);
  typedef struct {
    const char *name;
    ScriptingEnvConstructor constructor;
  } ScriptingLang;
  //! global registry of available languages
  static ScriptingLang langs[];
};

//! notify an object that it should update its scripting environment (see class
//! scripted)
class AlphaplotExport ScriptingChangeEvent: public QEvent
{
 public:
  ScriptingChangeEvent(ScriptingEnv *e)
      : QEvent(SCRIPTING_CHANGE_EVENT), env(e) {}
  ScriptingEnv *scriptingEnv() const { return env; }
  Type type() const { return SCRIPTING_CHANGE_EVENT; }

 private:
  ScriptingEnv *env;
};

//! Interface for maintaining a reference to the current ScriptingEnv
/**
 * Every class that wants to use a ScriptingEnv should subclass this one and
 * implement slot customEvent(QEvent*) such that it forwards any
 * ScriptingChangeEvents to scripted::scriptingChangeEvent.
 */
class AlphaplotExport scripted
{
 public:
  scripted(ScriptingEnv *env);
  ~scripted();
  void scriptingChangeEvent(ScriptingChangeEvent *);

 protected:
  ScriptingEnv *scriptEnv;
};

#endif  // SCRIPT_H
