#!/usr/bin/env zsh

river_dreams::local_ip_address::get_local_ip_address_using_ip() {
  echo "${$(
    ip -br a 2>/dev/null |
    awk '{if ($1 != "lo" && $1 !~ "docker[0-9]") {print $3; exit}}'
  )%/*}"
}

river_dreams::local_ip_address::get_local_ip_address_using_ifconfig() {
  ifconfig 2>/dev/null |
  grep "inet " |
  grep -Ev "127.0.0|172.17.0" |
  awk '{print $2}'
}

river_dreams::ip_address::get_local_ip_address() {
  [[ -x $(which ifconfig 2>/dev/null) ]] &&
    river_dreams::local_ip_address::get_local_ip_address_using_ifconfig ||
    river_dreams::local_ip_address::get_local_ip_address_using_ip
}

river_dreams::local_ip_address() {
  local -r local_ip_address=$(river_dreams::ip_address::get_local_ip_address)
  [[ -z ${local_ip_address} ]] && exit
  local -r local_ip_address_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "IP" ||
    echo ""
  )
  echo "%F{red}${local_ip_address_symbol} %f${local_ip_address}"
}
