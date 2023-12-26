# river-dreams
## About
A tribal-inspired ZSH theme for Linux.

![](assets/preview.png)
> In the preview, the terminal theme is [Flamerial](https://github.com/skippyr/flamerial)
> and font is [0xProto](https://github.com/0xType/0xProto) (Nerd Fonts patched).

It displays:

- Your local IPV4 address.
- Your disk use percentage.
- Your battery percentage.
- A calendar: displaying the day of the week, month and day of month.
- A clock: displaying the hour and minute.
- A decorator when you are the root user.
- A decorator for exit codes: both success and failure states.

## Requirements
In order to build it, you will need:

- A C99 compiler, like `gcc`, and `make`.
- The [Noto Sans](https://fonts.google.com/noto/specimen/Noto+Sans) font.
- A font patched by the [Nerd Fonts](https://www.nerdfonts.com/font-downloads)
  project.

## Install
- Edit the file `config.mk` to match your needs.
- Run the following command to install it (use `sudo` if necessary):

```sh
make install
```

- Add the following lines to your `~/.zshrc` configuration file and reopen ZSH. If you have changed the installation path in `config.mk`, you will need to update the path used below:

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

## Bugs
Report bugs at its [issues page](https://github.com/skippyr/river-dreams/issues).

## Patches
Send patches to be reviewed through its [pull requests page](https://github.com/skippyr/river-dreams/pulls).

All contributions must remain under its same license.

## Copyright
See `LICENSE` for copyright and license details.
