#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

river_dreams::triangles() {
  for column in $(seq 1 ${COLUMNS}); do
    echo -n "▽"
  done
}

river_dreams::root() {
  local -r user=$(whoami)
  if [[ ${user} == root ]]; then
    echo "%F{yellow}(✪%F{normal}%F{yellow}✪)"
  fi
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
    git status -s 2>/dev/null |
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

river_dreams::directory_permission() {
  local -r user=$(whoami)
  local -r directory_owner=$(ls -ld | awk '{print $3}')
  if [[ ${user} != root && ${user} != ${directory_owner} ]]; then
    echo "%F{normal} "
  fi
}

river_dreams::top_prompt() {
  local top_prompt_components=()

  local -r local_ip_address=$(
    ip a |
    grep "inet " |
    grep -v 127.0.0 |
    head -n 1 |
    awk '{print $2}'
  )
  if [[ -n ${local_ip_address} ]]; then
    top_prompt_components+=("%F{blue} %F{normal}${local_ip_address}")
  fi

  echo ${top_prompt_components[@]}
}

river_dreams::right_prompt() {
  local right_prompt_components=()

  local -r hidden_files_quantity=$(ls -d .* 2>/dev/null | wc -l)
  if [[ ${hidden_files_quantity} -gt 0 ]]; then
    right_prompt_components+=("%F{red} %F{normal}${hidden_files_quantity}")
  fi

  local -r untracked_files_quantity=$(
    git status -s --ignored 2>/dev/null |
    grep "! " |
    wc -l
  )
  if [[ ${untracked_files_quantity} -gt 0 ]]; then
    right_prompt_components+=("%F{magenta}󰮀 %F{normal}${untracked_files_quantity}")
  fi

  local -r active_docker_containers_quantity=$(docker ps 2>/dev/null | tail -n +2 | wc -l)
  if [[ ${active_docker_containers_quantity} -gt 0 ]]; then
    right_prompt_components+=("%F{blue}󱣘 %F{normal}${active_docker_containers_quantity}")
  fi

  echo ${right_prompt_components[@]}
}

PROMPT='$(river_dreams::triangles)$(river_dreams::top_prompt)
%(?..%F{yellow}[%F{red}%B%?%b%F{yellow}])$(river_dreams::root)%F{yellow}⤐ %F{red}%B%1~%b$(river_dreams::git)$(river_dreams::directory_permission)%F{normal} '
RPROMPT='$(river_dreams::right_prompt)'
