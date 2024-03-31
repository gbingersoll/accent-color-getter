const AccentColorGetter = require('../lib/binding.js');
const assert = require('assert');

assert(AccentColorGetter, 'The expected module is undefined');

function testOnWindows() {
  const instance = new AccentColorGetter();
  const systemColors = instance.getSystem();
  [
    'Background',
    'Foreground',
    'AccentDark3',
    'AccentDark2',
    'AccentDark1',
    'Accent',
    'AccentLight1',
    'AccentLight2',
    'AccentLight3',
  ].forEach(name => {
    assert(Object.keys(systemColors).includes(name), `Missing color: ${name}`);
  });

  const color = systemColors['Accent'];
  assert(color.id == 5);
  ['R', 'G', 'B', 'A', 'hexRGB', 'hexRGBA', 'rgba'].forEach(key => {
    assert(Object.keys(color).includes(key), `Missing key in color: ${key}`);
  });

  ['R', 'G', 'B', 'A'].forEach(key => {
    assert(typeof color[key] == 'number', `Expected ${key} to be a number`);
  });

  ['hexRGB', 'hexRGBA', 'rgba'].forEach(key => {
    assert(typeof color[key] == 'string', `Expected ${key} to be a string`);
  });

  let hexString = '#';
  hexString += color.R.toString(16).padStart(2, '0').toUpperCase();
  hexString += color.G.toString(16).padStart(2, '0').toUpperCase();
  hexString += color.B.toString(16).padStart(2, '0').toUpperCase();
  assert(color.hexRGB.toUpperCase() == hexString, 'hexRGB is incorrect');
  hexString += color.A.toString(16).padStart(2, '0').toUpperCase();
  assert(color.hexRGBA.toUpperCase() == hexString, 'hexRGBA is incorrect');

  let rgbString = `rgba(${color.R},${color.G},${color.B},`;
  assert(
    color.rgba.replaceAll(' ', '').indexOf(rgbString) == 0,
    'rgba is incorrect'
  );
  if (color.A == 255) {
    assert(color.rgba.slice(-4) == '1.0)');
  } else {
    // Don't bother trying to compare floats
    assert(color.rgba.slice(-1) == ')');
  }
}

if (process.platform == 'win32') {
  assert.doesNotThrow(testBasic, undefined, 'testBasic threw an expection');
} else {
  const instance = new AccentColorGetter();
  assert.throws(instance.getSystem, undefined, 'testBasic threw an expection');
}

console.log('Tests passed- everything looks OK!');
