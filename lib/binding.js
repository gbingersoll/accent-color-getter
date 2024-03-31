const addon = require('../build/Release/accent-color-getter-native.node');
const presets = require('./presets');

function AccentColorGetter() {
  this.getSystem = function () {
    return _addonInstance.get();
  };

  this.presets = presets;

  const _addonInstance = new addon.AccentColorGetter();
}

module.exports = AccentColorGetter;
