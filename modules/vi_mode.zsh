#!/usr/bin/env zsh

function zle-line-init zle-keymap-select {
    export RIVER_DREAMS_VI_MODE_SYMBOL="%F{red}?"
    case ${KEYMAP} in
      main)
        export RIVER_DREAMS_VI_MODE_SYMBOL="%F{green}I"
        ;;
      vicmd)
        export RIVER_DREAMS_VI_MODE_SYMBOL="%F{red}N"
        ;;
    esac
    zle reset-prompt
}

zle -N zle-line-init
zle -N zle-keymap-select

river_dreams::vi_mode() {
  echo "%F{yellow}[%B${RIVER_DREAMS_VI_MODE_SYMBOL}%b%F{yellow}]"
}

