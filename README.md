# River Dreams
## About
A theme for the ZSH shell with a design inspired by tribal patterns.
![](preview.png)

It can show you:
-	Your host name.
-	Your local IPV4 address.
-	Your disk usage.
-	A calendar: displaying week day, month and day.
-	A clock: displaying hour and minute.
-	If your last command has failed.
-	Your current directory.

## Installation
### Dependencies
-	Rust Toolchain.

### Procedures
-	Clone this repository.
```bash
git\
   clone --depth=1 https://github.com/skippyr/river-dreams\
   ~/.local/share/zsh/themes/river-dreams
```

-	Add the following source rule to your `~/.zshrc` file. Ensure to not source other theme.
```bash
source ~/.local/share/zsh/themes/river-dreams/river-dreams.zsh-theme
```
-	Reopen ZSH.
-	Wait for the code to compile.

## Copyright
All dependencies used by this software and their copyright noticies can be found in the [dependency graph](https://github.com/skippyr/river-dreams/network/dependencies).

This software is under the MIT license. A copy of the license is bundled with the source code.
