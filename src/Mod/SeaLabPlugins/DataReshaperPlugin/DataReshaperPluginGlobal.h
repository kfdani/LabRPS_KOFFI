/***************************************************************************
 *   Copyright (c) 2019 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the LabRPS development system.              *
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

#include <RPSGlobal.h>

#ifndef DATARESHAPERPLUGIN_GLOBAL_H
#define DATARESHAPERPLUGIN_GLOBAL_H


// DataReshaperPlugin
#ifndef DataReshaperPluginExport
#ifdef DataReshaperPlugin_EXPORTS
#  define DataReshaperPluginExport      LABRPS_DECL_EXPORT
#else
#  define DataReshaperPluginExport      LABRPS_DECL_IMPORT
#endif
#endif

// DataReshaperPluginGui
#ifndef DataReshaperPluginGuiExport
#ifdef DataReshaperPluginGui_EXPORTS
#  define DataReshaperPluginGuiExport   LABRPS_DECL_EXPORT
#else
#  define DataReshaperPluginGuiExport   LABRPS_DECL_IMPORT
#endif
#endif

#endif //DATARESHAPERPLUGIN_GLOBAL_H
