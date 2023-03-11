#!/usr/bin/env zsh

river_dreams::storage_devices() {
  local -r system_storage_device=$(
    lsblk -l |
    grep /$ |
    awk '{print $1}' |
    tr -d 0-9
  )
  local -r external_storage_devices=($(
    lsblk |
    grep disk |
    grep -vi swap |
    awk '{print $1}' |
    grep -v ${system_storage_device}
  ))

  [[ ${#external_storage_devices[@]} -eq 0 ]] && exit

  local mounted_external_storage_devices_quantity=0

  for external_storage_device in ${external_storage_devices[@]}; do
    local -r mountpoints=($(lsblk | grep ${external_storage_device} | awk '{print $7}'))
    [[ ${#mountpoints} -gt 0 ]] && ((mounted_external_storage_devices_quantity++))
  done

  local mount_section=""
  [[ ${mounted_external_storage_devices_quantity} -gt 0 ]] &&
    mount_section=" [%F{yellow}${mounted_external_storage_devices_quantity}%f]"

  local -r storage_devices_symbol=$(
    [[ ${RIVER_DREAMS_USE_FALLBACK_TEXT} == true ]] &&
    echo "DEVICES" ||
    echo "󱊞"
  )

  echo "%F{yellow}${storage_devices_symbol} %f${#external_storage_devices[@]}${mount_section}%f"
}
