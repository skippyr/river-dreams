export VIRTUAL_ENV_DISABLE_PROMPT=1;
_riverDreams_g_root=$(dirname ${0});

setopt promptsubst;
PROMPT='$(${_riverDreams_g_root}/build/bin/left-prompt)';
RPROMPT='$(${_riverDreams_g_root}/build/bin/right-prompt)';

[[ ! -f ${_riverDreams_g_root}/build/bin/left-prompt || ! -f ${_riverDreams_g_root}/build/bin/right-prompt ]] &&
	make -sC ${_riverDreams_g_root};
