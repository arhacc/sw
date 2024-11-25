#!/bin/bash

set -e

curl -sS 'https://get.sdkman.io' | bash
source "${HOME}/.sdkman/bin/sdkman-init.sh"
sdk install java 23.0.1-oracle
