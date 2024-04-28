setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_root_g=$(dirname ${0});

[[ ! -f ${_riverDreams_root_g}/build/bin/left-prompt ||
   ! -f ${_riverDreams_root_g}/build/bin/right-prompt ]] && make -sC ${_riverDreams_root_g};

PROMPT='$(${_riverDreams_root_g}/build/bin/left-prompt ${ZVM_MODE})';
RPROMPT='$(${_riverDreams_root_g}/build/bin/right-prompt)';
