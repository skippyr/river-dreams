#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch 2>/dev/null | grep "*" | tr -d "* "
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
  git status 2>/dev/null | head -n 2 | grep -iv "on " | cut -f 8 -d " "
}

river_dreams::git() {
  local -r branch=$(river_dreams::git::get_branch)

  [[ -z ${branch} ]] && exit

  local git_short_status=$(git status -s 2>/dev/null)
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
  local -r pushes_quantity=$(river_dreams::git::get_pushes_quantity)
  local -r push_section=$(
    [[ ${pushes_quantity} =~ [0-9] && ${pushes_quantity} -gt 0 ]] &&
    echo "${pushes_quantity}↑ " ||
    echo ""
  )
  local -r last_commit_hash=$(git log -1 --pretty=oneline 2>/dev/null | cut -c -8)

  echo "%F{red}«%F{yellow}${changes_section}%F{green}${commit_section}%F{red}${push_section}%f${branch} %F{yellow}${last_commit_hash}%F{red}»%f"
}
