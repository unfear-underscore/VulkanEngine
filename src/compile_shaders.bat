@echo off
cd shaders

@echo Compiling shaders...
@echo.

for /r %%i in (*.vert) do (
@echo Compiling vertex shader %%~nxi
C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%i -o compiled\%%~nxi.spv
)

for /r %%i in (*.frag) do (
@echo Compiling fragment shader %%~nxi
C:\VulkanSDK\1.3.268.0\Bin\glslc.exe %%i -o compiled\%%~nxi.spv
)

@echo.
@echo Compiled shaders!

pause