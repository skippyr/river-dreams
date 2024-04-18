<p align="center">
	<img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;riverdreams&ensp;≥v≥v</h1>
<p align="center">
	<img alt="" src="https://img.shields.io/github/license/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
	&nbsp;
	<img alt="" src="https://img.shields.io/github/v/tag/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
	&nbsp;
	<img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
	&nbsp;
	<img alt="" src="https://img.shields.io/github/stars/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## ❡ About

A tribal looking theme for the `zsh` shell for Linux.

<p align="center">
	<img alt="" src="assets/preview.webp" />
</p>
<p align="center"><sup><strong>Caption:</strong> a preview of riverdreams applied on <code>zsh</code>. The terminal theme used is <a href="https://github.com/skippyr/flamerial">Flamerial</a> and font is <a href="https://github.com/be5invis/Iosevka">Iosevka SS08</a>.</sup></p>

### Features

Its left prompt displays:

- Your local IPV4 address.
- Your disk usage.
- Your battery charge, if any.
- A calendar.
- A clock.
- A decorator when you are the root user.
- The exit code of the last command you typed in.
- The active Python virtual environment, if you have sourced one.
- The active directory path, shorting it when inside of a `git` repository.
- The active branch, when inside of a `git` repository.
- A decorator when you do not have access to the active directory.

Its right prompt displays:

- The total of each entry type and some statuses in the active directory: regulars, directories, blocks, characters, sockets, fifos, symlinks, hidden and temporary entries.
- The total of background jobs.

## ❡ Install

### Dependencies

The following dependencies must be installed before installing it:

- **git**: it will be used to clone this repository.
- **gcc**, **make**: they will be used to compile this theme.
- **Noto Sans**, [**Nerd Font Symbols**](https://www.nerdfonts.com/font-downloads): these fonts provide the pretty symbols used in the theme.

### Procedures

#### Manual Procedures

If you want to install this theme without a plugin manager or for a specific framework, using a terminal, follow these steps:

- Clone this repository using `git`:

```sh
git clone --depth 1 "~/.config/zsh/themes/riverdreams";
```

- Source the file `riverdreams.zsh-theme` that is inside of the repository you just cloned in your `~/.zshrc` configuration file:

```zsh
source "~/.config/zsh/themes/riverdreams/riverdreams.zsh-theme";
```

- Reopen `zsh`.
- Wait for the theme to compile.

#### Procedures For OhMyZSH

If you want to install and use this theme with the [`ohmyzsh`](https://github.com/ohmyzsh/ohmyzsh) framework, using a terminal, follow these steps:

- Clone this repository using `git`:

```sh
git clone --depth 1 \
    "https://github.com/skippyr/riverdreams" \
    "${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/riverdreams";
```

- Apply the theme in your `~/.zshrc` configuration file by setting the `ZSH_THEME` variable:

```zsh
ZSH_THEME="riverdreams/riverdreams";
```

- Reopen `zsh`.
- Wait for the theme to compile.

#### Procedures For Antidote

If you want to install this theme with the [`antidote`](https://github.com/mattmc3/antidote) plugin manager, using a terminal, follow these steps:

- Add this repository to your plugins file, usually `~/.zsh_plugins.txt`.

```
skippyr/riverdreams
```

- Use `antidote` to load your plugins.

```zsh
antidote load "~/.zsh_plugins.txt";
```

- Wait for the theme to compile.

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/riverdreams/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/riverdreams/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you agree to license your work under the same license that the project uses.

## ❡ License

This is free software licensed under the MIT License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
