# This script sets up Windows Dependencies
$ErrorActionPreference = "Stop"

$currentDir = Resolve-Path .
$extDir = Join-Path (Resolve-Path .) "ext\"
$webClient = New-Object System.Net.WebClient

$glfwVersion = "3.3"
$glmVersion = "0.9.9.6"
$cerealVersion = "1.3.0"
$premakeVersion = "5.0.0-alpha14"

Write-Output "Setting up Konstruisto dependencies for MSVC..."
Write-Output "Dependencies directory: $extDir"

# Create directory
New-Item -ErrorAction Ignore -ItemType directory -Path $extDir | Out-Null

# Step counter
$step = 0
$totalSteps = 8

$step++
# Set up GLFW
$glfwTargetPath = Join-Path $extDir "glfw-$glfwVersion.zip"
if (!(Test-Path $glfwTargetPath)) {
    Write-Output "[$step/$totalSteps] Downloading GLFW..."
    $glfwUrl = "https://github.com/glfw/glfw/releases/download/" + $glfwVersion + "/glfw-" + $glfwVersion + ".bin.WIN64.zip"
    $webClient.DownloadFile($glfwUrl, $glfwTargetPath)
}

$glfwIntermediateDir = Join-Path $extDir "glfw-$glfwVersion.bin.WIN64"
$glfwFinalDir = Join-Path $extDir "glfw-$glfwVersion"
if (!(Test-Path $glfwFinalDir) -And !(Test-Path $glfwIntermediateDir)) {
    Write-Output "[$step/$totalSteps] Unzipping GLFW..."
    Expand-Archive $glfwTargetPath -DestinationPath $extDir
}
if (Test-Path $glfwIntermediateDir) {
    Rename-Item -path $glfwIntermediateDir -newName $glfwFinalDir
}

$step++
# Set up GLM
$glmTargetPath = Join-Path $extDir "glm-$glmVersion.zip"
if (!(Test-Path $glmTargetPath)) {
    Write-Output "[$step/$totalSteps] Downloading GLM..."
    $glmUrl = "https://github.com/g-truc/glm/releases/download/$glmVersion/glm-$glmVersion.zip"
    $webClient.DownloadFile($glmUrl, $glmTargetPath)
}

$glmIntermediateDir = Join-Path $extDir "glm"
$glmFinalDir = Join-Path $extDir "glm-$glmVersion"
if (!(Test-Path $glmFinalDir) -And !(Test-Path $glmIntermediateDir)) {
    Write-Output "[$step/$totalSteps] Unzipping GLM..."
    Expand-Archive $glmTargetPath -DestinationPath $extDir
}
if (Test-Path $glmIntermediateDir) {
    Rename-Item -path $glmIntermediateDir -newName $glmFinalDir
}

$step++
# Set up Cereal
$cerealTargetPath = Join-Path $extDir "cereal-$cerealVersion.zip"
if (!(Test-Path $cerealTargetPath)) {
    Write-Output "[$step/$totalSteps] Downloading Cereal..."
    $cerealUrl = "https://github.com/USCiLab/cereal/archive/v" + $cerealVersion + ".zip"
    $webClient.DownloadFile($cerealUrl, $cerealTargetPath)
}

$cerealFinalDir = Join-Path $extDir "cereal-$cerealVersion"
if (!(Test-Path $cerealFinalDir)) {
    Write-Output "[$step/$totalSteps] Unzipping Cereal..."
    Expand-Archive $cerealTargetPath -DestinationPath $extDir
}

$step++
# Set up stb_image
$stbImageTargetPath = Join-Path $extDir "stb\stb\stb_image.h"
if (!(Test-Path $stbImageTargetPath)) {
    $stbImageTargetDir = Join-Path $extDir "stb\stb"
    New-Item -ErrorAction Ignore -ItemType directory -Path $stbImageTargetDir | Out-Null

    Write-Output "[$step/$totalSteps] Downloading stb_image..."
    $stbImageUrl = "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
    $webClient.DownloadFile($stbImageUrl, $stbImageTargetPath)
}

$step++
# Get NanoVG
$nanoVgDir = Join-Path $extDir "nanovg"
if (!(Test-Path $nanoVgDir)) {
    Write-Output "[$step/$totalSteps] Cloning memononen/nanovg for build..."
    git clone git@github.com:memononen/nanovg.git $nanoVgDir
}

$step++
# Get premake5
$premakeTargetPath = Join-Path $extDir "premake-$premakeVersion.zip"
if (!(Test-Path $premakeTargetPath)) {
    Write-Output "[$step/$totalSteps] Downloading Premake..."
    $premakeUrl = "https://github.com/premake/premake-core/releases/download/v" + $premakeVersion + "/premake-" + $premakeVersion + "-windows.zip"
    $webClient.DownloadFile($premakeUrl, $premakeTargetPath)
}

$premakeExePath = Join-Path $extDir "premake5.exe"
if (!(Test-Path $premakeExePath)) {
    Write-Output "[$step/$totalSteps] Unzipping premake..."
    Expand-Archive $premakeTargetPath -DestinationPath $extDir
}

$step++
# Build NanoVG
$nanoVgBuildDir = Join-Path $nanoVgDir "build"
$nanoVgFinalBuildDir = Join-Path $nanoVgDir "build-msvc"
if (!(Test-Path $nanoVgBuildDir) -And !(Test-Path $nanoVgFinalBuildDir)) {
    Write-Output "[$step/$totalSteps] Building NanoVG..."
    try {
        Write-Output "[$step/$totalSteps] Moving to $nanoVgDir."
        Set-Location $nanoVgDir
        
        Write-Output "[$step/$totalSteps] Generating VS2019 solution..."
        & "$premakeExePath" "vs2019"

        if (Test-Path $nanoVgBuildDir) {
            Rename-Item -path $nanoVgBuildDir -newName $nanoVgFinalBuildDir
        }
    } finally {
        Set-Location $currentDir
    }
}

$nanoVgSolution = Join-Path $nanoVgFinalBuildDir "nanovg.sln"
$todoMessage = "TODO: Open solution " + $nanoVgSolution + " in Visual Studio and build project nanovg for Release/x64."

$step++
Write-Host "[$step/$totalSteps] Success."
Write-Host $todoMessage