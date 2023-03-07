#!/usr/bin/env zsh

river_dreams::docker_containers() {
  local -r active_docker_containers_quantity=$(
    docker ps 2>/dev/null | tail -n +2 | wc -l
  )
  
  if [[ ${active_docker_containers_quantity} -gt 0 ]]; then
    echo "%F{blue}󱣘 %f${active_docker_containers_quantity}"
  fi
}
