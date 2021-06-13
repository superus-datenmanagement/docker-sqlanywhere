# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************

DEFAULT_INSTALL_PRODUCT=`get_default_install_product`
DEFAULT_INSTALL_PRODUCT=${DEFAULT_INSTALL_PRODUCT:=sqlanywhere}

read_version_field()
####################
{
    echo `dbversion -q -v "${2:-}" 2>/dev/null | grep "[[:space:]]${1:-}[[:space:]]" | cut -d ':' -f 2- 2>/dev/null`
}

find_product_tics()
###################
{
    product="${1:-${DEFAULT_INSTALL_PRODUCT}}"
    shift
    for tic in "$@" ; do
        tic=`eval echo ${tic}`
        if [ -n "${tic}" ] ; then
            tic_product=`read_version_field MODULE "${tic}"`
            if [ "${tic_product}" = "${product}" ] ; then
                echo ${tic}
                return
            fi
        fi
    done
}

get_version_field()
###################
{
    field="${1:-}"
    product="${2:-${DEFAULT_INSTALL_PRODUCT}}"

    fieldvarname="__VERSION_FIELD_${product}_${field}"
    fieldvar="\${${fieldvarname}:-}"
    if [ -z "`eval echo ${fieldvar}`" ]; then
        tic="`find_product_tic \"${product}\"`"
        value="`read_version_field \"${field}\" \"${tic:-}\"`"
        eval `echo ${fieldvarname}=\"${value}\"`
    fi

    echo `eval echo $fieldvar`
}


find_product_tic()
##################
{
    eval find_product_tics "${1:-${DEFAULT_INSTALL_PRODUCT}}" ${TICFILE:-}
}

get_major_version()
###################
{
    get_version_field VERSION_MAJOR "${1:-${DEFAULT_INSTALL_PRODUCT}}"
}


get_minor_version()
###################
{
    get_version_field VERSION_MINOR "${1:-${DEFAULT_INSTALL_PRODUCT}}"
}

get_patch_version()
###################
{
    get_version_field VERSION_PATCH "${1:-${DEFAULT_INSTALL_PRODUCT}}"
}

get_build_number()
##################
{
    get_version_field BUILD_NUMBER "${1:-${DEFAULT_INSTALL_PRODUCT}}"
}

get_version()
#############
{
    echo `get_major_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"``get_minor_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`
}

get_version_display()
#####################
{
    echo `get_major_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`.`get_minor_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`.`get_patch_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`
}

get_internal_dotted_version()
#############################
{
    echo `get_major_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`.`get_minor_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`.`get_patch_version "${1:-${DEFAULT_INSTALL_PRODUCT}}"`.`get_build_number "${1:-${DEFAULT_INSTALL_PRODUCT}}"`
}

get_intended_os()
#################
{
    echo `get_version_field IDENT "${1:-${DEFAULT_INSTALL_PRODUCT}}"` | cut -d '/' -f 5 | cut -d ' ' -f 1
}

get_intended_hw()
#################
{
    tic="`find_product_tic \"${1:-${DEFAULT_INSTALL_PRODUCT}}\"`"
    echo `get_version_field IDENT "${1:-${DEFAULT_INSTALL_PRODUCT}}"` | cut -d '/' -f 4 | tr '[:upper:]' '[:lower:]'
}
