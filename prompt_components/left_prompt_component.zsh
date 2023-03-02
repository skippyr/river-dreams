#!/usr/bin/env zsh

river_dreams::left_prompt_component() {
  echo $(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::decorator)$(river_dreams::directory)$(river_dreams::git)$(river_dreams::directory_ownership)
}
