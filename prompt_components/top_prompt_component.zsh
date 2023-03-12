#!/usr/bin/env zsh

river_dreams::top_prompt_component() {
  local top_prompt_components=()

  top_prompt_components+=($(river_dreams::local_ip_address))
  top_prompt_components+=($(river_dreams::clock))
  top_prompt_components+=($(river_dreams::disk_usage))
  top_prompt_components+=($(river_dreams::python_environment))
  top_prompt_components+=($(river_dreams::docker_containers))
  top_prompt_components+=($(river_dreams::storage_devices))

  echo ${top_prompt_components[@]}
}
