:: Used to automate everything for Alex so he can be lazy.

For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%a-%%b)

call win64_release_msvc12.bat
set filename=E:\simulationcraft\simc-601-alpha-win64-%mydate%.zip
call start winscp /command "open downloads" "put %filename% -nopreservetime -nopermissions" "exit"

call win32_release_msvc12.bat
set filename=E:\simulationcraft\simc-601-alpha-win32-%mydate%.zip
winscp /command "open downloads" "put %filename% -nopreservetime -nopermissions" "exit"
pause