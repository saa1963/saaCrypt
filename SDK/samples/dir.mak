    @IF EXIST $(@D)\makefile <<nmaketmp.bat
    @cd $(@D)
    @if NOT "%CD%"== ""  echo *** %CD% *** 
    @if "%CD%" == ""  echo *** $(SDKPROJ) *** 
    @if not "-$(dest)"=="-" (
    	echo $(MAKE) -nologo /$(MAKEFLAGS) $(makeopts) dest="$(dest)\$(@D)" sources
    	$(MAKE) -nologo /$(MAKEFLAGS) $(makeopts) dest="$(dest)\$(@D)" csources
    ) else (
    	echo $(MAKE) -nologo /$(MAKEFLAGS) $(makeopts)
    	$(MAKE) -nologo /$(MAKEFLAGS) $(makeopts)
    )
    @IF errorlevel 1 @(@if "-$(chk)"=="-1" @exit /b %errorlevel%)
    @cd ..
<<
