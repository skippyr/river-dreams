# River Dreams

A fast ZSH theme for daily work.

![](images/image_0.png)
![](images/image_1.png)
![](images/image_2.png)

It shows:
  + current hours and minutes.
  + local ipv4 address.
  + disk usage percentage.
  + sourced Python virtual environment.
  + failed exit codes.
  + if you are root.
  + if you are owner of current directory.
  + current directory abbreviated like the `fish` shell.
  + current git branch.
  + hidden files quantity.
  + executable files quantity.
  + symbolic links quantity.


## Installation

  + install a font patched by [Nerd Fonts](https://github.com/ryanoasis/nerd-fonts).
  + install `gcc` and the standart C libraries. They usually come bundled with the compiler.
  + clone this repository to a directory, for example: `~/.config/zsh/themes/river_dreams`:
    ```bash
    git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/themes/river_dreams
    ```
  + add a source rule in your `~/.zshrc` pointing to the file `river_dreams.zsh-theme` inside the repository you have cloned.
    ```bash
    source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme
    ```
  + restart your zsh session.

The code will automatically be compiled. At any point, you can recompile those
again using the function `river_dreams::recompile`.


## Customization

You can use environment variables to change River Dreams behaviors. For boolean values, use C standart: `1` for true and `0` for false.

If the environment variable `RIVER_DREAMS_USE_FALLBACK_TEXT` is not set, it will set it with value `1` if your terminal only supports the 4-bits color pallete and `0` if not. This will allow you to use River Dreams even when your terminal does not support the symbols used by default.

If the environment variable `RIVER_DREAMS_USE_COLOR_VARIANTS` is not set, it will set it with value `0`. This changes the accent color of the separator from `yellow` to `blue` and allow you to better match River Dreams with light terminal color schemes.


## Issues And Questions

If you have found an issue while using this software or have an idea to improve it, feel free to use the issues tab on its page on GitHub.


## License

Copyright (c) 2023, Sherman Rofeman. MIT License.
