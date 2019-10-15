#Start transcript log
Start-Transcript


[string[]]$matches = ""
#[string]$compiler = "-I.\lib\sdl_dev_lib_mingw\include\SDL2"
#[string]$lib = "-L.\lib\sdl_dev_lib_mingw\lib"
#[string]$linker = "-lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2"


#handle \bin directory exists\nonexistent


if(!(Test-Path '.\bin')){
    New-Item -Item-Type Directory -Force -Path '.\bin'
}


#Start gathering files from source


Set-Location .\src
Write-Output Gathering files...

#a descending recursive depth of 10 is a bit much, but we will use this for laziness 
Get-ChildItem -Recurse -Depth 10 | Where-Object {
	[regex]::Matches( $_.Name, ".cpp$")
} |
%{
	$matches += ( $_.DirectoryName + '\' + $_.Name)
}
$matches > source_files.txt
#echo $matches
Set-Location ..

#Start Compiling

Write-Output Compiling... 
g++ $matches -Ilib\sdl_dev_lib\include\SDL2 -Llib\sdl_dev_lib\lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -o bin/testing

#Create shortcut to executable


$Target = [string]$pwd + '.\bin\testing.exe'
$Shortcut = ".\testing.lnk" #~ NOTE ~ Shortcuts are .lnk files

$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut($Shortcut)
$Shortcut.TargetPath = $Target
$Shortcut.Save()