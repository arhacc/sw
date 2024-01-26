#!/bin/bash

# Set shell options

set -e
set -o pipefail

# Embedded files

sdk_conf_file='last.project.directory = $ARHACC_PATH/libraries/app_level/prj5/prj5.xpuprj
preferences.target=0,connecting,simulation2,remote,athena.arh.pub.ro,49000,xpu3200016,selected
preferences.target=1,connecting,pinq1,remote,zeus.arh.pub.ro,49000,xpu3200016
preferences.target=2,connecting,simulation1,remote,localhost,49000,xpu3200016
preferences.target=3,connecting,pynq2,remote,zeus.arh.pub.ro,49001,xpu3200016
gui.size.x = 1280
gui.size.y = 799
gui.splitPane1 = 0.9305354558610709
gui.splitPane2 = 0.17688679245283018
gui.splitPane3 = 0.3763779527559055
gui.splitPane5 = 0.547550432276657
lf = Nimbus
editor_theme = Dark
debug = 1
debug.magnifier.startIndex = 0
debug.magnifier.stopIndex = 6
debug.magnifier.jSplitPane1 = 0.1581858407079646
debug.window.state = false
debug.window.expanded.location.x = 0
debug.window.expanded.location.y = 33
debug.window.expanded.dimension.x = 493.0
debug.window.expanded.dimension.y = 771.0
gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled = true
gui.menu.file.preferences.general.automaticallyCheckForUpdates.interval = Every day
gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled = true
git.local.repo = $ARHACC_PATH
profile = AppLevel
librariesPath=$ARHACC_PATH/libraries/
last_directory = $ARHACC_PATH/libraries/app_level/Example6/Example6.asm
last.project.location = $ARHACC_PATH/libraries/app_level/Example6
selectedProfileLevel = AppLevel
pathToActiveProject=$ARHACC_PATH/libraries/app_level/Example0/Example0.xpuprj
github_token=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
remember_target_connection_for_each_project = false'

profile_d_xpu_sh_file='export PATH="${PATH}:/opt/xpu/bin"'

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

function write-profile-d-file() {
    case "$(uname -o)" in
        "GNU/Linux")
            if [[ ! -f /etc/profile.d/xpu.sh ]]
            then
                cat <<< "${profile_d_xpu_sh_file}" >/etc/profile.d/xpu.sh


                echo "Wrote /etc/profile.d/xpu.sh"
                echo "You may wish to restart your system to ensure the changes take effect."
            fi

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
    if [[ -x /opt/xpu/bin/xrt && `/opt/xpu/bin/xrt -version` = ${LATEST_RELEASE_TAG} ]]
    then
        echo "xrt ${LATEST_RELEASE_TAG} is already installed"
        return
    fi

    mkdir -p "/opt/xpu/bin"
    cd "/opt/xpu/bin"    

    local TRIPLE=unset
    local PRINTABLE_TRIPLE=unset

    case "$(uname -o) $(uname -m)" in
        "GNU/Linux x86_64")
            local TRIPLE=x86_64-linux-gnu
            local PRINTABLE_TRIPLE="GNU/Linux x86_64"
            ;;
        "Darwin "*)
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
#     if [[ `sudo dpkg-query -l | awk '$2 == "xpu" { print $3; }' | awk 'BEGIN { FS="-"; } {print $
# 1;}'` -eq ${LATEST_RELEASE_NUMBER} ]]
#     then
#         echo "sdk ${LATEST_RELEASE_TAG} is already installed"
#         return
#     fi

    local FILE_ENDING=unset
    local PRINTABLE_TRIPLE=unset

    case "$(uname -o) $(uname -m)" in
        "GNU/Linux x86_64")
            local FILE_ENDING=_amd64.deb
            local PRINTABLE_TRIPLE="GNU/Linux x86_64"
            ;;
        "Darwin "*)
            local FILE_ENDING=.dmg
            local PRINTABLE_TRIPLE="Mac OS X"
            ;;
    esac

    if [[ "${FILE_ENDING}" == unset ]]
    then
        echo Unsupported or unrecognized operating system or machine.
        exit 4
    fi

    local XPU_SDK_PACKAGE="xpu_${LATEST_RELEASE_NUMBER}${FILE_ENDING}"
    local XPU_SDK_PACKAGE_CHECKSUM="${XPU_SDK_PACKAGE}.sha256sum"

    echo "Downloading xpu ${LATEST_RELEASE_TAG} for ${PRINTABLE_TRIPLE}"

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
        "Darwin")
            hdiutil attach "${XPU_SDK_PACKAGE}"
            cp -rf /Volumes/xpu.app /Applications
            hdiutil unmount /Volumes/xpu.app
            ;;
    esac

    rm "${XPU_SDK_PACKAGE}" "${XPU_SDK_PACKAGE_CHECKSUM}"

    cd "${XPU_HOME}/bin"
}

function check-create-xpu-home() {
    if [[ -z "${XPU_HOME}" ]]
    then
        export XPU_HOME="${HOME}/.xpu"
        echo "!!! IMPORTANT !!!"
        echo "Add the following to your .bashrc"
        echo
        printf "\texport XPU_HOME=\"\${HOME}/.xpu\""
        printf "\texport PATH=\"\${PATH}:\${XPU_HOME}/bin\""
        echo
    fi

    {
        mkdir -p "${XPU_HOME}/bin" &&
        mkdir -p "${XPU_HOME}/tmp/cache" &&
        mkdir -p "${XPU_HOME}/etc" &&
        mkdir -p "${XPU_HOME}/logs" &&

	if [[ ! -f "${XPU_HOME}/etc/sdk.conf" ]]
	then
		cat <<< "$sdk_conf_file" >"${XPU_HOME}/etc/sdk.conf"
	fi
    } || { 
        echo Failed to create XPU_HOME directory >&2 ; \
        exit 3 ;
    }
}

function check-sudo() {
    if [[ ! `id -u` == 0 ]]
    then
        echo This script must be run as sudo. Exiting. >&2
        exit 1
    fi
}

check-exists-command git gh jq shasum

check-sudo

get-latest-release

install-xrt

install-sdk

write-profile-d-file