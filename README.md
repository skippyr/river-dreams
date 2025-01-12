<p align="center">
  <img alt="" src="https://raw.githubusercontent.com/skippyr/river_dreams/refs/heads/master/assets/ornament.png" width=1020 />
</p>
<h1 align="center">≥v≥v&ensp;River Dreams&ensp;≥v≥v</h1>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/river_dreams?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/river_dreams?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/river_dreams?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/river_dreams?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## ❡ About
River Dreams is a tribal looking theme for the ZSH shell made to help you craft your most ambitious software projects. It is available for macOS and Linux.

<p align="center">
  <img alt="" src="https://raw.githubusercontent.com/skippyr/river_dreams/refs/heads/master/assets/preview.png" width=1020 />
</p>
<p align="center"><strong>Caption:</strong> a preview of the River Dreams theme.</p>

### Prompts & Modules
The theme is composed of two prompts, one in the left and other in the right. Each prompt has modules that provides its features. From left to right, top to bottom, those are:

#### Left Prompt Modules
The left prompt prompt modules are:

- **IP Address Module:** displays the IP address of the computer.
- **Battery Module:** which, in case of a laptop, displays the information about the charge of the battery of the computer, including its percentage and status.
- **Disk Module:** which displays the information about the usage of the main disk of the computer, including its percentage and status.
- **Calendar Module:** which displays a calendar showing the week day, month, and day of month.
- **Clock Module:** which display a 24-hours clock with a symbol that changes accordingly to the day fraction.
- **Root Module:** which displays a hash symbol when the user is `root`.
- **Exit Code Module:** which displays the exit code of the last command inserted in the shell.
- **Virtual Env Module:** which displays the active Python virtual environment sourced.
- **Path Module:** which displays the current directory path, abbreviating it when inside of Git repositories.
- **Git Module:** which displays information about an active repository, including the active reference branch name or hash and a ballot X symbol when there are changes to be commited.
- **Directory Access Module:** which displays a lock symbol whenever the user can not write to the current directory.

#### Right Prompt Modules
The right prompt prompt modules are:

- **Directory Summary Module:** which displays the total number of each entry type in the current directory.
- **Jobs Module:** which displays the total number of background jobs running in the shell.

## ❡ Install
### Dependencies

The following dependencies must be installed before it:
- [**Rust Toolchain**](https://www.rust-lang.org): it will be used to build the project.
- **A font patched by the [Nerd Fonts project](https://www.nerdfonts.com/font-downloads):** it provides the pretty symbols used by the software.

Use your OS package manager or [HomeBrew](https://brew.sh) to install these packages.

It is also recommended to use a terminal with great Unicode support avoid installing multiple Nerd Fonts for the symbols used in the theme to appear correctly. The terminal recommended is [**Kitty**](https://github.com/kovidgoyal/kitty). You can download it by following the previous tip.


### Procedures
- Apply the Nerd Font installed in your terminal and reopen it.
- Install the theme using `cargo`. In case of an error, check `cargo`'s output for troubleshooting. Reopen your shell to retry compilation:

```zsh
cargo install river_dreams;
```

- Add its connector to your zsh configuration file.

```zsh
river_dreams --connector >> ~/.zshrc;
```

- Reopen your shell.

## ❡ Help
For quick instructions about the usage of the software, refer to its help page:

```zsh
river_dreams --help;
```

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/river_dreams/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## ❡ Contributing
This project is open to review and possibly accept contributions in the form of bug reports and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/river_dreams/pulls) or to my [e-mail](mailto:skippyr.developer@icloud.com).

## ❡ License
This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
