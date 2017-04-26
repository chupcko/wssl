var nicknameId = undefined;
var connectId = undefined;
var disconnectId = undefined;
var messageId = undefined;
var sendId = undefined;
var outputId = undefined;
var webSocket = undefined;

function init()
{
  document.getElementById('server').innerHTML = serverName+':'+serverPort;
  nicknameId = document.getElementById('nickname');
  connectId = document.getElementById('connect');
  disconnectId = document.getElementById('disconnect');
  messageId = document.getElementById('message');
  sendId = document.getElementById('send');
  outputId = document.getElementById('output');
  abled();
}

function abled()
{
  if(webSocket === undefined)
  {
    nicknameId.disabled = false;
    connectId.disabled = false;
    disconnectId.disabled = true;
    messageId.disabled = true;
    sendId.disabled = true;
  }
  else
  {
    nicknameId.disabled = true;
    connectId.disabled = true;
    disconnectId.disabled = false;
    messageId.disabled = false;
    sendId.disabled = false;
  }
}

function clean()
{
  outputId.innerHTML = '';
}

function write(message)
{
  var newData = document.createElement('p');
  newData.innerHTML = message;
  outputId.insertBefore(newData, outputId.firstChild);
}

function connect()
{
  if(webSocket !== undefined)
    write('ERROR: already connected');
  webSocket = new WebSocket('ws://'+serverName+':'+serverPort+'/'+nicknameId.value);
  webSocket.onopen = function(event)
  {
    write("Connected");
    abled();
  };
  webSocket.onclose = function(event)
  {
    write("Disconnected");
    webSocket = undefined;
    abled();
  };
  webSocket.onmessage = function(event)
  {
    write(event.data);
  };
  webSocket.onerror = function(event)
  {
    write('Error: '+event.type);
  };
}

function disconnect()
{
  if(webSocket === undefined)
    write('ERROR: not connected');
  webSocket.close();
}

function send()
{
  if(webSocket === undefined)
    write('ERROR: not connected');
  webSocket.send(messageId.value);
  messageId.value = '';
}
