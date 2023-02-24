#!/usr/bin/env zsh

river_dreams::root() {
  local -r user=$(whoami)
  if [[ ${user} == root ]]; then
    echo "%F{yellow}(✪%F{normal}%F{yellow}✪)"
  fi
}
