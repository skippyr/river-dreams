#!/usr/bin/env zsh

river_dreams::disk_usage() {
  local -r disk_usage=$(
    df |
    grep "/$" |
    awk '{print $5}' |
    tr -d "%"
  )
  echo "%F{green} %F{normal}${disk_usage}%%"
}
