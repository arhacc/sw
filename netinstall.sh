#!/bin/bash

# Set shell options

set -e
set -o pipefail

function user-gh() {
    sudo -u "${SUDO_USER}" gh "$@"
}

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

check-exists-command git gh jq shasum

function write-profile-d-file() {
    case "$(uname -o)" in
        "GNU/Linux")
            cat <<EOF >/etc/profile.d/xpu.sh
export PATH="${PATH}:/opt/xpu-sdk/bin"
EOF

            echo "Wrote /etc/profile.d/xpu.sh."
            echo "You may wish to restart your system to ensure the changes take effect."
            ;;
        "Darwin")
            ;;
    esac
}

function get-latest-release() {
    LATEST_RELEASE_TAG="$( \
        {
            user-gh api \
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
            user-gh api \
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

    LATEST_RELEASE_NUMBER="${LATEST_RELEASE_TAG#v}"

    echo "Latest release is ${LATEST_RELEASE_TAG} ${LATEST_RELEASE_ID}"
}

function get-asset-from-latest-release() {
    local ASSET_ID="$(
        {
            user-gh api \
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

    user-gh api \
        -H "Accept: application/octet-stream" \
        -H "X-GitHub-Api-Version: 2022-11-28" \
        "/repos/${REPO}/releases/assets/${ASSET_ID}" >"${1}" \
        || {
            echo Failed to get release asset "${1}" >&2 ; \
            exit 3 ;
        }
}

function install-xrt() {
    mkdir -p "/opt/xpu-sdk/bin"
    cd "/opt/xpu-sdk/bin"

    local TRIPLE=unset
    local PRINTABLE_TRIPLE=unset

    case "$(uname -o) $(uname -m)" in
        "GNU/Linux x86_64")
            local TRIPLE=x86_64-linux-gnu
            local PRINTABLE_TRIPLE="GNU/Linux x86_64"
            ;;
        "Darwin *")
            echo xrt is currently not cross-compiled to macOS. Skipping.
            return
            ;;
    esac

    if [[ "${TRIPLE}" == unset ]]
    then
        echo Unsupported or unrecognized operating system or machine.
        exit 4
    fi

    local XRT_TARBALL="xrt-${LATEST_RELEASE_TAG}-${TRIPLE}.tar.gz"
    local XRT_TARBALL_CHECKSUM="xrt-${LATEST_RELEASE_TAG}-${TRIPLE}.tar.gz.sha256sum"

    echo "Downloading xrt ${LATEST_RELEASE_TAG} for ${PRINTABLE_TRIPLE}"

    get-asset-from-latest-release "${XRT_TARBALL}"
    get-asset-from-latest-release "${XRT_TARBALL_CHECKSUM}"

    echo "Checking download integrity"

    if ! shasum -c -a 256 "${XRT_TARBALL_CHECKSUM}"
    then
        echo "Checksum not valid" >&2
        exit 5
    fi

    tar xfz "${XRT_TARBALL}" &&

    rm "${XRT_TARBALL}" "${XRT_TARBALL_CHECKSUM}"
}

function install-sdk() {
    local FILE_ENDING=unset
    local PRINTABLE_TRIPLE=unset

    case "$(uname -o) $(uname -m)" in
        "GNU/Linux x86_64")
            local FILE_ENDING=_amd64.deb
            local PRINTABLE_TRIPLE="GNU/Linux x86_64"
            ;;
        "Darwin *")
            local FILE_ENDING=.dmg
            local PRINTABLE_TRIPLE="Mac OS X"
            ;;
    esac

    if [[ "${FILE_ENDING}" == unset ]]
    then
        echo Unsupported or unrecognized operating system or machine.
        exit 4
    fi

    local XPU_SDK_PACKAGE="xpu-sdk_${LATEST_RELEASE_NUMBER}${FILE_ENDING}"
    local XPU_SDK_PACKAGE_CHECKSUM="${XPU_SDK_PACKAGE}.sha256sum"

    echo "Downloading xpu-sdk ${LATEST_RELEASE_TAG} for ${PRINTABLE_TRIPLE}"

    get-asset-from-latest-release "${XPU_SDK_PACKAGE}"
    get-asset-from-latest-release "${XPU_SDK_PACKAGE_CHECKSUM}"

    echo "Checking download integrity"

    if ! shasum -c -a 256 "${XPU_SDK_PACKAGE_CHECKSUM}"
    then
        echo "Checksum not valid" >&2
        exit 5
    fi

    case "$(uname -o)" in
        "GNU/Linux")
            dpkg --install "${XPU_SDK_PACKAGE}"
            ;;
        "Darwin *")
            hdiutil attach "${XPU_SDK_PACKAGE}"
            cp -rf /Volumes/xpu-sdk.app /Applications
            hdiutil unmount /Volumes/xpu-sdk.app
            ;;
    esac

    rm "${XPU_SDK_PACKAGE}" "${XPU_SDK_PACKAGE_CHECKSUM}"

    cd "${XPU_HOME}/bin"
}

# function check-create-xpu-home() {
#     if [[ -z "${XPU_HOME}" ]]
#     then
#         export XPU_HOME="${HOME}/.xpu"
#         echo "!!! IMPORTANT !!!"
#         echo "Add the following to your .bashrc"
#         echo
#         printf "\texport XPU_HOME=\"\${HOME}/.xpu\""
#         printf "\texport PATH=\"\${PATH}:\${XPU_HOME}/bin\""
#         echo
#     fi

#     {
#         mkdir -p "${XPU_HOME}/bin" &&
#         mkdir -p "${XPU_HOME}/tmp/cache" &&
#         mkdir -p "${XPU_HOME}/etc" &&
#         mkdir -p "${XPU_HOME}/logs"
#     } || { 
#         echo Failed to create XPU_HOME directory >&2 ; \
#         exit 3 ;
#     }
# }

function check-sudo() {
    if [[ ! `id -u` == 0 ]]
    then
        echo This script must be run as sudo. Exiting. >&2
        exit 1
    fi
}

check-sudo

get-latest-release

install-sdk

write-profile-d-file