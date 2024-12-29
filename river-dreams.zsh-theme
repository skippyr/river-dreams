setopt promptsubst;

export VIRTUAL_ENV_DISABLE_PROMPT=1;
export RD_ROOT=$(dirname ${0});
export PATH="${PATH}:${RD_ROOT}/target/release";

if [[ ! -x ${RD_ROOT}/target/release/river-dreams ]]; then
  cargo b --release --manifest-path ${RD_ROOT}/Cargo.toml;
fi

if [[ -x $(which river-dreams) ]]; then
  PROMPT='$(river-dreams l)';
  RPROMPT='$(river-dreams r)';
fi
