setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_RiverDreams_g_root=$(dirname ${0});

make -sC ${_RiverDreams_g_root};

PROMPT='$(${_RiverDreams_g_root}/build/bin/left-prompt ${ZVM_MODE})';
RPROMPT='$(${_RiverDreams_g_root}/build/bin/right-prompt)';
