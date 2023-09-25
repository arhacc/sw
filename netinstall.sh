#!/bin/bash

# Set shell options

set -o pipefail

# Constants

REPO=arhacc/sw

function p() {
	if [ -t 1 ]
	then
		printf '\n\033[35m%s\033[0m\n\n' "$@"
	else
		printf '%s\n' "$@"
	fi
}

function check-exists-command() {
    for program in "$@"
    do
        if ! command -v "${program}" &>/dev/null
        then
            printf 'Program %s is not installed. Aborting.' "${program}" >&2
            exit 2
        fi
    done
}

check-exists-command gh jq sha256sum

function get-latest-release() {
    LATEST_RELEASE_TAG="$( \
        {
            gh api \
                -H "Accept: application/vnd.github+json" \
                -H "X-GitHub-Api-Version: 2022-11-28" \
                "/repos/${REPO}/releases" \
            | jq -r '.[].tag_name' 2>/dev/null \
            | sort -V \
            | tail -1 ;
        } ||
            {
                echo Failed to get repo releases >&2 ; \
                exit 3 ; \
            }
    )"

    if [[ -z "${LATEST_RELEASE_TAG}" ]]
    then
        echo Repo "${REPO}" unexpectedly has no releases >&2
        exit 3
    fi

    LATEST_RELEASE_ID="$(
        {
            gh api \
                -H "Accept: application/vnd.github+json" \
                -H "X-GitHub-Api-Version: 2022-11-28" \
                "/repos/${REPO}/releases" \
            | jq -r ".[] | select( .tag_name == \"${LATEST_RELEASE_TAG}\" ).id" \
            ;
        } ||
            { 
                echo Failed to get release "${LATEST_RELEASE_TAG}" id >&2 ; \
                exit 3 ;
            }
    )"

    echo "Latest release is ${LATEST_RELEASE_TAG} ${LATEST_RELEASE_ID}"
}

function get-asset-from-latest-release() {
    local ASSET_ID="$(
        {
            gh api \
                -H "Accept: application/vnd.github+json" \
                -H "X-GitHub-Api-Version: 2022-11-28" \
                "/repos/${REPO}/releases/${LATEST_RELEASE_ID}/assets" \
            | jq -r ".[] | select( .name == \"${1}\" ).id" \
            ;
        } ||
            {
                echo Failed to get release asset "${1}" id >&2 ; \
                exit 3 ;
            }
    )"

    gh api \
        -H "Accept: application/octet-stream" \
        -H "X-GitHub-Api-Version: 2022-11-28" \
        "/repos/${REPO}/releases/assets/${ASSET_ID}" >"${1}" \
        || {
            echo Failed to get release asset "${1}" >&2 ; \
            exit 3 ;
        }
}

function install-xrt() {
    cd "${XPU_HOME}/bin"

    local TRIPLE=unset
    local PRINTABLE_TRIPLE=unset

    case "$(uname -o) $(uname -m)" in
        "GNU/Linux x86_64")
            local TRIPLE=x86_64-linux-gnu
            local PRINTABLE_TRIPLE="GNU/Linux x86_64"
            ;;
    esac

    if [[ "${TRIPLE}" == unset ]]
    then
        echo Unsupported or unrecognized operating system or machine.
        exit 4
    fi

    get-latest-release

    local XRT_TARBALL="xrt-${LATEST_RELEASE_TAG}-${TRIPLE}.tar.gz"
    local XRT_TARBALL_CHECKSUM="xrt-${LATEST_RELEASE_TAG}-${TRIPLE}.tar.gz.sha256sum"

    echo "Downloading xrt ${LATEST_RELEASE_TAG} for ${PRINTABLE_TRIPLE}"

    get-asset-from-latest-release "${XRT_TARBALL}"
    get-asset-from-latest-release "${XRT_TARBALL_CHECKSUM}"

    echo "Checking download integrity"

    if ! sha256sum -c "${XRT_TARBALL_CHECKSUM}"
    then
        echo "Checksum not valid" >&2
        exit 5
    fi

    tar xfz "${XRT_TARBALL}" &&

    rm "${XRT_TARBALL}" "${XRT_TARBALL_CHECKSUM}"
}

function check-create-xpu-home() {
    if [[ -z "${XPU_HOME}" ]]
    then
        export XPU_HOME="${HOME}/.xpu"
        echo "!!! IMPORTANT !!!"
        echo "Add the following to your .bashrc"
        echo
        printf "\texport XPU_HOME=\"\${HOME}\"/.xpu"
        echo
    fi

    mkdir -p "${XPU_HOME}/bin" || { 
        echo Failed to create XPU_HOME directory >&2 ; \
        exit 3 ;
    }
}

check-create-xpu-home

install-xrt