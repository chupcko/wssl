var connectId = undefined;
var disconnectId = undefined;
var outputId = undefined;
var webSocket = undefined;

function init()
{
  document.getElementById('server').innerHTML = serverName+':'+serverPort;
  connectId = document.getElementById('connect');
  disconnectId = document.getElementById('disconnect');
  outputId = document.getElementById('output');
  abled();
}

function abled()
{
  if(webSocket === undefined)
  {
    connectId.disabled = false;
    disconnectId.disabled = true;
  }
  else
  {
    connectId.disabled = true;
    disconnectId.disabled = false;
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
  webSocket = new WebSocket('ws://'+serverName+':'+serverPort+'/');
  webSocket.onopen = function(event)
  {
    write("onOpen");
    abled();
  };
  webSocket.onclose = function(event)
  {
    write("onClose");
    webSocket = undefined;
    abled();
  };
  webSocket.onmessage = function(event)
  {
    write('onMessage: '+event.data);
  };
  webSocket.onerror = function(event)
  {
    write('onError: '+event.type);
  };
}

function disconnect()
{
  if(webSocket === undefined)
    write('ERROR: not connected');
  webSocket.close();
}
