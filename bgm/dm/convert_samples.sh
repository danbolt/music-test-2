mkdir -p converted_samples
for i in original_samples/*.aiff; do
    o=converted_samples/${i#original_samples/}
    sox "$i" -r 32000 -c 1 "${o%}"
done
