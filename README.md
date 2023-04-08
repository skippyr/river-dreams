# River Dreams

A fast and versatile ZSH theme for programmers and hackers.

> River Dreams with pretty symbols.
![](images/image_0.png)
> River Dreams with fallback text.
![](images/image_1.png)

It shows:
+ current hours and minutes in 24h format.
+ local ipv4 address and hostname.
+ disk usage percentage.
+ sourced Python virtual environment.
+ exit code of failed commands.
+ current user.
+ current directory path abbreviated just like what the `fish` shell does.
+ a decorator if you are not the user owner of the current directory.
+ current git branch, if in a directory being tracked by git.
+ a decorator to help you find where you can type your commands.
+ background jobs quantity.
+ hidden files quantity for the current directory.
+ executable files quantity for the current directory.
+ symbolic links quantity for the current directory.

It also contains fallback text that can be used when your terminal can not render pretty symbols.


## Installation

+ install `gcc` and the standard C libraries. You will probably have them already installed as those are, usually, system dependencies.
+ optionally, install the [Noto Sans](https://fonts.google.com/noto/specimen/Noto+Sans) font - this one will be probably installed too - and use a font patched by [Nerd Fonts](https://github.com/ryanoasis/nerd-fonts) in your terminal to see pretty symbols. If you do not use them, you can enable River Dreams's fallback text.
+ clone this repository to a directory, for example: `~/.config/zsh/themes/river_dreams`:

	```bash
	git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/themes/river_dreams
	```
If you do not have `git` installed, you can download the source code from the page on GitHub.

+ add a source rule in your `~/.zshrc` pointing to the file `river_dreams.zsh-theme` inside the repository you have cloned.

	```bash
	source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme
	```

+ restart your ZSH session.

The source codes will automatically be compiled and start running. If you have modified the source codes, you can use the function `river_dreams::compile_source_files` to recompile them again.


## Customization

You can set environment variables to change River Dreams behaviors, even while it is already running. For boolean values, use C standard: `1` for true and `0` for false.

Set the environment variable `RIVER_DREAMS_USE_FALLBACK_TEXT` to enable and disable fallback text. If it is not set, it will be set with value `1` if your terminal only supports the 4-bits color pallete and `0` if not.


## Issues And Questions

If you have found an issue while using this software or have an idea to improve it, feel free to use the issues tab on its page on GitHub.


## License

Copyright (c) 2023, Sherman Rofeman. MIT License.

