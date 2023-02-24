#!/usr/bin/env zsh

river_dreams::git::get_branch() {
  git branch 2>/dev/null | grep "*" | tr -d "* "
}

river_dreams::git::has_changes() {
  git status -s 2>/dev/null | grep "[?NMD].* " &>/dev/null
}

river_dreams::git::needs_to_commit() {
  git status -s 2>/dev/null | grep "A.* " &>/dev/null
}

river_dreams::git() {
  local -r git_color=${RIVER_DREAMS_GIT_COLOR:-red}
  local -r branch_symbol=${RIVER_DREAMS_GIT_BRANCH_SYMBOL:-}
  local -r branch_symbol_color=${RIVER_DREAMS_GIT_BRANCH_SYMBOL_COLOR:-yellow}
  local changes_symbol=${RIVER_DREAMS_GIT_CHANGES_SYMBOL:-✗}
  local -r changes_symbol_color=${RIVER_DREAMS_GIT_CHANGES_SYMBOL_COLOR:-magenta}
  local commit_symbol=${RIVER_DREAMS_GIT_COMMIT_SYMBOL:-+}
  local -r commit_symbol_color=${RIVER_DREAMS_GIT_COMMIT_SYMBOL_COLOR:-red}
  local push_symbol=${RIVER_DREAMS_GIT_PUSH_SYMBOL:-⥉}
  local -r push_symbol_color=${RIVER_DREAMS_GIT_PUSH_SYMBOL_COLOR:-green}

  local -r branch=$(river_dreams::git::get_branch)

  changes_symbol=$(
    river_dreams::git::has_changes &&
    echo "${changes_symbol}" ||
    echo ""
  )
  commit_symbol=$(
    river_dreams::git::needs_to_commit &&
    echo "${commit_symbol}" ||
    echo ""
  )
  push_symbol=$(
    git status 2>/dev/null |
    grep -i "your branch is ahead" &>/dev/null &&
    echo "${push_symbol}" ||
    echo ""
  )

  if [[ -n ${branch} ]]; then
    echo "%F{${git_color}}:git(%F{${branch_symbol_color}}${branch_symbol}%F{${changes_symbol_color}}${changes_symbol}%F{${commit_symbol_color}}${commit_symbol}%F{${push_symbol_color}}${push_symbol}%F{normal}${branch}%F{${git_color}})"
  fi
}
