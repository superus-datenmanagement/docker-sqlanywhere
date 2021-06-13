# ***************************************************************************
# Copyright (c) 2019 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
toupper()
#########
{
    echo "$*" | tr a-z A-Z
}

tolower()
#########
{
    echo "$*" | tr A-Z a-z
}

not()
#####
{
    "$@"
    [ $? -ne 0 ]
}

check_tool_requirements()
#########################
{
    local sedtest awktest greptest trtest tailtest

    # make sure sed understands substituting hex codes
    sedtest=$(echo "o o" | sed 's/ /\x00/')
    if [ "$sedtest" = "ox00o" ]; then
	output_fatal_error "${ERR_SED}"
    fi
    # check existence of other required tools
    awktest=$(echo "awktest" | awk '{print $1}')
    if [ "$awktest" != "awktest" ]; then
	output_fatal_error "${ERR_AWK}"
    fi
    greptest=$(echo "greptest" | grep 'greptest')
    if [ "$greptest" != "greptest" ]; then
	output_fatal_error "${ERR_GREP}"
    fi
    trtest=$(echo "trtest" | tr a-z a-z)
    if [ "$trtest" != "trtest" ]; then
	output_fatal_error "${ERR_TR}"
    fi
}

output_msg()
###########
{
    ## [ ${QUIET:-0} -ne 0 ] && return ;

    eval echo "$*" >&2
}

output_fatal_error()
####################
{
    eval echo "$*" >&2
    exit 1
}

output_usage_error()
####################
{
    eval echo "$*" >&2
    usage
}

cui_echo()
##########
{
    ## [ ${QUIET:-0} -eq 0 ] && echo "$*"
    NOOP=NOOP
}

cui_eval_echo()
###############
{
    ## [ ${QUIET:-0} -ne 0 ] && return ;

    eval echo "$*"
}

cui_wait_for_input()
####################
# $2 - default response if the user hits enter in interactive mode
{
    if [ ${AUTOYES:-1} -eq 1 ] && [ -n "${2:-}" ] ; then
	CUI_RESPONSE="${2:-}"
    else
	cui_eval_echo "${1:-}"
	read CUI_RESPONSE
	[ -z "${CUI_RESPONSE:-}" ] && CUI_RESPONSE="${2:-}"
    fi
}

cui_ask_y_n()
#############
# $1 - prompt question
# $2 - default answer "Y" or "N" (influences key Y/n or N/y)
{
    if [ "${AUTOYES:-1}" -eq 1 ] ; then
	true 
	return 
    fi

    _DEFAULT_ANSWER="${2:-N}"

    if [ "${_DEFAULT_ANSWER}" = "Y" ] ; then
        _PROMPT="${MSG_PROMPT_YES_NO}"
    else
        _PROMPT="${MSG_PROMPT_NO_YES}"
    fi
    
    cui_echo ""
    cui_wait_for_input "${1} ${_PROMPT}" "${_DEFAULT_ANSWER}"

    case $CUI_RESPONSE in
        "${MSG_ANSWER_yes}" | "${MSG_ANSWER_Yes}" | "${MSG_ANSWER_y}" | "${MSG_ANSWER_Y}" )
	    true
            ;;
        * )
	    false
            ;;
    esac
}

exists()
########
{
    type $1 > /dev/null 2>&1
}

shell_quote()
#############
{
    printf %s\\n "$1" | sed "s/'/'\\\\''/g;1s/^/'/;\$s/\$/'/" ;
}

to_shell_string()
#################
{
    OUTPUT=""
    
    for i in "${@}"; do
        printf "%s "  $(shell_quote "$i")
    done
}

contains()
##########
{
    local e
    for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
    return 1
}

escape_char()
#############
{
    local is_path=0
    if [ "${1:-}" = "-p" ]; then
        is_path=1
        shift
    fi

    if [[ "$i" = [a-zA-Z0-9_:\-] ]] ; then
        printf '%s' "$i"
    elif [ "$i" = '/' ] ; then
        if [ "${is_path:-0}" = "1" ]; then
            printf '%s' "$i"
        else
            # systemd replaces / with -; which is ambiguous when unescaping
            # so this transformation is not reversible
            printf '-'
        fi
    else
        LC_CTYPE=C printf '\\x%02x' "'${i}"
    fi
}

escape_chars()
##############
{
    local OIFS="${IFS}"
    IFS=
    while read -r -n 1 i; do
        escape_char "$1" '$i'
    done
    IFS="${OIFS}"
}

escape()
########
{
    if exists systemd-escape; then
        systemd-escape "$1"
    else
        printf '%s' "$1" | escape_chars
    fi
}

escape_path()
#############
{
    printf '%s' "$1" | escape_chars -p
}

unescape()
##########
{
    for i in "$@"; do
        if exists systemd-escape; then
            systemd-escape -u "$i"
        else
            printf -- "${i}\n"
        fi
    done
}
