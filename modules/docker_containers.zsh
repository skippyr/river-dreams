#!/usr/bin/env zsh

river_dreams::docker_containers() {
  local -r active_docker_containers_quantity=$(
    docker ps 2>/dev/null | tail -n +2 | wc -l
  )

  [[ ${active_docker_containers_quantity} -eq 0 ]] && exit

  local -r docker_containers_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "CONTAINERS" ||
    echo ""
  )
  
  echo "%F{blue}${docker_containers_symbol} %f${active_docker_containers_quantity}"
}
