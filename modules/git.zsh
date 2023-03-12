#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch --show-current 2>/dev/null
}

river_dreams::git::get_commit_hash() {
  git rev-parse --short HEAD 2>/dev/null
}

river_dreams::git::get_changes_quantity() {
  git ls-files -mo | wc -l
}

river_dreams::git::get_staged_quantity() {
  git diff --name-only --cached | wc -l
}

river_dreams::git::get_pushes_quantity() {
  git cherry 2>/dev/null | wc -l
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)
  [[ -z ${branch} ]] && exit
  local commit_hash=$(river_dreams::git::get_commit_hash)
  [[ -n ${commit_hash} ]] && commit_hash=" ${commit_hash}"
  local changes_quantity=$(river_dreams::git::get_changes_quantity)
  local changes_section=""
  [[ ${changes_quantity} -gt 0 ]] && changes_section="${changes_quantity}* "
  local staged_quantity=$(river_dreams::git::get_staged_quantity)
  local staged_section=""
  [[ ${staged_quantity} -gt 0 ]] && staged_section="${staged_quantity}+ "
  local pushes_quantity=$(river_dreams::git::get_pushes_quantity)
  local pushes_section=""
  [[ ${pushes_quantity} -gt 0 ]] && pushes_section="${pushes_quantity}↑ "
  echo "%F{red}«%F{yellow}${changes_section}%F{green}${staged_section}%F{blue}${pushes_section}%f${branch}%F{yellow}${commit_hash}%F{red}»"
}
