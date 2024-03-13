
if (([Security.Principal.WindowsPrincipal] `
        [Security.Principal.WindowsIdentity]::GetCurrent() `
    ).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator) -eq $False)

function New-TemporaryDirectory {
    $parent = [System.IO.Path]::GetTempPath()
    [string] $name = [System.Guid]::NewGuid()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}

Function Get-Folder($initialDirectory="")
{
    [System.Reflection.Assembly]::LoadWithPartialName("System.windows.forms")|Out-Null

    $foldername = New-Object System.Windows.Forms.FolderBrowserDialog
    $foldername.Description = "Select a folder for the git repositories"
    $foldername.rootfolder = "MyComputer"
    $foldername.SelectedPath = $initialDirectory

    if($foldername.ShowDialog() -eq "OK")
    {
        $folder += $foldername.SelectedPath
    }
    return $folder
}

$dir = New-TemporaryDirectory

Write-Host "Working in temporary directory $dir"

Set-Location $dir

# Check for gh command line

if ($null -eq (Get-Command "gh.exe" -ErrorAction SilentlyContinue)) 
{ 
   Write-Host "gh.exe not found"
   Write-Host 'Please install the GitHub CLI from https://cli.github.com/ and run "gh auth login" in a terminal to configure it'
   Read-Host 'Press Enter to exit the installer'
   Exit 1
}

Write-Host "Downloading jq..."
Invoke-WebRequest -Uri https://github.com/jqlang/jq/releases/download/jq-1.7.1/jq-windows-amd64.exe -OutFile jq.exe
Write-Host "Downloaded jq."

Write-Host 'Checking latest release...'

$release = gh api -H "Accept: application/vnd.github+json" /repos/arhacc/sw/releases/latest
$release_tag = ($release | jq -r '.tag_name')
$release_number = $release_tag.Substring(1)

Write-Host "Latest release is $release_tag"

$filename_qt = "XpuSdk-$release_number.msi"
$msi_url = ($release | jq -r ".assets[] | select(.name == \""$filename_qt\"")| .browser_download_url")

Write-Host "Downloading .msi from $msi_url"

Invoke-WebRequest -Uri $msi_url -OutFile $filename_qt

Write-Host "Launching .msi installer"

$params = @{
    "FilePath" = "$Env:SystemRoot\system32\msiexec.exe"
    "ArgumentList" = @(
       "/i"
        "$filename_qt"
    )
    "Verb" = "runas"
    "PassThru" = $true
}
$msi_installer = Start-Process @params
$msi_installer.WaitForExit()

Write-Host "Installer finished"

# Set up ARHACC_PATH

Write-Host "Launching folder prompt for ARHACC_PATH"

$arhacc_path = Get-Folder

Write-Host "Selected ARHACC_PATH = $arhacc_path"

Write-Host "Setting ARHACC_PATH = $arhacc_path"
[System.Environment]::SetEnvironmentVariable('ARHACC_PATH', $arhacc_path, 'User')   
Write-Host "Setting XPU_LIBRARIES_PATH = $arhacc_path\libraries"
[System.Environment]::SetEnvironmentVariable('XPU_LIBRARIES_PATH', "$arhacc_path\libraries", 'User')
Write-Host "Setting XPU_HW_PATH = $arhacc_path\hw"
[System.Environment]::SetEnvironmentVariable('XPU_HW_PATH', "$arhacc_path\hw", 'User')

Write-Host "Pulling repos in ARHACC_PATH"

Set-Location $arhacc_path
git clone https://github.com/arhacc/libraries
git clone https://github.com/arhacc/hw

Write-Host "Finished pulling repos"

Write-Host "Setting HOME = $Env:USERPROFILE"
[System.Environment]::SetEnvironmentVariable('HOME', $Env:USERPROFILE, 'User')   

$xpu_home = "$Env:USERPROFILE\.xpu"

Write-Host "Setting XPU_HOME = $xpu_home"
[System.Environment]::SetEnvironmentVariable('XPU_HOME', $xpu_home, 'User')

Write-Host "Creating directory $xpu_home"
New-Item -ItemType Directory -Force -Path $xpu_home

Wirte-Host "Creating directory $xpu_home\bin"
New-Item -ItemType Directory -Force -Path $xpu_home\bin

Wirte-Host "Creating directory $xpu_home\etc"
New-Item -ItemType Directory -Force -Path $xpu_home\etc

Write-Host "Creating symlink $xpu_home\etc\architecture_implementations -> $arhacc_path\hw\architecture_implementations\"
New-Item -ItemType SymbolicLink -Force -Path $xpu_home\etc\architecture_implementations -Target "$arhacc_path\hw\architecture_implementations\"

Write-Host "Writing default config in $xpu_home\etc\sdk.conf"

'last.project.directory = $ARHACC_PATH/libraries/app_level/prj5/prj5.xpuprj
preferences.target=0,connecting,simulation2,remote,athena.arh.pub.ro,49000,xpu3200016,selected
preferences.target=1,connecting,pinq1,remote,zeus.arh.pub.ro,49000,xpu3200016
preferences.target=2,connecting,simulation1,remote,localhost,49000,xpu3200016
preferences.target=3,connecting,pynq2,remote,zeus.arh.pub.ro,49001,xpu3200016
gui.size.x = 1280
gui.size.y = 799
gui.splitPane1 = 0.9305354558610709
gui.splitPane2 = 0.17688679245283018
gui.splitPane3 = 0.3763779527559055
gui.splitPane5 = 0.547550432276657
lf = Nimbus
editor_theme = Dark
debug = 1
debug.magnifier.startIndex = 0
debug.magnifier.stopIndex = 6
debug.magnifier.jSplitPane1 = 0.1581858407079646
debug.window.state = false
debug.window.expanded.location.x = 0
debug.window.expanded.location.y = 33
debug.window.expanded.dimension.x = 493.0
debug.window.expanded.dimension.y = 771.0
gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled = true
gui.menu.file.preferences.general.automaticallyCheckForUpdates.interval = Every day
gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled = true
git.local.repo = $ARHACC_PATH
profile = AppLevel
librariesPath=$ARHACC_PATH/libraries/
last_directory = $ARHACC_PATH/libraries/app_level/Example6/Example6.asm
last.project.location = $ARHACC_PATH/libraries/app_level/Example6
selectedProfileLevel = AppLevel
pathToActiveProject=$ARHACC_PATH/libraries/app_level/Example0/Example0.xpuprj
github_token=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
remember_target_connection_for_each_project = false' | Out-File -Encoding utf8NoBOM $xpu_home\etc\sdk.conf


Write-Host "Writing $xpu_home\bin\gui.bat"

"cd $Env:USERPROFILE\AppData\Local\xpu-sdk\app
java -cp ""xpu-sdk-0.1.0.jar;xpu-sdk-libs-0.14.78.jar"" xpu.sw.tools.sdk.Sdk -cmd gui" | Out-File -Encoding utf8NoBOM $xpu_home\bin\gui.bat

Write-Host "Creating Desktop shortcut for gui.bat"

$DesktopPath = [Environment]::GetFolderPath("Desktop")
New-Item -ItemType SymbolicLink -Path $xpu_home\bin\gui.bat -Target "$DesktopPath\XPU SDK.lnk"

Read-Host 'Press Enter to exit the installer'
