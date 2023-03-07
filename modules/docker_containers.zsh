#!/usr/bin/env zsh

river_dreams::docker_containers() {
  local -r active_docker_containers_quantity=$(
    docker ps 2>/dev/null | tail -n +2 | wc -l
  )
  local -r docker_containers_symbol=$(
    test $(tput colors) -eq 8 &&
    echo "CONTAINERS"
    echo "󱣘"
  )
  
  if [[ ${active_docker_containers_quantity} -gt 0 ]]; then
    echo "%F{blue}${docker_containers_symbol} %f${active_docker_containers_quantity}"
  fi
}
