#! /bin/bash

# install sdkman

if [ -z "$SDKMAN_VERSION" ]; then
	curl -s "https://get.sdkman.io" | bash
	"$HOME/.sdkman/bin/sdkman-init.sh"
else
	. $HOME/.sdkman/bin/sdkman-init.sh
       	sdk selfupdate
fi

# install java 19

if [ -z "$JAVA_HOME" ]; then
	echo $JAVA_HOME
else
	. $HOME/.sdkman/bin/sdkman-init.sh
        sdk install java 19.0.1-tem
fi	

# install gradle (7.6 required for java 19)

if [ -z "$GRADLE_HOME" ]; then
	. $HOME/.sdkman/bin/sdkman-init.sh
        sdk upgrade gradle
	echo $GRADLE_HOME
else
	. $HOME/.sdkman/bin/sdkman-init.sh
        sdk install gradle
fi



