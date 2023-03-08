#!/usr/bin/env zsh

river_dreams::local_ip_address::get_local_ip_address_using_ip() {
  ip -d a 2>/dev/null | grep 'inet ' | grep -v 127.0.0 | grep -v docker | awk '{print $2}'
}

river_dreams::local_ip_address::get_local_ip_address_using_ipconfig() {
  ifconfig 2>/dev/null | grep -A 1 RUNNING | grep 'inet ' | grep -v 127.0.0 | awk '{print $2}'
}

river_dreams::ip_address::get_local_ip_address() {
  test -x $(which ip 2>/dev/null) &>/dev/null &&
  river_dreams::local_ip_address::get_local_ip_address_using_ip ||
  river_dreams::local_ip_address::get_local_ip_address_using_ipconfig
}

river_dreams::local_ip_address() {
  local -r local_ip_address=$(river_dreams::ip_address::get_local_ip_address)
  local -r local_ip_address_symbol=$(
    test ${RIVER_DREAMS_FALLBACK_TEXT} == true &&
    echo "IP" ||
    echo " "
  )
  
  if [[ -n ${local_ip_address} ]]; then
    echo "%F{red}${local_ip_address_symbol} %f${local_ip_address}"
  fi
}
