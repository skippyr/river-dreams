# river-dreams
A tribal-inspired ZSH theme for Linux.

![](preview.png)
> In the preview, the terminal theme is [Flamerial](https://github.com/skippyr/flamerial)
> and font is [Mononoki](https://github.com/madmalik/mononoki) (Nerd Fonts patched).

It displays:

- Your local IPv4 address.
- Your disk usage.
- Your battery percentage.
- A calendar.
- A clock.
- A decorator for root user.
- A decorator for exit codes.

## Requirements
In order to build it, you will need:

- A C99 compiler, like `gcc`, and `make`.
- The [Noto Sans](https://fonts.google.com/noto/specimen/Noto+Sans) font.
- A font patched by the [Nerd Fonts](https://www.nerdfonts.com/font-downloads)
  project (release `>= 3.0.0 && <= 4.0.0`).

## Install
Edit the file `config.mk` to match your needs. Then, run the following command
to install it (use `sudo` if necessary):

```sh
make install
```

After it, add the following lines to your `~/.zshrc` file and reopen ZSH:

```sh
setopt promptsubst
PROMPT='$(river-dreams)'
```

## Uninstall
If you want to uninstall it, run the following command (use `sudo` if
necessary):

```sh
make uninstall
```

## Copyright
See `LICENSE` for copyright and license details.
