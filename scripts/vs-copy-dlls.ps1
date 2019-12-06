Write-Output 'Copying required DLLs...'

try {
    xcopy /y /q ext\glew-2.1.0\bin\Release\x64\glew32.dll bin
    xcopy /y /q ext\glfw-3.3\lib-vc2019\glfw3.dll bin
    xcopy /y /q ext\assimp-5.0.0\build-msvc\code\MinSizeRel\assimp-vc142-mt.dll bin
} catch {
	exit 1
}