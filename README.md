# River Dreams


## Brief

"River Dreams" is prompt for ZSH that shows useful information about your
current environment:

  + local ip address.
  + exit code on errors.
  + current directory.
  + directory permissions.
  + git branch and status.
  + quantity of active docker containers.
  + quantity of files being untracked by git.
  + quantity of hidden files.


## Installation

  + Clone this repository to a separate directory, for example
    `~/.config/zsh/plugins/river_dreams`:

    ```bash
    mkdir -p ~/.config/zsh/plugins/river_dreams
    git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/plugins/river_dreams
    ```

    Use the flag `--depth` with value `1` to download only the latest commit.

  + Source the file `river_dreams.zsh` that is inside the repository's directory
    in your `~/.zshrc` file:

    ```bash
    source ~/.config/zsh/plugins/river_dreams/river_dreams.zsh
    ```

  + Refresh you ZSH session by reopening your terminal emulator or sourcing
    the `~/.zshrc`.

  If zsh is not your default shell, you can make it by using these two commands:

  ```bash
  sudo usermod -s /bin/zsh $(whoami)
  sudo usermod -s /bin/zsh root
  ```

  The first command will change your shell and the second one will change the
  root user one.

  Some terminal emulators, like Konsole and old versions of GNOME terminal, have
  a specific configuration in their menus to change the shell command they
  use, so changing the default shell for your user will not make a change
  in those applications. Edit that configuration to use `/bin/zsh` instead.

## License

This project is under the MIT license.
