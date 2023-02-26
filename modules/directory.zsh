#!/usr/bin/env zsh

river_dreams::directory::get_directory_symbol() {
}

river_dreams::directory() {
  local -r current_directory=$(pwd)
  local directory_symbol=""

  [[ ${current_directory} == ${HOME} ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/Downloads ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/Documents ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/Pictures ]] &&
    directory_symbol="🖼 "
  [[ ${current_directory} == ${HOME}/Music ]] &&
    directory_symbol="🎜 "
  [[ ${current_directory} == ${HOME}/Public ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/Videos ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/Templates ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/.config ]] &&
    directory_symbol=" "
  [[ ${current_directory} == ${HOME}/.cache ]] &&
    directory_symbol=" "

  echo "%F{red}${directory_symbol}%F{green}%B%1~%b"
}
