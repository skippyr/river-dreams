#!/usr/bin/env zsh

river_dreams::local_ip_address::get_local_ip_address_using_ip() {
  ip a 2>/dev/null |
  grep 'inet ' |
  grep -Ev "127.0.0|docker" |
  awk '{print $2}' |
  cut -f 1 -d /
}

river_dreams::local_ip_address::get_local_ip_address_using_ifconfig() {
  ifconfig 2>/dev/null |
  grep -A 1 RUNNING |
  grep 'inet ' |
  grep -v 127.0.0 |
  awk '{print $2}'
}

river_dreams::ip_address::get_local_ip_address() {
  [[ -x $(which ip 2>/dev/null) ]] &&
    river_dreams::local_ip_address::get_local_ip_address_using_ip ||
    river_dreams::local_ip_address::get_local_ip_address_using_ifconfig
}

river_dreams::local_ip_address() {
  local -r local_ip_address=$(river_dreams::ip_address::get_local_ip_address)

  [[ -z ${local_ip_address} ]] && exit

  local -r local_ip_address_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "IP" ||
    echo " "
  )
  echo "%F{red}${local_ip_address_symbol} %f${local_ip_address}"
}
