setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_root_g=$(dirname $(dirname $(dirname ${0})));

PROMPT='$(${_riverDreams_root_g}/build/bin/left-prompt)';
RPROMPT='$(${_riverDreams_root_g}/build/bin/right-prompt)';
