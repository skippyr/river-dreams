#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  local git_short_status="$1"
  echo "${git_short_status}" | grep "#" | cut -f 2 -d " " | cut -f 1 -d "."
}

river_dreams::git::get_changes_quantity() {
  local git_short_status="$1"
  echo "${git_short_status}" | grep "[?NMDT] \S" | wc -l
}

river_dreams::git::get_commits_quantity() {
  local git_short_status="$1"
  echo "${git_short_status}" | grep "^[ANMDRUT].* " | wc -l
}

river_dreams::git::get_pushes_quantity() {
  local git_short_status="$1"
  echo  "${git_short_status}" | grep "#" | cut -f 4 -d " " | tr -d "]"
}

river_dreams::git() {
  local git_short_status=$(git status -sb 2>/dev/null)
  local -r branch=$(river_dreams::git::get_branch "${git_short_status}")

  [[ -z ${branch} ]] && exit

  local -r changes_quantity=$(river_dreams::git::get_changes_quantity "${git_short_status}")
  local -r changes_section=$(
    test ${changes_quantity} -gt 0 &&
    echo "${changes_quantity}* " ||
    echo ""
  )
  local -r commits_quantity=$(river_dreams::git::get_commits_quantity "${git_short_status}")
  local -r commit_section=$(
    test ${commits_quantity} -gt 0 &&
    echo "${commits_quantity}+ " ||
    echo ""
  )
  local -r pushes_quantity=$(river_dreams::git::get_pushes_quantity "${git_short_status}")
  local -r push_section=$(
    [[ ${pushes_quantity} =~ [0-9] && ${pushes_quantity} -gt 0 ]] &&
    echo "${pushes_quantity}↑ " ||
    echo ""
  )
  local -r last_commit_hash=$(git log -1 --pretty=oneline 2>/dev/null | cut -c -8)

  echo "%F{red}«%F{red}${changes_section}%F{green}${commit_section}%F{yellow}${push_section}%f${branch} %F{yellow}${last_commit_hash}%F{red}»%f"
}
