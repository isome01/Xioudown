#Start transcript log

#Start-Transcript


[string[]]$matches = ""
#[string]$compiler = "-I.\lib\sdl_dev_lib_mingw\include\SDL2"
#[string]$lib = "-L.\lib\sdl_dev_lib_mingw\lib"
#[string]$linker = "-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2"


#handle \bin directory exists\nonexistent


#if(!(Test-Path '.\bin')){
#    New-Item -Item-Type Directory -Force -Path '.\bin'
#}


#Start gathering files from the engine
Set-Location ..\bin
Write-Output Gathering files...

#a descending recursive depth of 10 is a bit much, but we will use this for laziness 
Get-ChildItem -Recurse -Depth 10 | Where-Object {
	[regex]::Matches( $_.Name, ".cpp$")
} |
%{
	$matches += ( $_.DirectoryName + '\' + $_.Name)
}

#gather files to compile from src/client
Set-Location ..\src
Write-Output Gathering files...

Get-ChildItem -Recurse -Depth 10 | Where-Object {
	[regex]::Matches( $_.Name, ".cpp$")
} |
%{
	$matches += ( $_.DirectoryName + '\' + $_.Name)
}

#echo $matches
Set-Location ..\build
$matches > source_files.txt

Set-Location ..\lib

#Start Compiling and include SDL

Write-Output Compiling... 
g++ $matches -Isdl_dev_lib\include\SDL2 -Lsdl_dev_lib\lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -o ../bin/testing

#also include other third-party dependencies


#Create shortcut to executable
Set-Location ..\bin
$Target = [string]$pwd + '\testing.exe'
$Shortcut = ".\testing.lnk" #~ NOTE ~ Shortcuts are .lnk files

Write-Output "\n\n\n\nShortcut for executable made at: " + $Target
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut($Shortcut)
$Shortcut.TargetPath = $Target
$Shortcut.Save()

Set-Location ..\build