CC=cc
CARGS="-Wall -Wextra -Werror -g -ggdb"

$CC $CARGS src/pop.c -o bin/popc
if [ $? -ne 0 ]
then
    echo "Compilation failed!"
    exit 1
fi

bin/popc bin/test.popb