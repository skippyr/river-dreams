<p align="center">
	<img src="imgs/ornament.webp" alt="" />
</p>
<h1 align="center">⊼⊵⊼⊵&ensp;riverdreams&ensp;⊼⊵⊼⊵</h1>
<p align="center">
	<img src="https://img.shields.io/github/license/skippyr/riverdreams?style=plastic&label=%E2%8A%B5%20license&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/v/tag/skippyr/riverdreams?style=plastic&label=%E2%8A%B5%20tag&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/commit-activity/t/skippyr/riverdreams?style=plastic&label=%E2%8A%B5%20commits&labelColor=%2324130e&color=%23b8150d" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/stars/skippyr/riverdreams?style=plastic&label=%E2%8A%B5%20stars&labelColor=%2324130e&color=%23b8150d" alt="" />
</p>

## ❡ About

A tribal inspired ZSH theme for Linux designed to help you develop your dream projects.

<p align="center"><img src="imgs/preview.webp" alt="" /></p>
<p align="center"><sup><strong>Caption:</strong> a preview of the riverdreams theme applied on zsh. The terminal theme used is <a href="https://github.com/skippyr/flamerial">flamerial</a> and font is <a href="https://github.com/be5invis/Iosevka">iosevka</a>.</sup></p>

### ¦ Features

Its left prompt displays:

&emsp;**1.**&ensp;Your local IPV4 address.\
&emsp;**2.**&ensp;Your disk use percentage.\
&emsp;**3.**&ensp;Your battery status and percentage, if any.\
&emsp;**4.**&ensp;A calendar.\
&emsp;**5.**&ensp;A clock.\
&emsp;**6.**&ensp;A decorator when you are the root user.\
&emsp;**7.**&ensp;A decorator for exit codes.\
&emsp;**8.**&ensp;The active virtual environment, if one has been sourced.\
&emsp;**9.**&ensp;The active directory path, shorting it inside of a git repo.\
&emsp;**10.**&ensp;The active branch, when inside of a git repo.\
&emsp;**11.**&ensp;A decorator when you can not modify the active directory.

Its right prompt displays:

&emsp;**1.**&ensp;An overview of the entries in the active directory:\
&emsp;&emsp;**1.1.**&ensp;The total of hidden entries.\
&emsp;&emsp;**1.2.**&ensp;The total of temporary entries.\
&emsp;&emsp;**1.3.**&ensp;The total of regular entries that are executables.\
&emsp;&emsp;**1.4.**&ensp;The total of symlinks.\
&emsp;&emsp;**1.5.**&ensp;The total of regular entries.\
&emsp;&emsp;**1.6.**&ensp;The total of directories.\
&emsp;&emsp;**1.7.**&ensp;The total of block devices.\
&emsp;&emsp;**1.8.**&ensp;The total of character devices.\
&emsp;&emsp;**1.9.**&ensp;The total of fifos.\
&emsp;&emsp;**1.10.**&ensp;The total of sockets.\
&emsp;**2.**&ensp;The total of background jobs.

## ❡ Install

### ¦ Dependencies

The following dependencies are required to install it:

&emsp;**1.**&ensp;**git**: it will be used to clone this repository.\
&emsp;**2.**&ensp;**gcc**, **make**: they will be used to compile and install it.\
&emsp;**3.**&ensp;**Nerd Fonts Symbols**, **Noto Sans**: these fonts provides the pretty symbols used in the prompt.

### ¦ Manual Procedures

If you want to install it manually, without a plugin manager or framework, follow these steps:

&emsp;**1.**&ensp;Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams\
		    ~/.local/share/zsh/themes/riverdreams;
```

&emsp;**2.**&ensp;Source its theme file in your ~/.zshrc startup file.

```sh
source ~/.local/share/zsh/themes/riverdreams/riverdreams.zsh-theme;
```

&emsp;**3.**&ensp;Reopen zsh.

### ¦ Procedures For OhMyZSH

If you want to install and use it with the [OhMyZSH](https://github.com/ohmyzsh/ohmyzsh) framework, follow these steps:

&emsp;**1.**&ensp;Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams\
		    ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/riverdreams;
```

&emsp;**2.**&ensp;Set ZSH\_THEME="riverdreams/riverdreams" in your ~/.zshrc startup file.\
&emsp;**3.**&ensp;Reopen zsh.

### ¦ Procedures For Antidote

If you want to install it using the [antidote](https://github.com/mattmc3/antidote) plugin manager, follow these steps:

&emsp;**1.**&ensp;Add this repository to your plugins file, usually ~/.zsh_plugins.txt:

```sh
skippyr/riverdreams
```

&emsp;**2.**&ensp;Reopen zsh.

## ❡ Help

If you need help about this project, open a new issue in its [issues page](https://github.com/skippyr/riverdreams/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/riverdreams/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the same license that the project uses.

## ❡ License

This project is licensed under the MIT License. Refer to the LICENSE file that comes in its source code for license and copyright details.

&nbsp;

<p align="center"><sup><strong>⊼⊵⊼⊵&ensp;Here Are Dragons!&ensp;⊼⊵⊼⊵</strong><br />Made with love by skippyr <3</sup></p>
