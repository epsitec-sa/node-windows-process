/*global describe,it*/

var spawn = require("child_process").spawn;
var assert = require("assert");
var lib = require("../");

describe("IsProcessRunning", function () {
  it("should be running", function () {
    const child = spawn("explorer.exe");

    assert.strictEqual(lib.IsProcessRunning(child.pid, "explorer.exe"), true);

    child.stdin.pause();
    child.kill();
  });

  it("should not be running", function () {
    assert.strictEqual(lib.IsProcessRunning(1234, "explorer.exe"), false);
  });
});
