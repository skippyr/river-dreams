# river-dreams

## About

A tribal-inspired ZSH theme for Linux designed to be used in simple
environments without a status bar, such as tiling window managers. It displays:

- Your local IPV4 address.
- Your disk use percentage.
- Your battery percentage.
- A calendar: displaying the day of the week, month and day of month.
- A clock: displaying the hour and minute.
- A decorator when you are the root user.
- A decorator for exit codes: both success and failure states.

![](preview.png)
> In the preview, the theme is [Flamerial](https://github.com/skippyr/flamerial)
> and the font is Iosevka (Nerd Fonts patched).

## Install

### Dependencies

The following dependencies are required to install it:

- The version control software `git` will be used to download this repository.
- The GNU core utilities and C build utilites, such as `gcc` and `make`, will be
  used to compile and install this software in your system. They may also
  include the standard C headers.
- A font patched by the Nerd Fonts project and Noto Sans will provide the cool
  symbols used in the prompt. To avoid font conflicts, which can lead to badly
  rendered symbols, it is recommended to only have one Nerd Fonts font
  installed.

Generally, you are able to install most these dependencies through the use of
your Linux distro package manager. Some dependencies may already be installed in
your operating system.

### Step-By-Step Procedures

After its dependencies have been met, to install it, follow this step-by-step
using a command-line utility:

- Clone this repository using `git`.

```sh
git clone --depth 1 https://github.com/skippyr/river-dreams
```

- Access the repository directory using `cd`.

```sh
cd river-dreams
```

- Use `make` to install it.

```sh
sudo make install clean
```

- Apply the theme in your `~/.zshrc` configuration file by adding these lines
  to it:

```sh
setopt promptsubst
PROMPT='$(river-dreams)'
```

- Reopen zsh.

If you want a custom install, you can configure the build process by editing the
`config.mk` file.

## Uninstall

To uninstall it, follow this step-by-step using a command-line utility:

- Go back to the repository directory.
- Use `make` to uninstall it.

```sh
sudo make uninstall clean
```

## Help

You can ask for help related to this project through its [issues page](https://github.com/skippyr/river-dreams/issues).

## Contributing

This project is open to receive contributions from people of any skill level.
You can send code contributions to be reviewed through its [pull requests page](https://github.com/skippyr/river-dreams/pulls).

By contributing to this project, you agree to use the same copyright used by it.

## Copyright

MIT License\
Copyright (c) 2023, Sherman Rofeman \<skippyr.developer@gmail.com\>

See the `LICENSE` file that comes in its source code for more details.
