#!/usr/bin/env zsh

function zle-line-init zle-keymap-select {
    export RIVER_DREAMS_VI_MODE=${${KEYMAP/vicmd/N}/main/I}
    zle reset-prompt
}

zle -N zle-line-init
zle -N zle-keymap-select

river_dreams::vi_mode() {
  if [[ -n ${RIVER_DREAMS_VI_MODE} ]]; then
    local symbol=""
    case ${RIVER_DREAMS_VI_MODE} in
      I)
        symbol="%F{green}I"
        ;;
      N)
        symbol="%F{red}N"
        ;;
    esac
    echo "%F{yellow}[%B${symbol}%b%F{yellow}]"
  fi
}

