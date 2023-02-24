#!/usr/bin/env zsh

river_dreams::directory::get_directory_symbol() {
  local -r home_symbol=${RIVER_DREAMS_DIRECTORY_HOME_SYMBOL:- }
  local -r downloads_symbol=${RIVER_DREAMS_DIRECTORY_DOWNLOADS_SYMBOL:- }
  local -r documents_symbol=${RIVER_DREAMS_DIRECTORY_DOCUMENTS_SYMBOL:- }
  local -r pictures_symbol=${RIVER_DREAMS_DIRECTORY_PICTURES_SYMBOL:-🖼 }
  local -r music_symbol=${RIVER_DREAMS_DIRECTORY_MUSIC_SYMBOL:-🎜 }
  local -r public_symbol=${RIVER_DREAMS_DIRECTORY_PUBLIC_SYMBOL:- }
  local -r videos_symbol=${RIVER_DREAMS_DIRECTORY_VIDEOS_SYMBOL:- }
  local -r templates_symbol=${RIVER_DREAMS_DIRECTORY_TEMPLATES_SYMBOL:- }
  local -r config_symbol=${RIVER_DREAMS_DIRECTORY_CONFIG_SYMBOL:- }
  local -r cache_symbol=${RIVER_DREAMS_DIRECTORY_CACHE_SYMBOL:- }

  local -r current_directory=$(pwd)

  [[ ${current_directory} == ${HOME} ]] &&
    echo ${home_symbol}
  [[ ${current_directory} == ${HOME}/Downloads ]] &&
    echo ${downloads_symbol}
  [[ ${current_directory} == ${HOME}/Documents ]] &&
    echo ${documents_symbol}
  [[ ${current_directory} == ${HOME}/Pictures ]] &&
    echo ${pictures_symbol}
  [[ ${current_directory} == ${HOME}/Music ]] &&
    echo ${music_symbol}
  [[ ${current_directory} == ${HOME}/Public ]] &&
    echo ${public_symbol}
  [[ ${current_directory} == ${HOME}/Videos ]] &&
    echo ${videos_symbol}
  [[ ${current_directory} == ${HOME}/Templates ]] &&
    echo ${templates_symbol}
  [[ ${current_directory} == ${HOME}/.config ]] &&
    echo ${config_symbol}
  [[ ${current_directory} == ${HOME}/.cache ]] &&
    echo ${cache_symbol}
}

river_dreams::directory() {
  local -r directory_color=${RIVER_DREAMS_DIRECTORY_COLOR:-green}
  local -r directory_symbol_color=${RIVER_DREAMS_DIRECTORY_SYMBOL_COLOR:-red}
  
  local -r directory_symbol=$(river_dreams::directory::get_directory_symbol)

  echo "%F{${directory_symbol_color}}${directory_symbol}%F{${directory_color}}%B%1~%b"
}
