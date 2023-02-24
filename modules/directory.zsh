#!/usr/bin/env zsh

river_dreams::directory() {
  local -r current_directory=$(pwd)
  local directory_icon=""
  [[ ${current_directory} == ${HOME} ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Downloads ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Documents ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Pictures ]] &&
    directory_icon="🖼 "
  [[ ${current_directory} == ${HOME}/Music ]] &&
    directory_icon="🎜 "
  [[ ${current_directory} == ${HOME}/Public ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Videos ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Templates ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/.config ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/.cache ]] &&
    directory_icon=" "
  echo "%F{yellow}⤐ ${directory_icon}%F{green}%B%1~%b"
}
