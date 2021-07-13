/*global describe,it*/

var spawn = require("child_process").spawn;
var assert = require("assert");
var lib = require("../");

describe("isProcessRunning", function () {
  it("should be running", function () {
    const child = spawn("explorer.exe");

    assert.strictEqual(lib.isProcessRunning(child.pid, "explorer.exe"), true);

    child.stdin.pause();
    child.kill();
  });

  it("should not be running", function () {
    assert.strictEqual(lib.isProcessRunning(1234, "explorer.exe"), false);
  });
});
