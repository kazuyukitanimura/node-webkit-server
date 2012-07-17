var ChildProcess = require("child_process"),
    Events       = require("events"),
    Node         = require("./node"),
    Parser       = require("./parser"),
    Path         = require("path"),
    Socket       = require("net").Socket,
    Util         = require("util");

var WEBKIT_SERVER_BINARY = Path.join(
  __dirname, "..", "vendor", "webkit-server", "bin", "webkit_server"
);

var Browser = function(callback) {
  this.callbacks = [];

  this.parser = new Parser();
  this.parser.on("response", function(error, response) {
    this.callbacks.shift().call(this, error, response);
  }.bind(this));

  this.start(function(port) {
    this.connect(port, callback.bind(this));
  }.bind(this));

  process.on("exit", this.stop.bind(this));
};

Util.inherits(Browser, Events.EventEmitter);

Browser.prototype.body = function(callback) {
  this.command("Body", callback);
};

Browser.prototype.evaluate = function(code, callback) {
  this.command("Evaluate", code, callback);
};

Browser.prototype.execute = function(code, callback) {
  this.command("Execute", code, callback);
};

Browser.prototype.find = function(selector, callback) {
  this.command("Find", selector, function(error, ids) {
    if (error) {
      return callback.call(this, error);
    } else if (ids.length > 0) {
      var nodes = ids.split(',').map(function(id) {
        return new Node(id, this);
      }.bind(this));
    }

    callback.call(this, undefined, nodes || []);
  });
};

Browser.prototype.headers = function(callback) {
  this.command("Headers", callback);
};

Browser.prototype.render = function(path, width, height, callback) {
  this.command("Render", path, width, height, callback);
};

Browser.prototype.reset = function(callback) {
  this.command("Reset", callback);
};

Browser.prototype.source = function(callback) {
  this.command("Source", callback);
};

Browser.prototype.status = function(callback) {
  this.command("Status", callback);
};

Browser.prototype.url = function(callback) {
  this.command("Url", callback);
};

Browser.prototype.visit = function(path, callback) {
  this.command("Visit", path, callback);
};

Browser.prototype.command = function() {
  var options  = Array.prototype.slice.call(arguments),
      name     = options.shift(),
      callback = options.pop(),
      length   = options.length,
      commands = [name + "\n"];

  commands.push(length + "\n");

  for (var index = 0; index < length; index++) {
    var option = options[index].toString();

    commands.push(Buffer.byteLength(option) + "\n" + option);
  }

  this.callbacks.push(callback);
  this.socket.write(commands.join(""), "utf8");
};

Browser.prototype.connect = function(port, callback) {
  this.socket = new Socket();
  this.socket.on("connect", callback);
  this.socket.on("data", function(data) {
    this.parser.execute(data);
  }.bind(this));
  this.socket.on("error", function(error) {
    throw error;
  }.bind(this));
  this.socket.connect(port);
};

Browser.prototype.start = function(callback) {
  this.server = ChildProcess.spawn(WEBKIT_SERVER_BINARY);
  this.server.stdout.once("data", function(data) {
    var matches = data.toString().match(/port: (\d+)/);

    if (matches) {
      callback(parseInt(matches[1], 10));
    }
  });
  this.server.stderr.pipe(process.stderr);
  this.server.on("exit", function(code) {
    this.emit("exit", code);
  }.bind(this));
};

Browser.prototype.stop = function(error) {
  this.socket && this.socket.end();
  this.server && this.server.kill("SIGHUP");
};

module.exports = Browser;
