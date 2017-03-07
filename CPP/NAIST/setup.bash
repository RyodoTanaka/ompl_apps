#!/bin/bash

# This Script's Path
SCRIPT_PATH=$( dirname ${BASH_SOURCE:-$0} )

# ENV PATH
export OMPL_APPS_DIR=${SCRIPT_PATH}
export OMPL_APPS_EXE_FILES=$(ls ${SCRIPT_PATH}/bin)

# List of the Execution files
EXE_FILES=$(ls ${SCRIPT_PATH}/bin)

function _func_complete_omplrun()
{
    local cur=${COMP_WORDS[COMP_CWORD]}
    if [ "$COMP_CWORD" -eq 1 ]; then
        COMPREPLY=( $(compgen -W $OMPL_APPS_EXE_FILES -- $cur) )
fi
}

function func_ompl()
{
    core=$(find ${SCRIPT_PATH} -name $1)
    option=$(echo $@ | sed -e "s/$1//g")
    command=$core$option
    $command
}

alias omplrun=func_ompl
complete -o default -F _func_complete_omplrun omplrun
