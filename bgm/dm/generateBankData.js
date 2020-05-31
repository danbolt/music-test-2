
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
const generateKeyMapData = (name) => { return DEFAULT_KEYMAP_DATA.replace('REPLACENAME', name); }

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

const midiNumberMapping = {};
const sampleData = sampleFilenames.map((fileName) => {
  const result = {};

  result.midiInsturmentNumber = parseInt(fileName.split('_', 1)[0]);
  result.fileName = fileName;
  result.insturmentName = fileName.split('.')[0].replace(/\d*_/, '').replace(/\(/, '').replace(/\)/, '');
  result.melodic = (result.insturmentName.split('_')[0] === 'Melodic');

  if (result.melodic) {
    midiNumberMapping[result.midiInsturmentNumber] = result;
  }

  return result;
});

console.log(DEFAULT_ENVELOPE);

sampleData.forEach((data) => {
  if (!(data.melodic)) {
    return;
  }
  
  console.log(generateKeyMapData(data.insturmentName));
  console.log(generateSoundData(data.insturmentName, data.fileName));
  console.log(generateInsturmentData(data.insturmentName));
});

console.log('bank SongBank ');
console.log('{');
console.log('    sampleRate = 32000;');
console.log('');

const HIGHEST_MIDI_INSTURMENT_NUMBER = 127;
for (let i = 0; i <= HIGHEST_MIDI_INSTURMENT_NUMBER; i++) {
  if (!(midiNumberMapping[i])) {
    continue;
  }

  console.log('    instrument [' + (i - 1) + '] = ' + midiNumberMapping[i].insturmentName + ';');
}

console.log('}\n');


