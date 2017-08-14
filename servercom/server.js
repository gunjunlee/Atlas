var app = require('http').createServer(handler)
  , io = require('socket.io').listen(app)
  , fs = require('fs');

app.listen(65432);

function handler (req, res) {
    fs.readFile(__dirname + '/index.html',
    function (err, data) {
        res.writeHead(200);
        res.end(data);
    });
}

io.sockets.on('connection', function (socket) {
    socket.emit('Server', 'server');
    socket.on('Client', function(data){
      console.log(data);
    });
    socket.on('mouseClick', function (data) {
          fs.writeFile('./mouse', data, function(err){
          if(err) throw err;
        });
    });
});


