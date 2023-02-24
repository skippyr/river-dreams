#!/usr/bin/env zsh

river_dreams::commands_separator() {
  for column in $(seq 1 ${COLUMNS}); do
    echo -n "▽"
  done
}
