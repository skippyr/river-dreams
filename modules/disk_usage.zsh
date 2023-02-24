#!/usr/bin/env zsh

river_dreams::disk_usage::get_usage_of_slash() {
  df |
  grep "/$" |
  awk '{print $5}' |
  tr -d "%"
}

river_dreams::disk_usage() {
  local -r disk_usage_symbol=${RIVER_DREAMS_DISK_USAGE_SYMBOL:- }
  local -r disk_usage_symbol_color=${RIVER_DREAMS_DISK_USAGE_SYMBOL_COLOR:-green}

  local -r disk_usage=$(river_dreams::disk_usage::get_usage_of_slash)

  echo "%F{${disk_usage_symbol_color}}${disk_usage_symbol}%F{normal}${disk_usage}%%"
}
