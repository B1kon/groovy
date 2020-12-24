
grep Pass *_res_* |grep -c Pass > f 
 set s=0 
 for /F %x in (f) do @(set /a s+=1 >nul) 
 if  %s% == 1 ( echo OK > result_003_05)
