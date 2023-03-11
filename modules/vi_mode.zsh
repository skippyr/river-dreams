river_dreams::vi_mode() {
  [[ -z ${ZVM_MODE} ]] && exit
  local -r vi_mode=$(echo ${ZVM_MODE} | tr [:lower:] [:upper:])
  local vi_mode_color="%F{red}"
  case ${vi_mode} in
    I)
      vi_mode_color="%F{green}"
      ;;
    V | VL)
      vi_mode_color="%F{cyan}"
      ;;
    R)
      vi_mode_color="%F{magenta}"
      ;;
    *)
      vi_mode_color="%F{red}"
      ;;
  esac
  echo "%F{yellow}[%B${vi_mode_color}${vi_mode}%b%F{yellow}]%f"
}
