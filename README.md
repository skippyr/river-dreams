# riverdreams

## About

A tribal inspired ZSH theme for Linux that displays:

- Your local IPV4 address.
- Your disk use percentage.
- Your battery percentage.
- A calendar.
- A clock.
- A decorator when you are the root user.
- A decorator for exit codes.
- The current directory.

![](preview.png)

## Install

### Dependencies

- The Nerd Fonts Symbols font.
- Noto Sans.
- `gcc`
- `make`

### Step-By-Step

- Use `make` to install it.

```sh
sudo make install clean;
```

- Add these lines to `~/.zshrc` to apply it:

```sh
setopt promptsubst;
PROMPT='$(riverdreams)';
```

- Reopen `zsh`.

## Uninstall

- Use `make` to uninstall it.

```sh
sudo make uninstall;
```

## Help

You can ask for help related to this project through its [issues page](https://github.com/skippyr/riverdreams/issues).

## Contributing

This project is open to receive contributions.  You can send code contributions
to be reviewed through its [pull requests page](https://github.com/skippyr/riverdreams/pulls).

By contributing to this project, you agree to use the same copyright used by it.

## Copyright

MIT License\
Copyright (c) 2023, Sherman Rofeman \<skippyr.developer@gmail.com\>

See the `LICENSE` file that comes in its source code for more details.
