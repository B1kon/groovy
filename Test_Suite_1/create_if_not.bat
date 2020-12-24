echo off
set P=%~1
echo P=%P%
set D=%~2
echo D=%D%

set goto_END=0
set cnt=0
for /F %%g in ('dir /b %P%') do (
if %%g == %D% set /a cnt=cnt+1

)
echo cnt=%cnt%
::echo "=========================1111"

set cmd=mkdir %P%\%D%
if %cnt% == 0 ( echo "mkdir %P%\%D%" )
if %cnt% == 0 %cmd% || set goto_END=1

::echo "=========================2222"