${_riverDreams_root_g} = Split-Path $(Split-Path $PSScriptRoot);
${_riverDreams_isWinNt_g} = [System.Environment]::OsVersion.Platform -match "Win32NT";

if (-not(Test-Path ("${_riverDreams_root_g}\build\bin\river-dreams" + $(if (${_riverDreams_isWinNt_g}) { ".exe" })))) {
  if (${_riverDreams_isWinNt_g}) {
    ${originalPath} = ${env:PATH};
    foreach (${year} in $(Get-ChildItem "C:\Program Files\Microsoft Visual Studio")) {
      foreach (${edition} in $(Get-ChildItem "C:\Program Files\Microsoft Visual Studio\${year}")) {
        [System.Environment]::SetEnvironmentVariable("PATH",
          ${env:PATH} +
          ";C:\Program Files\Microsoft Visual Studio\${year}\${edition}\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin");
      }
    }
  }
  if (-not(Get-Command git -ErrorAction SilentlyContinue) -or
      -not(Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host;
    Write-Host -ForegroundColor DarkRed -NoNewline "[ERROR] ";
    Write-Host -NoNewline "river-dreams ";
    Write-Host -ForegroundColor DarkGray -NoNewline "(code 1)"
    Write-Host ": some dependecies are missing. Install the ones defined in the README.md file.";
    if (${_riverDreams_isWinNt_g}) {
      [System.Environment]::SetEnvironmentVariable("PATH", ${originalPath});
      ${originalPath} = ${null};
    }
    return;
  }
  Write-Host;
  Write-Host -ForegroundColor DarkBlue -NoNewline "[INFO] ";
  Write-Host "river-dreams: wait while the theme is being prepared for the first time.";
  git -C ${_riverDreams_root_g} submodule init > ${null} 2>&1;
  git -C ${_riverDreams_root_g} submodule update > ${null} 2>&1;
  Remove-Item -Recurse -Force -ErrorAction SilentlyContinue `
              "${_riverDreams_root_g}\build";
  cmake -S ${_riverDreams_root_g} `
        -B "${_riverDreams_root_g}\build\cmake" > ${null};
  cmake --build "${_riverDreams_root_g}\build\cmake" --config release > ${null};
  cmake --install "${_riverDreams_root_g}\build\cmake" > ${null};
  cls;
  if (${_riverDreams_isWinNt_g}) {
    [System.Environment]::SetEnvironmentVariable("PATH", ${originalPath});
    ${originalPath} = ${null};
  }
}

[System.Environment]::SetEnvironmentVariable("PATH",
  [System.Environment]::GetEnvironmentVariable("PATH") +
  $(if (${_riverDreams_isWinNt_g}) { ";" } else { ":" }) +
  (Join-Path (Join-Path ${_riverDreams_root_g} "build") "bin"));

function Prompt {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8;
  ${exitCode} = $(if (${LASTEXITCODE} -eq ${null}) { 0 } else { ${LASTEXITCODE} });
  ${consoleWidth} = $([Console]::WindowWidth);
  ${leftPrompt} = & "river-dreams" "pwsh" "left" ${exitCode} ${consoleWidth};
  Write-Host -NoNewline ${leftPrompt};
  ${rightPromptParts} =
      & "river-dreams" "pwsh" "right" ${exitCode} ${consoleWidth} -Split "`n";
  if (${rightPromptParts}[1] -gt 0) {
    ${cursorLeft} = [Console]::CursorLeft;
    ${cursorTop} = [Console]::CursorTop;
    [Console]::SetCursorPosition([Console]::WindowWidth - ${rightPromptParts}[1], ${cursorTop});
    Write-Host -NoNewline ${rightPromptParts}[0];
    [Console]::SetCursorPosition(${cursorLeft}, ${cursorTop});
  }
  " ";
}
