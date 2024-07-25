${_riverDreams_root_g} = $(Split-Path $(Split-Path $PSScriptRoot));

Set-Item -Path Env:VIRTUAL_ENV_DISABLE_PROMPT -Value 1;

function Prompt {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8;
  ${leftPrompt} =
      & "${_riverDreams_root_g}\build\bin\left-prompt.exe" `
      $([Console]::WindowWidth) `
      $(if (${LASTEXITCODE} -eq $null) { 0 } else { ${LASTEXITCODE} }) `
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
  " "
}
