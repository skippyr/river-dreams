#!/usr/bin/env zsh

river_dreams::user() {
  local -r user=$(whoami)
  local symbol=" "
  local color="green"
  [[ ${user} == root ]] && symbol="🩑 " && color="magenta"
  echo "%F{yellow}[%F{${color}}${symbol}%F{normal}%B${user}%b%F{yellow}]"
}
