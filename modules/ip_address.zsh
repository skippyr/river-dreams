#!/usr/bin/env zsh

river_dreams::ip_address() {
  local -r local_ip_address=$(
    test -x $(which ip 2>/dev/null) &>/dev/null &&
    ip -d a 2>/dev/null | grep 'inet ' | grep -v 127.0.0 | grep -v docker | awk '{print $2}' ||
    ifconfig 2>/dev/null | grep -A 1 RUNNING | grep 'inet ' | grep -v 127.0.0 | awk '{print $2}'
  )
  if [[ -n ${local_ip_address} ]]; then
    echo "%F{red} %F{normal}${local_ip_address}"
  fi
}
