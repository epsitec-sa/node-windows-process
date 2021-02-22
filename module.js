const addon = require("./build/Release/module");

function isProcessRunning(processPid, processName) {
  const res = addon.is_process_running(processPid, processName);
  return res === 1 ? true : false;
}

module.exports.IsProcessRunning = isProcessRunning;
