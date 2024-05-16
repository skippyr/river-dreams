setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_g_root=$(dirname ${0});

make -sC ${_riverDreams_g_root};

PROMPT='$(${_riverDreams_g_root}/build/bin/left-prompt ${ZVM_MODE})';
RPROMPT='$(${_riverDreams_g_root}/build/bin/right-prompt)';
