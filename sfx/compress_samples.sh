rm -r compressed_samples
rm -r table_data
mkdir -p compressed_samples
mkdir -p table_data
for i in converted_samples/*.aiff; do
	SPOT=table_data/${i#converted_samples/}
    ~/Projects/sdk-tools/tabledesign/tabledesign_native ${i} > "${SPOT/\.aiff/}.table"
done

for i in converted_samples/*.aiff; do
	SPOT=table_data/${i#converted_samples/}
	SPOT2=compressed_samples/${i#converted_samples/}
    ~/Projects/sdk-tools/adpcm/vadpcm_enc_native -c "${SPOT/\.aiff/}.table" $i "${SPOT2/\.aiff/}.aifc"
done
