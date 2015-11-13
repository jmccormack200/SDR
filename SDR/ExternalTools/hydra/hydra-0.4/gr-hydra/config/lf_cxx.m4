dnl Autoconf support for C++
dnl Copyright (C) 1988 Eleftherios Gkioulekas <lf@amath.washington.edu>
dnl  
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software 
dnl Foundation, Inc., 51 Franklin Street, Boston, MA 02110-1301, USA.
dnl 
dnl As a special exception to the GNU General Public License, if you 
dnl distribute this file as part of a program that contains a configuration 
dnl script generated by Autoconf, you may include it under the same 
dnl distribution terms that you use for the rest of that program.

# -----------------------------------------------------------------
# This macro should be called to configure your C++ compiler.
# When called, the macro does the following things:
# 1. It finds an appropriate C++ compiler
#    If you passed the flag --with-cxx=foo, then it uses that
#    particular compiler
# 2. Checks whether the compiler accepts the -g 
# ------------------------------------------------------------------

AC_DEFUN([LF_CONFIGURE_CXX],[
 AC_REQUIRE([AC_PROG_CXX])dnl
 AC_REQUIRE([AC_PROG_CXXCPP])dnl
 LF_CXX_PORTABILITY
])

# -----------------------------------------------------------------------
# This macro tests the C++ compiler for various portability problem.
# 1. Defines CXX_HAS_NO_BOOL if the compiler does not support the bool
#    data type
# 2. Defines CXX_HAS_BUGGY_FOR_LOOPS if the compiler has buggy
#    scoping for the for-loop
# 3. Defines USE_ASSERT if the user wants to use assertions
# -----------------------------------------------------------------------


AC_DEFUN([LF_CXX_PORTABILITY],[

  dnl
  dnl Check for common C++ portability problems
  dnl

  dnl AC_LANG_PUSH
  dnl AC_LANG_CPLUSPLUS
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS

  dnl Check whether we have bool
  AC_MSG_CHECKING(whether C++ has bool)
  AC_TRY_RUN([main() { bool b1=true; bool b2=false; }],
             [ AC_MSG_RESULT(yes) ],
             [ AC_MSG_RESULT(no)
               AC_DEFINE(CXX_HAS_NO_BOOL,[],[Define if C++ is missing bool type]) ],
             [ AC_MSG_WARN(Don't cross-compile)]
            )

  dnl Test whether C++ has buggy for-loops
  AC_MSG_CHECKING(whether C++ has buggy scoping in for-loops)
  AC_TRY_COMPILE([#include <iostream.h>], [
   for (int i=0;i<10;i++) { }
   for (int i=0;i<10;i++) { }
], [ AC_MSG_RESULT(no) ],
   [ AC_MSG_RESULT(yes)
     AC_DEFINE(CXX_HAS_BUGGY_FOR_LOOPS,[],[Define if for loop scoping is broken]) ])

  dnl Test whether the user wants to enable assertions
  AC_MSG_CHECKING(whether user wants assertions)
  AC_ARG_ENABLE(assert,
                [  --disable-assert        don't use cpp.h assert],
                [ AC_DEFINE(NDEBUG,[],[Define to disable asserts (don't doit!)])
                  AC_MSG_RESULT(no)  ],
                [ AC_MSG_RESULT(yes) ],
               )

  dnl Test whether C++ has std::isnan
  AC_MSG_CHECKING(whether C++ has std::isnan)
  AC_TRY_COMPILE([#include <cmath>], [
   std::isnan(0);
], [ AC_MSG_RESULT(yes)
	AC_DEFINE(CXX_HAS_STD_ISNAN,[],[Define if has std::isnan]) ],
   [ AC_MSG_RESULT(no) ])

  dnl Done with the portability checks
  dnl AC_LANG_POP([C++])
  AC_LANG_RESTORE
])

AH_BOTTOM([// Workaround for compilers with buggy for-loop scoping
// That's quite a few compilers actually including recent versions of
// Dec Alpha cxx, HP-UX CC and SGI CC.
// The trivial "if" statement provides the correct scoping to the 
// for loop

#ifdef CXX_HAS_BUGGY_FOR_LOOPS
#undef for
#define for if(1) for
#endif
])

AH_BOTTOM([// If the C++ compiler we use doesn't have bool, then
// the following is a near-perfect work-around. 
// You must make sure your code does not depend on "int" and "bool"
// being two different types, in overloading for instance.

#ifdef CXX_HAS_NO_BOOL
#define bool int
#define true 1
#define false 0
#endif
])