rm -r converted_samples
mkdir -p converted_samples
num=0
for i in original_samples/*.aiff; do
	echo $num
    o=converted_samples/00${num}_Percussive_${i#original_samples/}
    sox "$i" -b 16 -r 32000 -c 1 "${o%}"
    let "num += 1"
done
