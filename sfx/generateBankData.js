
/*
 * Script for generating an insturment bank for the N64 sound tools.

 * Before you run this script:
 * 1) create a folder of aiff files called `original_samples`
 * 2) run `convert_samples.sh`
 * 3) run `compress_samples.sh`
 */

const fs = require('fs');


const SAMPLES_DIR = 'compressed_samples';

// N64 tools don't have good unicode support
const ENCODING = 'ascii';

const DEFAULT_ENVELOPE_NAME = 'defaultenv';
const DEFAULT_ENVELOPE = `envelope defaultenv
{
    attackTime    = 0;
    attackVolume  = 127;
    decayTime   = 500000;
    decayVolume   = 100;
    releaseTime   = 200000;
    
}

`;

const DEFAULT_KEYMAP_DATA = `keymap REPLACENAMEKeymap
{
    velocityMin = 0;
    velocityMax = 127;
    keyMin      = 0;
    keyMax      = 127;
    keyBase     = 60;
    detune      = 0;
}
`;

const DEFAULT_KEYMAP_DATA_SPECIFIC_KEY = `keymap REPLACENAMEKeymap
{
    velocityMin = 0;
    velocityMax = 127;
    keyMin      = KMIN;
    keyMax      = KMAX;
    keyBase     = KBASE;
    detune      = 0;
}
`;


const generateKeyMapData = (name, specificKey) => {
  if (specificKey !== undefined) {
    return DEFAULT_KEYMAP_DATA_SPECIFIC_KEY.replace('REPLACENAME', name).replace('KMIN', specificKey).replace('KMAX', specificKey).replace('KBASE', specificKey);
  }

  return DEFAULT_KEYMAP_DATA.replace('REPLACENAME', name);
}

const DEFUALT_SOUND_DATA = `sound SOUND_NAME
{
    use ("compressed_samples/FILE_NAME");
    
    pan    = 64;
    volume = 127;
    keymap = KEYMAP_NAMEKeymap;
    envelope = defaultenv;
}
`;
const generateSoundData = (name, fileName) => {  return DEFUALT_SOUND_DATA.replace('SOUND_NAME', (name + 'Sound')).replace('FILE_NAME', fileName).replace('KEYMAP_NAME', name); };

const DEFAULT_INSTURMENT_DATA = `instrument INSTURMENT_NAME
{
    volume = 127;
    pan    = 64;

    sound  = SOUND_NAME;
}
`;
const generateInsturmentData = (name) => { return DEFAULT_INSTURMENT_DATA.replace('INSTURMENT_NAME', name).replace('SOUND_NAME', (name + 'Sound')); };

const sampleFilenames = fs.readdirSync(SAMPLES_DIR, ENCODING);

const melodicNumberMapping = {};
const percussiveNumberMapping = {};
const sampleData = sampleFilenames.map((fileName) => {
  const result = {};

  result.midiInsturmentNumber = parseInt(fileName.split('_', 1)[0]);
  result.fileName = fileName;
  result.insturmentName = fileName.split('.')[0].replace(/\d*_/, '').replace(/\(/, '').replace(/\)/, '');
  result.melodic = (result.insturmentName.split('_')[0] === 'Melodic');

  if (result.melodic) {
    melodicNumberMapping[result.midiInsturmentNumber] = result;
  } else {
    percussiveNumberMapping[result.midiInsturmentNumber] = result;
  }

  return result;
});

console.log(DEFAULT_ENVELOPE);

const STANDARD_KIT_BEGIN = `instrument Percussion_Kit
{
    volume = 127;
    pan    = 64;
`
const STANDARD_KIT_END = `}
`;

sampleData.forEach((data) => {  
  console.log(generateKeyMapData(data.insturmentName, data.melodic ? undefined : (data.midiInsturmentNumber + 10)));
  console.log(generateSoundData(data.insturmentName, data.fileName));

  if (data.melodic) {
    console.log(generateInsturmentData(data.insturmentName));
  }
});

const LOWEST_PERCUSSION_NUMBER = 0;
const HIGHEST_PERCUSSION_NUMBER = 127;
console.log(STANDARD_KIT_BEGIN);

for (let i = LOWEST_PERCUSSION_NUMBER; i <= HIGHEST_PERCUSSION_NUMBER; i++) {
  if (percussiveNumberMapping[i] == undefined) {
    continue;
  }

  console.log('    sound = ' + percussiveNumberMapping[i].insturmentName + 'Sound;');
}

console.log(STANDARD_KIT_END);

console.log('bank SongBank ');
console.log('{');
console.log('    sampleRate = 32000;');
console.log('    instrument [0] = Percussion_Kit;');
console.log('');

const HIGHEST_MIDI_INSTURMENT_NUMBER = 127;
for (let i = 0; i <= HIGHEST_MIDI_INSTURMENT_NUMBER; i++) {
  if (!(melodicNumberMapping[i])) {
    continue;
  }

  console.log('    instrument [' + (i - 1) + '] = ' + melodicNumberMapping[i].insturmentName + ';');
}

console.log('}\n');


