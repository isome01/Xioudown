#
# Generalized test-program compiler script
#

$project_base = Convert-Path([string]$pwd + '\..')
$build_base = $project_base + '\build'
$bin_base = $project_base + '\bin'
$lib_base = $project_base + '\lib'
$test_base = $project_base + '\test'
$compPath = $test_base + '\comp'
$integPath = $test_base + '\integration'

#define methods
function copyLibsFromPath([string]$path = "") {
  Set-Location $project_base
  $success = 0
  if (!(Test-Path($path))) {
    Write-Output "Error: path does not exist (" $path ")."
  } else {
    if (!(Test-Path($compPath))) {
      New-Item -Force -ItemType "directory" -Path $compPath
    }

    #go through each item and copy appropriate files with .dll extension
    Get-ChildItem -Recurse -Path $path | Where-Object {
      [regex]::Matches($_.Name, ".dll$")
    } | ForEach-Object {
      [string]$resource = ($path + '\' + $_.Name)
      if (!(Test-Path($compPath + "\" + $_.Name))) {
        Copy-Item -Path $resource -Destination $compPath
      }
    }
    $success = 1
  }

   return [boolean]($success)
}

function createTestDir () {
  if (!(Test-Path($test_base))) {
    Set-Location $project_base
    # create a test path if one doesn't already exist
    New-Item -Force -ItemType "directory" -Path $test_base
    Set-Location $test_base
    New-Item -Force -ItemType "directory" -Path $compPath
    New-Item -Force -ItemType "directory" -Path $integPath
    return [boolean](1)
  }
  
  return [boolean](0)
}

function main () {
  Set-Location $project_base

  $testDirExists = createTestDir('')
  if ($testDirExists) {
    Write-Output "Created directory 'Test': As one did not already exist. Start implementing tests!"
    Set-Location $build_base

  } else {
    Write-Output "Getting libraries..."
    if (!(copyLibsFromPath(".\lib\sdl_dev_lib\bin"))) {
      Write-Error("Unidentified path.")
    } else {
      #gather files to compile from test dir
      Set-Location $test_base
      Write-Output 'Gathering files for testing...'
    }

    #Prompt user for directory to test
    [string]$testDir = ''
    [string]$testDirPath = ''
    while (1) {
      $testDir = Read-Host -Prompt "Enter a test directory"
      $testDirPath = $integPath + '\' + $testDir
      if (!(Test-Path($testDirPath))) {
        Write-Output "The directory you've entered does not exist in " + $test_base "."
      } else {
        break
      }
    }

    [string[]]$matches = ""

    #Start gathering files from the engine
    Write-Output "Gathering files from Xioudown engine..." 
    Get-ChildItem -Path $bin_base -Recurse -Depth 10 | Where-Object {
      [regex]::Matches( $_.Name, ".cpp$")
    } | %{
      $matches += ( $_.DirectoryName + '\' + $_.Name)
    }

    #Start gathering files from test directory
    Write-Output "Gathering files from " $testDir "directory."
    Get-ChildItem -Path $testDirPath -Recurse -Depth 10 | Where-Object {
      [regex]::Matches( $_.Name, ".cpp$")
    } |
    %{
      $matches += ( $_.DirectoryName + '\' + $_.Name)
    }

    #echo $matches to directory
    Set-Location $testDirPath
    $matches > source_files.txt

    #Start Compiling and include SDL
    #NOTE: YOU should also include other third-party libraries (e.g. QTCore)
    Set-Location $lib_base
    Write-Output "Compiling..."
    $compOutput = ($compPath + "\" + $testDir).Replace('\', '/')
    Write-Output $compOutput
    g++ $matches -Isdl_dev_lib\include\SDL2 -Lsdl_dev_lib\lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -o $compOutput

    if (!(Test-Path($compOutput + ".exe"))) {
      Write-Error "Unable to compile source files."
    } else {
      #after successful compilation
      Write-Output "Executable made; name is " $testDir ".exe"

      #Create shortcut to executable
      $integOutput = ($integPath + '\' + $testDir)
      $Target = $compOutput + '.exe'
      $Shortcut =  $integOutput + "\testing.lnk" #~ NOTE ~ Shortcuts are .lnk files

      $WScriptShell = New-Object -ComObject WScript.Shell
      $Shortcut = $WScriptShell.CreateShortcut($Shortcut)
      $Shortcut.TargetPath = $Target
      $Shortcut.Save()
      Write-Output "Shortcut for executable made at " $Target
    }
  }

  Set-Location $build_base
}

main('')