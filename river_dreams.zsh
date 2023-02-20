#!/usr/bin/env zsh

setopt promptsubst

river_dreams::triangles() {
  for column in $(seq 1 ${COLUMNS}); do
    echo -n "▽"
  done
}

river_dreams::git() {
  local -r branch=$(git branch 2>/dev/null | grep "*" | tr -d "* ")
  local -r unstaged_changes_symbol=$(
    git status -s 2>/dev/null |
    grep "[?NM] " &>/dev/null &&
    echo "%F{magenta}✗" ||
    echo ""
  )
  local -r commit_changes_symbol=$(
    git status 2>/dev/null |
    grep "A " &>/dev/null &&
    echo "%F{red}+" ||
    echo ""
  )
  local -r pull_changes_symbol=$(
    git status 2>/dev/null |
    grep -i "your branch is ahead" &>/dev/null &&
    echo "%F{green}⇡" ||
    echo ""
  )
  if [[ -n ${branch} ]]; then
    echo "%F{blue}:git(%F{yellow}${unstaged_changes_symbol}${commit_changes_symbol}${pull_changes_symbol}%F{normal}${branch}%F{blue})"
  fi
}

PROMPT='$(river_dreams::triangles)%(?..%F{yellow}[%F{red}%B%?%b%F{yellow}])%F{yellow}⤐ %F{red}%B%1~%b$(river_dreams::git)%F{normal} '
