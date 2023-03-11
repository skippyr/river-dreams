#!/usr/bin/env zsh

river_dreams::root() {
  [[ ! $(whoami) == root ]] && exit
  echo "%F{yellow}[%F{red}#%F{yellow}]%f"
}
