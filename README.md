# river-dreams

## About

A tribal-inspired ZSH theme for Linux.

![](preview.png)
> In the preview, the theme is [flamerial](https://github.com/skippyr/flamerial)
> and the font is [Iosevka](https://github.com/be5invis/Iosevka) (Nerd Fonts
> patched).

It displays:

- Your local IPV4 address.
- Your disk use percentage.
- Your battery percentage.
- A calendar: displaying the day of the week, month and day of month.
- A clock: displaying the hour and minute.
- A decorator when you are the root user.
- A decorator for exit codes: both success and failure states.

## Install

### Dependencies

The following dependencies are required to install it:

- The version control software `git` will be used to download this repository.
- The GNU `coreutils` utilities and C build utilites, such as `gcc` and `make`
  will be used to compile and install this library in your system.

Generally, you are able to install these dependencies through the use of your
Linux distro package manager, such as `apt`, `dnf` or `pacman`. Here are some
commands you can use for those:

```sh
# For apt (Debian based distros).
sudo apt update -y && sudo apt install -y git build-essential

# For dnf (Fedora based distros).
sudo dnf update -y && sudo dnf -y install git gcc make

# For pacman (Arch Linux based distros).
sudo pacman --noconfirm --needed -Syu git base-devel
```

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
sudo make uninstall
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
