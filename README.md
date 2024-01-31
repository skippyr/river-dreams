<p align="center">
	<img src="imgs/ornament.webp" alt="" />
</p>
<h1 align="center">≥v≥v&ensp;riverdreams&ensp;≥v≥v</h1>
<p align="center">
	<img src="https://img.shields.io/github/license/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/v/tag/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/commit-activity/t/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/stars/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" alt="" />
</p>

## ❡ About

A tribal inspired ZSH theme for Linux designed to help you develop your most ambitious projects.

<p align="center"><img src="imgs/preview.webp" alt="" /></p>
<p align="center"><sup><strong>Caption:</strong> a preview of the riverdreams theme applied on zsh. The terminal theme used is <a href="https://github.com/skippyr/flamerial">flamerial</a> and font is <a href="https://github.com/be5invis/Iosevka">iosevka</a>.</sup></p>

### Features

It is designed to be used by experienced shell users that are looking for a simple and fast solution for their daily usage of the terminal.

Its left prompt displays:

- Your local IPV4 address.
- Your disk use percentage.
- Your battery status and percentage, if you have one.
- A calendar.
- A clock.
- A decorator when you are the root user.
- A decorator for exit codes.
- The active virtual environment, if one has been sourced.
- The active directory path, shorting it when inside of a git repo.
- The active branch, when inside of a git repo.
- A decorator when you can not modify the active directory.

Its right prompt displays:

- An overview of all the entry types in the active directory:
    - The total of hidden entries.
    - The total of backup/temporary entries.
    - The total of regular entries that are executable.
    - The total of symlinks (they are also followed).
    - The total of regular entries.
    - The total of directories.
    - The total of block devices.
    - The total of character devices.
    - The total of fifos;
    - The total of sockets;
- The total of jobs running in the background.

## ❡ Install

### Dependencies

The following dependencies are required to install it:

- **git**: it will be used to clone this repository.
- **gcc**, **make**: they will be used to compile and install it.
- **Nerd Fonts Symbols**, **Noto Sans**: these fonts provides the pretty symbols used in the prompt.

### Manual Procedures

If you want to install it manually, without a plugin manager or framework, follow these steps:

- Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams\
		    ~/.local/share/zsh/themes/riverdreams;
```

- Source its theme file in your ~/.zshrc startup file:

```sh
source ~/.local/share/zsh/themes/riverdreams/riverdreams.zsh-theme;
```

- Reopen zsh.

### Procedures For OhMyZSH

If you want to install and use it with the [OhMyZSH](https://github.com/ohmyzsh/ohmyzsh) framework, follow these steps:

- Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams\
		    ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/riverdreams;
```

- Apply the theme in your ~/.zshrc startup file:

```sh
ZSH_THEME="riverdreams/riverdreams";
```

- Reopen zsh.

### Procedures For Antidote

If you want to install it using the [antidote](https://github.com/mattmc3/antidote) plugin manager, follow these steps:

- Add this repository to your plugins file, usually ~/.zsh_plugins.txt:

```sh
skippyr/riverdreams
```

- Reopen zsh.

## ❡ Help

If you need help about this project, open a new issue in its [issues page](https://github.com/skippyr/riverdreams/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/riverdreams/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the same license that the project uses.

## ❡ License

This project is licensed under the MIT License. Refer to the LICENSE file that comes in its source code for license and copyright details.
