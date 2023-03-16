#!/usr/bin/env zsh

river_dreams::directory::get_directory_abbreviated() {
  local directory=${PWD}
  local user=$(whoami)
  [[ ${user} == root ]] &&
    directory=($(echo "${directory}" | sed "s/\/root/~/")) ||
    directory=($(echo "${directory}" | sed "s/\/home\/${user}/~/"))
  local directory_path_splitted=(${(s./.)directory})
  local directory_path_abbreviated=""
  [[ ! ${directory_path_splitted[1]} == "~" ]] && directory_path_abbreviated+="/"
  local last_directory_path_split_index=$((${#directory_path_splitted[@]} - 1))
  local index=0
  for directory_path_split in ${directory_path_splitted[@]}; do
    if [[ ${index} -eq ${last_directory_path_split_index} ]]; then
      directory_path_abbreviated+=${directory_path_split}
    else
      [[ ${directory_path_split:0:1} == "." ]] &&
        directory_path_abbreviated+=${directory_path_split:0:2}/ ||
        directory_path_abbreviated+=${directory_path_split:0:1}/
    fi
    ((index++))
  done
  echo ${directory_path_abbreviated}
}

river_dreams::directory() {
  local directory_symbol=""
  [[ ! ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    case $(pwd) in
      ${HOME})
        directory_symbol=" "
        ;;
      ${HOME}/Downloads)
        directory_symbol=" "
        ;;
      ${HOME}/Documents)
        directory_symbol=" "
        ;;
      ${HOME}/Pictures)
        directory_symbol=" "
        ;;
      ${HOME}/Music)
        directory_symbol="🎜 "
        ;;
      ${HOME}/Public)
        directory_symbol=" "
        ;;
      ${HOME}/Vides)
        directory_symbol="󰨜 "
        ;;
      ${HOME}/Templates)
        directory_symbol=" "
        ;;
      ${HOME}/.local)
        directory_symbol=" "
        ;;
      ${HOME}/.config)
        directory_symbol=" "
        ;;
      ${HOME}/.cache)
        directory_symbol=" "
        ;;
    esac
  local -r directory_abbreviated=$(river_dreams::directory::get_directory_abbreviated)
  echo "%F{red}${directory_symbol}%F{blue}%B${directory_abbreviated}%b%f"
}
