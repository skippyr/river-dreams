river_dreams::vi_mode() {
  if [[ -n ${ZVM_MODE} ]]; then
    local -r vi_mode=$(echo ${ZVM_MODE} | tr [:lower:] [:upper:])
    local vi_mode_symbol="%F{red}?"
    case ${vi_mode} in
      I)
        vi_mode_symbol="%F{green}I"
        ;;
      N)
        vi_mode_symbol="%F{red}N"
        ;;
      V | VL)
        vi_mode_symbol="%F{cyan}V"
        ;;
      R)
        vi_mode_symbol="%F{magenta}R"
        ;;
    esac
    echo "%F{yellow}[%B${vi_mode_symbol}%b%F{yellow}]%f"
  fi
}
