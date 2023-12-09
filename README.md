# River Dreams
## About
A Linux-only ZSH theme with tribal design.

![](preview.png)

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
  project.

## Installation
- Compile the file `river-dreams.c`.

```c
mkdir -p ~/.local/bin
cc -O3 -o ~/.local/bin/river-dreams river-dreams.c
```

- Make its output be your ZSH prompt by adding these lines to your `~/.zshrc`
  config file.

```c
setopt promptsubst
PROMPT='$(~/.local/bin/river-dreams)'
```

- Reopen ZSH.

## Copyright
See `LICENSE` for copyright and license details.
