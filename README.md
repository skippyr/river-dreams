# River Dreams

The ZSH theme that developers have been dreaming for a long time. It will
finally show how you are the best terminal wizard out there while providing you
useful information about your development environment.


## Preview

![](images/preview/preview_0.png)


## What features does it have?

In the prompt you will find:
  + your local IP address.
  + current hours and minutes.
  + the disk usage percentage of the partition mounted at `/`.
  + sourced Python environment.
  + latest git tag.
  + exit status of last command if it throwed an error.
  + a decorator if you are root.
  + the current directory with an icon if it is a directory of the
    home of your user.
  + a decorator to show directory ownership.
  + git branch and its status with numeric indicators and decorators and the
    last commit hash.
  + the quantity of hidden files.
  + the quantity of files being ignored by git.
  + the quantity of active Docker containers in the background.
  + the quantity of jobs running in the background.
  + the time elapsed in the last command in minutes and seconds.

River Dreams will also use fallback text if it detects that your terminal
emulator only supports the 4-bits color palette. This is a measure that
allows you to use all its capabilities even in old terminals, that probably
does not support UTF-8.

## Installation/Uninstallation

Before installing the theme, you need to ensure that you have its required
dependencies installed:
  + the Noto Font family - this font has a variety of characters that can
    be used as a fallback by your terminal if your current font does not
    contains some symbols used in the theme.
  + a font patched by Nerd Fonts or only the Nerd Font symbols - this font will
    provide you the symbols used in the theme. You can download it in its
    official repository on GitHub.
  + `awk` or some variant - it will be used to parse commands.

Optionally, you can install these other dependencies to get more information
to show in the theme:
  + `git` - it will be used to provide Git information.
  + `docker` - it will be used to provide Docker information. Remember to
     activate its socket/daemon.
  + `ip` or `ipconfig` - it will be used to provide your local IP address.

If you are using Linux, you can probably download the dependencies using
your distribution's package manager.

If you are using MacOS, you can use [HomeBrew](https://brew.sh).

### Installing/Uninstalling With OhMyZSH
  + clone this repository to `${ZSH_CUSTOM}/themes` that is, by default, at
    `~/.oh-my-zsh/custom/themes`.

    ```bash
    git clone --depth 1 https://github.com/skippyr/river_dreams ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/themes/river_dreams
    ```
  + Set the variable `ZSH_THEME=river_dreams/river_dreams` in your `~/.zshrc`.
  + Refresh your ZSH by reopening your terminal emulator.

  If you want to uninstall it, use the following command:
  
  ```bash
  rm -rf ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/themes/river_dreams
  ```

### Installing/Uninstalling Manually
  + clone this repository to a separated directory, for example
    `~/.config/zsh/plugins/river_dreams`.

    ```bash
    git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/plugins/river_dreams
    ```
  + source the theme file `river_dreams.zsh-theme` that is inside the
    repository's directory in your `.zshrc`.

    ```bash
    source ~/.config/zsh/plugins/river_dreams/river_dreams.zsh-theme
    ```
  + Refresh your ZSH by reopening your terminal emulator.

  If you want to uninstall it, remove the directory you have cloned:

  ```bash
  rm -rf ~/.config/zsh/plugins/river_dreams
  ```

### Know Installation Issues

It has been detected that some Linux distributions that have their own default
configurations for `zsh` can result in River Dreams causing errors, but those
can be fixed if those default configurations are removed.

One example of distribution that caused this issue was `Kali Linux`.

## See Also

River Dreams is now part of the [`awesome-zsh-plugins`](https://github.com/unixorn/awesome-zsh-plugins)
repository. What about going there to find out more cool themes, frameworks and
plugins for ZSH?

Are you looking for a color scheme? I am proud to announce that
[`Tropical Paradise`](https://github.com/skippyr/tropical_paradise) has been
released with dark and light variants. What about checking it out?

## Issues, Enhancements And Questions

If you have found an issue while using this software or have an idea to
improve it, feel free to use the issues tab on its page on GitHub.

Questions can be made in the same way, but it would be better if you use
the Discussions tab instead.

## License

Copyright (c) 2023 Sherman Rofeman. MIT License.

