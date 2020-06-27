
const Jimp = require('jimp');
const fs = require('fs');

const processImage = async (filename) => {
  const image = await Jimp.read(filename);

  const buffer = new Buffer(image.bitmap.width * image.bitmap.height * 2);

  image.scan(0, 0, image.bitmap.width, image.bitmap.height, function (x, y, idx) {
    const red = this.bitmap.data[idx + 0];
    const green = this.bitmap.data[idx + 1];
    const blue = this.bitmap.data[idx + 2];
    const alpha = this.bitmap.data[idx + 3];

    const r5 = ~~((red / 255.0) * 31) & 0b00011111;
    const g5 = ~~((green / 255.0) * 31) & 0b00011111;
    const b5 = ~~((blue / 255.0) * 31) & 0b00011111;
    const a5 = 0b00000001;

    const topHalf = 0xFF & (((r5 << 3) & (g5 >> 2)));
    const bottomHalf = 0xFF & ((g5 << 5) & (b5 << 1) & a5)

    buffer[((y * image.bitmap.height) + x) * 2] = topHalf;
    buffer[(((y * image.bitmap.height) + x) * 2) + 1] = bottomHalf;

    if (x == image.bitmap.width - 1 && y == image.bitmap.height - 1) {
      // image scan finished, do your stuff

      fs.writeFile(filename + '.bin', buffer, { encoding: 'binary', flag: 'w' })
    }
  });
};



const imageName = process.argv[process.argv.length - 1];

if (imageName.endsWith('.png') === false) {
  console.log('please check that the last argument ends in .png');
  return;
}

processImage(imageName);