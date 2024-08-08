${_riverDreams_root_g} = $(Split-Path $(Split-Path $PSScriptRoot));

${originalPath} = ${env:PATH};
# Visual Studio 2022 Development Profile does not adds CMake to the ${PATH}
# environment variable at the time this script runs. Thus, that need to be
# manually set temporarily.
foreach (${edition} in $(Get-ChildItem "C:\Program Files\Microsoft Visual Studio\2022")) {
  # Add each installed edition of Visual Studio 2022 to ${PATH}.
  [System.Environment]::SetEnvironmentVariable(
    "PATH",
    ${env:PATH} +
    ";C:\Program Files\Microsoft Visual Studio\2022\${edition}\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
  );
}

if (-not(Get-Command git -ErrorAction SilentlyContinue) -or
    -not(Get-Command cmake -ErrorAction SilentlyContinue)) {
  Write-Host;
  Write-Host "river-dreams";
  Write-Host;
  Write-Host "[ERROR] Some dependencies are missing.";
  Write-Host "        Please install the ones defined in the README.md file.";
  [System.Environment]::SetEnvironmentVariable("PATH", ${originalPath});
  ${originalPath} = ${null};
  return;
}

if (-not (Test-Path "${_riverDreams_root_g}\build\bin\left-prompt.exe") -or
    -not (Test-Path "${_riverDreams_root_g}\build\bin\right-prompt.exe")) {
  Write-Host;
  Write-Host "river-dreams";
  Write-Host;
  Write-Host "Preparing the theme for the first time.";
  Write-Host "Please, wait just a moment.";
  git -C ${_riverDreams_root_g} submodule init > ${null} 2>&1;
  git -C ${_riverDreams_root_g} submodule update > ${null} 2>&1;
  Remove-Item -Recurse -Force -ErrorAction SilentlyContinue `
              "${_riverDreams_root_g}\build";
  cmake -S ${_riverDreams_root_g} `
        -B "${_riverDreams_root_g}\build\cmake" > ${null};
  cmake --build "${_riverDreams_root_g}\build\cmake" --config release > ${null};
  cmake --install "${_riverDreams_root_g}\build\cmake" > ${null};
  cls;
}

[System.Environment]::SetEnvironmentVariable("PATH", ${originalPath});
${originalPath} = ${null};
Set-Item -Path env:VIRTUAL_ENV_DISABLE_PROMPT -Value 1;

function Prompt {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8;
  ${leftPrompt} =
      & "${_riverDreams_root_g}\build\bin\left-prompt.exe" `
      $([Console]::WindowWidth) `
      $(if (${LASTEXITCODE} -eq ${null}) { 0 } else { ${LASTEXITCODE} }) `
      $(if (([Security.Principal.WindowsPrincipal] `
             [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) `
             { 1 } else { 0 });
  ${rightPromptParts} =
      & "${_riverDreams_root_g}\build\bin\right-prompt.exe" -Split "`n";
  Write-Host -NoNewline ${leftPrompt};
  if (${rightPromptParts}[1] -gt 0) {
    ${cursorLeft} = [Console]::CursorLeft;
    ${cursorTop} = [Console]::CursorTop;
    [Console]::SetCursorPosition([Console]::WindowWidth - ${rightPromptParts}[1], ${cursorTop});
    Write-Host -NoNewline ${rightPromptParts}[0];
    [Console]::SetCursorPosition(${cursorLeft}, ${cursorTop});
  }
  " ";
}
