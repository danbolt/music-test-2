for i in ~/Downloads/sample/*.aiff; do
    o=./${i#~/Downloads/sample/}
    sox "$i" -r 32000 -c 1 "${o%}"
done
