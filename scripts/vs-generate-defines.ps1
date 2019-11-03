Write-Output 'Generating defines...'

try {
    # Load template
    $templatePath = Resolve-Path -Path ".\scripts\vs-generated-defines.template.props"
    $propsTemplate = [IO.File]::ReadAllText($templatePath)

    # Get values
    $projectVersion= (git describe --match "v[0-9]*" --abbrev=0 HEAD) | Out-String
    $projectVersion = $projectVersion -replace "`n|`r"
    $projectLastCommit= (git rev-parse --short HEAD) | Out-String
    $projectLastCommit = $projectLastCommit -replace "`n|`r"
    $buildDesc = "$projectVersion-$projectLastCommit" + ' $(Configuration)'

    # Create new .props file
    $props = $propsTemplate
    $props = $props -replace "{{PROJECT_VERSION}}", "$projectVersion"
    $props = $props -replace "{{BUILD_DESC}}", "$buildDesc"

    # Save file
    $outputPath = Join-Path (Resolve-Path .) "vs-generated-defines.props"
    Set-Content -Path $outputPath -Value $props
} catch {
	exit 1
}