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

A tribal looking shell theme for ZSH, made to help you develop your ambitious software projects.

<p align="center">
  <img alt="" src="assets/preview.png" width=1020 />
</p>
<p align="center"><strong>Caption:</strong> a preview of the River Dreams theme.</p>

### Features

These are the descriptions of each prompt feature from left to right, top to bottom, respectively:

#### Left Prompt Features

The left prompt shows:

- Your local IPV4 address.
- If you are using a laptop, its battery charge and if it is being currently charged.
- Your disk usage.
- A calendar showing the week day, month and day.
- A 24-hours clock with a symbol that changes depending on the day time.
- A hash symbol when you are the root user.
- The exit code of the last command ran.
- The active Python venv, if you have sourced one.
- The current active directory path, shorting it when inside of a Git repo.
- When inside of a Git repo, the active branch name or the reference hash if its under rebase, and a X symbol if there are changes to be commited.
- A lock symbol when you can not modify the current active directory.

#### Right Prompt Features

The right prompt shows:

- The total of each entry type in the current active directory: block devices, character devices, directories, fifos, files, hidden entries, sockets, symlinks and temporary entries.
- The total jobs running in the background.

## ❡ Install

### Dependencies

The following dependencies must be installed before it:

- [**Rust Toolchain**](https://www.rust-lang.org): it will be used to build the project.
- **Git**: it will be used to clone this repository.
- **A font patched by the [Nerd Fonts project](https://www.nerdfonts.com/font-downloads):** it provides the pretty symbols used by the software.

> [!TIP]
> Use your OS package manager or [HomeBrew](https://brew.sh) to install these packages.

> [!WARNING]
> It is highly recommended that you use a terminal with good unicode support and avoid installing multiple Nerd Fonts for the symbols used in the theme to appear correctly. The terminal recommendation for Linux is **Kitty**. You can download it by following the previous tip.

### Procedures

- Clone this repository using `git`:

```zsh
git clone --depth 1 https://github.com/skippyr/river-dreams \
                    ~/.config/zsh/themes/river-dreams;
```

- Add its theme script to your zsh init file.

```zsh
echo "source ~/.config/zsh/themes/river-dreams/river-dreams.zsh-theme;" \
     >> ~/.zshrc;
```

- Reopen your shell and await for cargo to automatically compile the theme.

> [!NOTE]
> In case of an error, check cargo's output for troubleshooting. Reopen your shell to retry compilation.

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/river-dreams/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions in the form of bug reports and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/river-dreams/pulls) or to my [e-mail](mailto:skippyr.developer@icloud.com).

## ❡ License

This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the LICENSE file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
