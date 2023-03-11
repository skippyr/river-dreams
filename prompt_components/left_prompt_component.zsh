#!/usr/bin/env zsh

river_dreams::left_prompt_component() {
  # echo $(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::vi_mode)$(river_dreams::decorator)$(river_dreams::directory)$(river_dreams::git)$(river_dreams::directory_ownership)
  echo "$(river_dreams::directory)"
}
