# River Dreams

## About

A theme for the ZSH shell with a design inspired by tribal drawing patterns.

Here is a preview that you can check out:

![](preview.png)

**Note:** the colors used in this shell theme depends on your terminal emulator's theme. This means that it may look different in your setup compared to the preview. The theme used is [Flamerial](https://github.com/skippyr/flamerial).

In the prompt, you will find:

-	Your host and local IPV4 address.
-	Your disk usage.
-	A calendar: displaying the day of the week, month and day.
-	A clock: displaying the hour and minute.
-	If you are the root user, as a hashtag symbol, `#`, will appear.
-	If your last command failed.
-	If you have sourced a virtual environment, its base name.
-	Your current directory path base name.

## Installation

### Dependencies

In order to install and run this software properly, the following dependencies must be installed:

-	`git`: required to clone this repository.
-	[Rust Toolchain](https://www.rust-lang.org/tools/install): required to compile the source code. It includes all the tools to develop in Rust, such as `rustc` and `cargo`. But it might also depend in you having a C compiler installed too: like `clang` or `gcc`.
-	[A Nerd Fonts' font](https://www.nerdfonts.com/font-downloads): required to provide the pretty symbols used in the prompt.

### Procedures

Using a command-line utility, follow these steps:

-	Clone this repository using `git`.

```bash
git\
    clone --depth 1 https://github.com/skippyr/river-dreams\
    ~/.local/share/zsh/themes/river-dreams
```

-	Add the following source rule to your `~/.zshrc` file. Ensure to not source any other theme to avoid causing conflicts.

```bash
source ~/.local/share/zsh/themes/river-dreams/river-dreams.zsh-theme
```

-	Open a new shell session.
-	Wait for the code to compile. Once done, the theme should be installed and running.

## Support

Report issues, questions and suggestions through its [issues page](https://github.com/skippyr/river-dreams/issues).

## Copyright

This software depends on different libraries. To check out what are those and their respective copyright notices, visit its [dependency graph](https://github.com/skippyr/river-dreams/network/dependencies).

This software is under the MIT license. A copy of the license is bundled with the source code.
