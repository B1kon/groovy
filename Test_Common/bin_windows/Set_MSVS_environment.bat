
@rem ** set path on Visual Studio C **
@rem set TO_VS_COMNTOOLS="%VS110COMNTOOLS%"
@rem @set MSVC="%VS110COMNTOOLS%..\..\VC"
@rem **************************************

set goto_END=0
@if exist cl.exe (
    echo 'cl.exe exists'
    set /a goto_END=goto_END+1
)
@if  exist vcvars32.bat (
    echo 'vcvars32.bat exists'
	set /a goto_END=goto_END+1
)
@if %goto_END% == 2 goto END

echo "start %MSVC%\vcvarsall.bat"
@set MSVC="C:\Program Files (x86)\Microsoft Visual Studio 8\VC"
@call %MSVC%\vcvarsall.bat

:END
