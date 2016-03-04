#
# Copyright 1997 - 1999 Microsoft Corporation.  All rights reserved.
#
#
# Include SdkPropBld.Mak in order to get standard build options, environment
#  variables, and macros.  The sdkpropbld.mak file is installed with the SDK 
#  in the include directory.
# 

!ifdef clean
makeopts = $(makeopts) clean=1
!endif # clean

!ifdef nodebug
makeopts = $(makeopts) nodebug=1
!endif

!ifdef chk
makeopts = $(makeopts) chk=1
!endif # chk
