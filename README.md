<p align="center">
  <img alt="" src="assets/ornament.png" width=1020 />
</p>
<h1 align="center">≥v≥v&ensp;River Dreams&ensp;≥v≥v</h1>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/river-dreams?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/river-dreams?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/river-dreams?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/river-dreams?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## ❡ About

A fast and full-featured tribal looking shell theme available for Windows (PowerShell), Linux (ZSH) and MacOS (ZSH). It is designed to help you develop even your most ambitious software projects.

<p align="center">
  <img alt="" src="assets/preview.png" width=1020 />
</p>
<p align="center"><strong>Caption:</strong> River Dreams running on MacOS (ZSH). Terminal theme is <a href="https://github.com/skippyr/flamerial">Flamerial</a>. Font is Fira Code (patched by the <a href="https://www.nerdfonts.com/font-downloads">Nerd Fonts project</a>).</p>

> [!NOTE]
> The font and colors used in the software may be affected by your terminal settings. Images for illustration purpose only.

### Features

These are the descriptions of each prompt feature from left to right, top to bottom, respectively:

#### Left Prompt Features

- Your local IPV4 address or "No Address" if no address can be found.
- If you are using a laptop, its battery charge and if it is being currently charged.
- Your disk usage.
- A calendar showing the week day, month and day.
- A 24-hours clock with a symbol that changes depending on the day time: dawn (between 00h00m to 06h00m), morning (between 06h00m and 12h00m), afternoon (between 12h00m and 18h00m) and night (between 18h00m and 00h00m).
- A hash symbol when you have administrator privileges, on Windows, or is the root user on MacOS and Linux.
- The exit code of the last command ran.
- The active Python virtual environment, if you have sourced one.
- The current active directory path, shorting it when inside of a Git repository.
- The active branch, when inside of a Git repository.

#### Right Prompt Features

- The total of each entry type in the current active directory: block devices, character devices, directories, fifos, files, hidden entries, sockets, symlinks and temporary entries. Symlinks are followed. Types availability and interpretation are subject to the operating system you are using.

## ❡ Install

### Dependencies

The following dependencies must be installed before it:

#### Dependencies For Windows

- **[Visual Studio 2022](https://visualstudio.microsoft.com) with the "Desktop development with C++" workload:** it provides all the tools required to build this software.
- **[Git](https://git-scm.com):** it will be used to clone this repository.
- **A font patched by the [Nerd Fonts project](https://www.nerdfonts.com/font-downloads):** it provides the pretty symbols used by the software.

#### Dependencies For Linux

- **GCC, CMake**: they will be used to build this software.
- **Git**: it will be used to clone this repository.
- **A font patched by the [Nerd Fonts project](https://www.nerdfonts.com/font-downloads):** it provides the pretty symbols used by the software.

> [!TIP]
> Use your distro package manager to install these packages.

#### Dependencies For MacOS

- **Apple Command Line Tools, cmake**: they will be used to build this software.
- **Git**: it will be used to clone this repository.
- **A font patched by the [Nerd Fonts project](https://www.nerdfonts.com/font-downloads):** it provides the pretty symbols used by the software.

> [!TIP]
> Use `xcode-select --install` to install the Apple command line tools. For the rest, use [HomeBrew](https://brew.sh/).

### Procedures

On Windows, using the `Developer PowerShell for VS 2022` terminal profile or, on any other operating systems, using any terminal, follow these instructions:

- Clone this repository using `git`:

```powershell
# For Windows (PowerShell).
git clone --depth 1 https://github.com/skippyr/river-dreams `
          "${env:USERPROFILE}\powershell\themes\river-dreams";
```

```zsh
# For Linux (ZSH) and MacOS (ZSH).
git clone --depth 1 https://github.com/skippyr/river-dreams \
          ~/.local/share/zsh/themes/river-dreams;
```

- Source the specific connector for your shell:

```powershell
# For Windows (PowerShell).
Write-Output `n'. "${env:USERPROFILE}\powershell\themes\river-dreams\src\connectors\river-dreams.ps1";' | `
Out-File -Append -Encoding UTF8 ${PROFILE};
```

```zsh
# For Linux (ZSH) and MacOS (ZSH).
echo "source ~/.local/share/zsh/themes/river-dreams/src/connectors/river-dreams.zsh;" >> ~/.zshrc;
```

- Reopen your shell.
- During its first execution, River Dreams will automatically check its dependencies and build its source code, a process that can take a moment. Once that is done, the theme will be loaded and will be ready for you to use.

## ❡ Uninstall

- In your terminal profile file, remove the line that sources the connector added during the installation. If you are on Windows, use `echo ${PROFILE}` to locate this file.
- Remove the repository you have cloned during the installation. It contains all the file automatically generated and used by the software.

```zsh
# For Windows (PowerShell).
Remove-Item -Recurse -Force "${env:USERPROFILE}\powershell\themes\river-dreams";
```

```zsh
# For Linux (ZSH) and MacOS (ZSH).
rm -rf ~/.local/share/zsh/themes/river-dreams;
```

- Reopen your shell.
- River Dreams should now be uninstalled.

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/river-dreams/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## ❡ Privacy

This software does not collect, store or share any user data. It only gathers the minimal information needed for its functionality, and this data is discarded once its execution is over. Its source code is available for reviews. If you have any questions about it, open a new issue in its [issues pages](https://github.com/skippyr/river-dreams/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/river-dreams/pulls) or to my [e-mail](mailto:skippyr.developer@icloud.com).

By contributing to this project, you agree to license your work under the same license that the project uses.

## ❡ License

This is free software licensed under the MIT License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>Birth:</strong> February 20th 2023.</sup></p>
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
