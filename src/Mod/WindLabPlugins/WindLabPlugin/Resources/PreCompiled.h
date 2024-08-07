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


#ifndef __PRECOMPILED_GUI__
#define __PRECOMPILED_GUI__

#include <RPSConfig.h>

#ifdef _MSC_VER
#   pragma warning(disable : 4005)
#endif

#ifdef _PreComp_

// standard
#include <iostream>
#include <cassert>
#include <cmath>

#include <cstdlib>

// STL
#include <vector>
#include <map>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <stack>
#include <queue>
#include <bitset>

// Boost
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/bind/bind.hpp>
#include <boost/scoped_ptr.hpp>

#ifdef RPS_OS_WIN32
# define NOMINMAX
# include <windows.h>
#endif

// Qt Toolkit
#ifndef __QtAll__
# include <Gui/QtAll.h>
#endif

# include <QGuiApplication>
# include <QMessageBox>
#include <qdebug.h>
#include <QString>


#endif //_PreComp_

#endif // __PRECOMPILED_GUI__
