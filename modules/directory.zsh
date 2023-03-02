#!/usr/bin/env zsh

river_dreams::directory() {
  local -r current_directory=$(pwd)
  local directory_symbol=""

  if [[ ${current_directory} == ${HOME} ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/Downloads ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/Documents ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/Pictures ]]; then
    directory_symbol="🖼 "
  elif [[ ${current_directory} == ${HOME}/Music ]]; then
    directory_symbol="🎜 "
  elif [[ ${current_directory} == ${HOME}/Public ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/Videos ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/Templates ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/.config ]]; then
    directory_symbol=" "
  elif [[ ${current_directory} == ${HOME}/.cache ]]; then
    directory_symbol=" "
  fi
  echo "%F{red}${directory_symbol}%F{green}%B%1~%b"
}
