# River Dreams
## About
A linux-only ZSH theme with a tribal looking. It was designed to be a great
addition for minimalist window manager environments: by including a status bar,
it makes it easier to find some basic info about the operating system.

![](preview.png)

> [!NOTE]
> The colors used in this theme are affected by the terminal theme settings.
> Theme used is [Flamerial](https://github.com/skippyr/flamerial). Font used is
> Mononoki (nerd font patched).

It displays:

- Your local IPV4 address.
- The disk use percentage.
- The battery percentage and its status, if any.
- The day of week, month and day of month.
- The hour and minute.
- A decorator when root.
- A decorator for exit codes.
- The directory path.

## Requirements
In order to build it, you will need:

- A C compiler, like GCC.
- [Noto Sans](https://fonts.google.com/noto/specimen/Noto+Sans).
- A font patched by the [Nerd Fonts](https://www.nerdfonts.com/font-downloads)
  project (release `>=3.0.0,<4.0.0`).

## Installation
- Compile the file `river-dreams.c`.

```bash
mkdir -p ~/.local/bin
cc -ansi -O3 -o ~/.local/bin/river-dreams river-dreams.c
```

- Make its output be your ZSH prompt by adding these lines to your `~/.zshrc`
  config file. Ensure to not be sourcing any other theme.

```zsh
setopt promptsubst
PROMPT='$(~/.local/bin/river-dreams)'
```

- Reopen ZSH.

## Copyright
See `LICENSE` for copyright and license details.
