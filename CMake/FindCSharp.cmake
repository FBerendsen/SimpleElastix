#
# A CMake Module for finding and using C# (.NET and Mono).
#
# The following variables are set:
#   CSHARP_FOUND - set to ON if C# is found
#   CSHARP_USE_FILE - the path to the C# use file
#   CSHARP_TYPE - the type of the C# compiler (eg. ".NET" or "Mono")
#   CSHARP_VERSION - the version of the C# compiler (eg. "v4.0" or "2.10.2")
#   CSHARP_COMPILER - the path to the C# compiler executable (eg. "C:/Windows/Microsoft.NET/Framework/v4.0.30319/csc.exe" or "/usr/bin/gmcs")
#
# This file is based on the work of GDCM:
#   http://gdcm.svn.sf.net/viewvc/gdcm/trunk/CMake/FindCSharp.cmake
# Copyright (c) 2006-2010 Mathieu Malaterre <mathieu.malaterre@gmail.com>
#

# TODO: ADD ABILITY TO SELECT WHICH C# COMPILER eg. .NET or Mono (if both exist). For the moment, .NET is selected above Mono.

# Make sure find package macros are included
include( FindPackageHandleStandardArgs )

unset( CSHARP_COMPILER CACHE )
unset( CSHARP_INTERPRETER CACHE )
unset( CSHARP_TYPE CACHE )
unset( CSHARP_VERSION CACHE )
unset( CSHARP_FOUND CACHE )

if( WIN32 )
  find_package( DotNetFrameworkSdk )
  if (NOT CSHARP_DOTNET_FOUND)
    find_package( Mono )
  endif (NOT CSHARP_DOTNET_FOUND)
else( UNIX )
  find_package( Mono )
endif( WIN32 )

if( CSHARP_DOTNET_FOUND )
  set( CSHARP_TYPE ".NET" CACHE STRING "Using the .NET compiler" )
  set( CSHARP_VERSION ${CSHARP_DOTNET_VERSION} CACHE STRING "C# .NET compiler version" FORCE )
  set( CSHARP_COMPILER ${CSHARP_DOTNET_COMPILER_${CSHARP_DOTNET_VERSION}} CACHE STRING "Full path to .NET compiler" FORCE )
  set( CSHARP_INTERPRETER "" CACHE INTERNAL "" FORCE )
elseif( CSHARP_MONO_FOUND )
  set( CSHARP_TYPE "Mono" CACHE STRING "Using the Mono compiler" )
  set( CSHARP_VERSION ${CSHARP_MONO_VERSION} CACHE STRING "C# Mono compiler version" FORCE )
  set( CSHARP_COMPILER ${CSHARP_MONO_COMPILER_${CSHARP_MONO_VERSION}} CACHE STRING "Full path to Mono compiler" FORCE )
  set( CSHARP_INTERPRETER ${CSHARP_MONO_INTERPRETER_${CSHARP_MONO_VERSION}} CACHE STRING "Full path to Mono interpretor" FORCE )
endif( CSHARP_DOTNET_FOUND )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CSHARP DEFAULT_MSG CSHARP_TYPE CSHARP_VERSION CSHARP_COMPILER)

# Set the USE_FILE path
# http://public.kitware.com/Bug/view.php?id=7757
get_filename_component( current_list_path ${CMAKE_CURRENT_LIST_FILE} PATH )
set( CSHARP_USE_FILE ${current_list_path}/UseCSharp.cmake )
