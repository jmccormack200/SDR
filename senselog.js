var http = require('http').Server(null);
var io = require('socket.io').Server(http);
var MongoClient = require('mongodb').MongoClient;
var config = require('./config/config.json');

var server = function() {
    io.on('connection', function(socket) {

        console.log('-> client connected');

        socket.on('feedback', function(value) {
            if (value) {
                console.log('<- pressed');
                socket.emit('sensors');
            } else {
                console.log('<- released');
            }
        });

        socket.on('sensor-data', function(data) {
            console.log(data);
        });

        socket.on('node-data', function(data) {
            console.log(data);
        });

        socket.on('all-data', function(data) {
            console.log('<- recieved data from ' + socket.id);
            MongoClient.connect(config.mongourl, function(err, db) {
                var collection = db.collection(config.collection);
                collection.insert(data);
                db.close();
            });
        });

    });

    http.listen(3000, function() {
        console.log('ready');
    });
}

module.export = server;