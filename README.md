<h1 align="center">riverdreams</h1>

## About

It is a multi-line ZSH theme for Linux with a tribal inspired style, designed to
help you stay motivated while developing your dream projects.

![](preview.png)

In the preview above, the terminal theme used is
[flamerial](https://github.com/skippyr/flamerial) and font is
[intel-one-mono](https://github.com/intel/intel-one-mono).

Its prompt displays:

1. Your local IPV4 address.
2. Your disk use percentage.
3. Your battery status and percentage.
4. A calendar.
5. A clock.
6. A decorator when you are the root user.
7. A decorator for exit codes.
8. Your current directory.

## Install

### Dependencies

The following dependencies are required to install this theme in your system:

1. **git**: it will be used to clone this repository.
2. **gcc**, **make**: they will be used to compile and install it.
3. **Nerd Fonts Symbols**, **Noto Sans**: these fonts provides the pretty
   symbols used in the prompt.

### Procedures

Using a terminal, follow these steps:

1. Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams;
```

2. Access the repository directory using cd:

```sh
cd riverdreams;
```

3. Use make to compile and install this theme:

```sh
sudo make install clean;
```

4. Apply the theme in your ~/.zshrc startup file.

```sh
setopt promptsubst;
PROMPT='$(riverdreams)';
```

5. Reopen ZSH.

## Uninstall

Using a terminal, follow these steps:

1. Go back to the repository directory.
2. Use make to uninstall this theme:

```sh
sudo make uninstall;
```

### Help

If you need help about this project, open a new issue in its
[issues page](https://github.com/skippyr/riverdreams/issues) or send me an
[e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

### Contributing

This project is open to review and possibly accept contributions, specially
fixes and suggestions. If you are interested, send your contribution to its
[pull requests page](https://github.com/skippyr/riverdreams/pulls) or to my
[e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the
same license that the project uses.

### License

This project is licensed under the MIT License. Refer to the LICENSE file that
comes in its source code for license and copyright details.
