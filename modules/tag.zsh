#!/usr/bin/env bash

river_dreams::tag() {
  local -r tag=$(git tag 2>/dev/null | tail -n 1)
  local -r tag_symbol=$(
    test $(tput colors) -eq 8 &&
    echo "TAG" ||
    echo ""
  )
  if [[ -n ${tag} ]]; then
    echo "%F{red}${tag_symbol} %f${tag}"
  fi
}

