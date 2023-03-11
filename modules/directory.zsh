#!/usr/bin/env zsh

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
      ${HOME}/.config)
        directory_symbol=" "
        ;;
      ${HOME}/.cache)
        directory_symbol=" "
        ;;
    esac
  echo "%F{red}${directory_symbol}%F{cyan}%B%1~%b%f"
}
