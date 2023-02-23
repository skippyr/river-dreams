#!/usr/bin/env zsh

setopt promptsubst
setopt +o nomatch

export VIRTUAL_ENV_DISABLE_PROMPT=1

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
    grep "[?NMD] " &>/dev/null &&
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

river_dreams::clock() {
  local -r clock=$(date +%Hh%Mm)
  echo "%F{yellow} %F{normal}${clock}"
}

river_dreams::ip_address() {
  local -r local_ip_address=$(
    test -x $(which ip 2>/dev/null) &>/dev/null &&
    ip -d a 2>/dev/null | grep 'inet ' | grep -v 127.0.0 | grep -v docker | awk '{print $2}' ||
    ifconfig 2>/dev/null | grep -A 1 RUNNING | grep 'inet ' | grep -v 127.0.0 | awk '{print $2}'
  )
  if [[ -n ${local_ip_address} ]]; then
    echo "%F{blue} %F{normal}${local_ip_address}"
  fi
}

river_dreams::python_environment() {
  if [[ -n ${VIRTUAL_ENV} ]]; then
    local -r python_environment=$(
      echo ${VIRTUAL_ENV} |
      rev |
      cut -f 1 -d / |
      rev
    )
    echo "%F{red}%F{normal} ${python_environment}"
  fi
}

river_dreams::hidden_files() {
  local -r hidden_files_quantity=$(ls -d .* 2>/dev/null | wc -l)
  if [[ ${hidden_files_quantity} -gt 0 ]]; then
    echo "%F{red} %F{normal}${hidden_files_quantity}"
  fi
}

river_dreams::untracked_files() {
  local -r untracked_files_quantity=$(
    git status -s --ignored 2>/dev/null |
    grep "! " |
    wc -l
  )
  if [[ ${untracked_files_quantity} -gt 0 ]]; then
    echo "%F{magenta}󰮀 %F{normal}${untracked_files_quantity}"
  fi
}

river_dreams::docker_containers() {
  local -r active_docker_containers_quantity=$(docker ps 2>/dev/null | tail -n +2 | wc -l)
  if [[ ${active_docker_containers_quantity} -gt 0 ]]; then
    echo "%F{blue}󱣘 %F{normal}${active_docker_containers_quantity}"
  fi
}

river_dreams::exit_code() {
  echo "%(?..%F{yellow}[%F{red}%B%?%b%F{yellow}])"
}

river_dreams::jobs() {
  echo "%(1j.%F{green} %F{normal}%j.)"
}

river_dreams::directory() {
  local -r current_directory=$(pwd)
  local directory_icon=""
  [[ ${current_directory} == ${HOME} ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Downloads ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Documents ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Pictures ]] &&
    directory_icon="🖼 "
  [[ ${current_directory} == ${HOME}/Music ]] &&
    directory_icon="🎜 "
  [[ ${current_directory} == ${HOME}/Public ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Videos ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/Templates ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/.config ]] &&
    directory_icon=" "
  [[ ${current_directory} == ${HOME}/.cache ]] &&
    directory_icon=" "
  echo "%F{yellow}⤐ ${directory_icon}%F{red}%B%1~%b"
}

river_dreams::time_elapsed() {
  local time_elapsed=$(history -D | tail -n 1 | awk '{print $2}')
  local time_elapsed_in_seconds=$(
    echo ${time_elapsed} |
    cut -f 2 -d :
  )
  local time_elapsed_in_minutes=$(
    echo ${time_elapsed} |
    cut -f 1 -d :    
  )
  if [[
    ${time_elapsed_in_seconds} -gt 0 ||
    ${time_elapsed_in_minutes} -gt 0
  ]]; then
    [[ ${time_elapsed_in_seconds} -lt 10 ]] &&
      time_elapsed_in_seconds="${time_elapsed_in_seconds:1:1}s" ||
      time_elapsed_in_seconds="${time_elapsed_in_seconds}s"
    [[ ${time_elapsed_in_minutes} -eq 0 ]] &&
      time_elapsed_in_minutes="" ||
      time_elapsed_in_minutes="${time_elapsed_in_minutes}m"
    echo "%F{yellow}%F{normal} ${time_elapsed_in_minutes}${time_elapsed_in_seconds}"
  fi
}

river_dreams::disk_usage() {
  local -r disk_usage=$(
    df |
    grep "/$" |
    awk '{print $5}' |
    tr -d "%"
  )
  echo "%F{green} %F{normal}${disk_usage}%%"
}

river_dreams::top_prompt() {
  local top_prompt_components=()

  top_prompt_components+=($(river_dreams::clock))
  top_prompt_components+=($(river_dreams::disk_usage))
  top_prompt_components+=($(river_dreams::ip_address))
  top_prompt_components+=($(river_dreams::python_environment))

  echo ${top_prompt_components[@]}
}

river_dreams::right_prompt() {
  local right_prompt_components=()

  right_prompt_components+=($(river_dreams::hidden_files))
  right_prompt_components+=($(river_dreams::untracked_files))
  right_prompt_components+=($(river_dreams::docker_containers))
  right_prompt_components+=($(river_dreams::jobs))
  right_prompt_components+=($(river_dreams::time_elapsed))

  echo ${right_prompt_components[@]}
}

PROMPT='$(river_dreams::triangles)$(river_dreams::top_prompt)
$(river_dreams::exit_code)$(river_dreams::root)$(river_dreams::directory)$(river_dreams::git)$(river_dreams::directory_permission)%F{normal} '
RPROMPT='$(river_dreams::right_prompt)'
