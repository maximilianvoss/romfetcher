#
if [ ! -e ~/logs ]; then
    mkdir ~/logs
fi
if [ ! -e ~/.romfetcher ]; then
    mkdir ~/.romfetcher
fi
romfetcher >>~/logs/romfetcher.log 2>&1