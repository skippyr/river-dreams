#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch --show-current 2>/dev/null
}

river_dreams::git::get_commit_hash() {
  git rev-parse --short HEAD 2>/dev/null
}

river_dreams::git::get_changes_quantity() {
  git ls-files -mo --exclude-standard | wc -l
}

river_dreams::git::get_staged_quantity() {
  git diff --name-only --cached | wc -l
}

river_dreams::git::get_diff() {
  git branch -v |
  awk -F '[][]' '{print $2}'
}

river_dreams::git::get_diff_status() {
  local -r diff="$1"
  echo "${diff}" | cut -f 1 -d " "
}

river_dreams::git::get_diff_quantity() {
  local -r diff="$1"
  echo "${diff}" | cut -f 2 -d " "
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)
  if [[ -n ${branch} ]]; then
    local commit_hash=$(river_dreams::git::get_commit_hash)
    [[ -n ${commit_hash} ]] && commit_hash=" ${commit_hash}"
    
    local changes_quantity=$(river_dreams::git::get_changes_quantity)
    local changes_section=""
    [[ ${changes_quantity} -gt 0 ]] && changes_section="%F{red}${changes_quantity}*%f"
    
    local staged_quantity=$(river_dreams::git::get_staged_quantity)
    local staged_section=""
    [[ ${staged_quantity} -gt 0 ]] && staged_section="%F{green}${staged_quantity}+%f"

    local -r diff=$(river_dreams::git::get_diff)
    local -r diff_status=$(river_dreams::git::get_diff_status "${diff}")
    local -r diff_quantity=$(river_dreams::git::get_diff_quantity "${diff}")
    local diff_symbol="↑"
    local diff_color="yellow"
    if [[ ${diff_status} == behind ]]; then
      diff_symbol="↓"
      diff_color="blue"
    fi
    local diff_section=""
    [[ ${diff_quantity} -gt 0 ]] && diff_section="%F{${diff_color}}${diff_quantity}${diff_symbol}%f"

    local status_section=(
      ${changes_section}
      ${staged_section}
      ${diff_section}
    )
    [[ -n ${status_section} ]] && status_section=" [${status_section}]"

    local tag=$(git describe --tags --abbrev=0 2>/dev/null)
    [[ -n ${tag} ]] && tag=" ${tag}"

    echo "%F{red}::«%f%B${branch}%b${status_section}%F{magenta}${tag}%F{yellow}${commit_hash}%F{red}»"
  fi
}
