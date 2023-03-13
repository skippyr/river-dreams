river_dreams::vi_mode() {
  [[ -z ${ZVM_MODE} ]] && exit
  local vi_mode="?"
  local vi_mode_color="%F{red}"
  case ${ZVM_MODE} in
    i)
      vi_mode="I"
      vi_mode_color="%F{green}"
      ;;
    v | vl)
      vi_mode="V"
      vi_mode_color="%F{cyan}"
      ;;
    r)
      vi_mode="R"
      vi_mode_color="%F{magenta}"
      ;;
    n)
      vi_mode="N"
      ;;
  esac
  echo "%F{yellow}[%B${vi_mode_color}${vi_mode}%b%F{yellow}]%f"
}
