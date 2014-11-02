@echo off

setlocal
set error=0

call :CompileShader ovr vs OVR_VertexShader
call :CompileShader ovr ps DistortionCorrectionPixelShader

call :CompileShader vs_standard vs main

call :CompileShader ps_solid ps main

echo.

if %error% == 0 (
    echo Shaders compiled ok
) else (
    echo There were shader compilation errors!
)

endlocal
exit /b

:CompileShader
set fxc="C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc" /nologo %1.fx /T%2_4_0_level_9_1 /Zpc /Qstrip_reflect /Qstrip_debug /E%3 /FhCompiled\%1_%3.inc /Vn%1_%3
echo.
echo %fxc%
%fxc% || set error=1
exit /b
