#!/usr/bin/env zsh

river_dreams::clock() {
  local -r clock=$(date +%Hh%Mm)
  echo "%F{yellow} %F{normal}${clock}"
}
