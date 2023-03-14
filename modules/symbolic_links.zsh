#!/usr/bin/env zsh

river_dreams::symbolic_links() {
  local -r symbolic_links_quantity=$(find -maxdepth 1 -type l | wc -l)
  [[ ${symbolic_links_quantity} -eq 0 ]] && exit
  local -r symbolic_links_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
      echo "SYMLINKS" ||
      echo ""
  )
  echo "%F{blue}${symbolic_links_symbol} %f${symbolic_links_quantity}"
}
