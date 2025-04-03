DEFINE=$(cat PalcomWorking/defines.h | grep SYS_VERSION)
DEFINE=$(echo -en $DEFINE | sed 's/#define SYS_VERSION "//g')
VERSION=$(echo -en $DEFINE | sed 's/"//g')
sed -i "s/<code>version .*<\/code>/<code> version $VERSION<\/code>/g" README.md
