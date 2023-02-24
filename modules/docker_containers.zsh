#!/usr/bin/env zsh

river_dreams::docker_containers::get_active_docker_containers_quantity() {
  docker ps 2>/dev/null | tail -n +2 | wc -l
}

river_dreams::docker_containers() {
  local -r docker_containers_symbol=${RIVER_DREAMS_DOCKER_CONTAINERS_SYMBOL:-󱣘 }
  local -r docker_containers_symbol_color=${RIVER_DREAMS_DOCKER_CONTAINERS_SYMBOL_COLOR:-blue}

  local -r active_docker_containers_quantity=$(river_dreams::docker_containers::get_active_docker_containers_quantity)
  
  if [[ ${active_docker_containers_quantity} -gt 0 ]]; then
    echo "%F{${docker_containers_symbol_color}}${docker_containers_symbol}%F{normal}${active_docker_containers_quantity}"
  fi
}
