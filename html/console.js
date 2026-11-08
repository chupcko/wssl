let servicePorts = {
  echo: {ws: 5001, wss: 5011},
  time: {ws: 5002, wss: 5012},
  chat: {ws: 5003, wss: 5013}
};

let serviceBlurb = {
  echo: 'Returns every message back to the sender.',
  time: 'Broadcasts the server time, twice a second.',
  chat: 'Talk to everyone connected to the server.'
};

let service = undefined;
let serverScheme = undefined;
let serverName = undefined;
let serverPort = undefined;

let nicknameId = undefined;
let connectId = undefined;
let connectPlainId = undefined;
let disconnectId = undefined;
let messageId = undefined;
let sendId = undefined;
let outputId = undefined;
let webSocket = undefined;

function init()
{
  nicknameId = document.getElementById('nickname');
  connectId = document.getElementById('connect');
  connectPlainId = document.getElementById('connectPlain');
  disconnectId = document.getElementById('disconnect');
  messageId = document.getElementById('message');
  sendId = document.getElementById('send');
  outputId = document.getElementById('output');
  clean();
  choose();
}

function picked(name)
{
  let radios = document.getElementsByName(name);
  for(let i = 0; i < radios.length; i++)
  {
    if(radios[i].checked)
    {
      return radios[i].value;
    }
  }
  return '';
}

function choose()
{
  if(webSocket !== undefined)
  {
    write('switching, closing old connection', '');
    let oldSocket = webSocket;
    webSocket = undefined;
    oldSocket.onopen = function(event)
    {
    };
    oldSocket.onmessage = function(event)
    {
    };
    oldSocket.onerror = function(event)
    {
    };
    oldSocket.onclose = function(event)
    {
    };
    oldSocket.close();
  }
  service = picked('service');
  let family = picked('family');
  let security = picked('security');
  let secure = false;
  if(security === 'wss')
  {
    secure = true;
  }
  serverScheme = 'ws://';
  if(secure)
  {
    serverScheme = 'wss://';
  }
  serverPort = servicePorts[service][security];
  serverName = serverHost(family, secure);
  document.getElementById('choice').textContent = serviceBlurb[service];
  document.getElementById('server').textContent = serverScheme + serverName + ':' + serverPort;
  let warnBox = document.getElementById('warn');
  if((location.protocol === 'https:') && (security === 'ws'))
  {
    let httpLink = document.getElementById('httplink');
    httpLink.href = 'http://' + location.host + location.pathname;
    httpLink.textContent = 'http://' + location.host + location.pathname;
    warnBox.style.display = '';
  }
  else
  {
    warnBox.style.display = 'none';
  }
  let nickBox = document.getElementById('nickbox');
  if(service === 'chat')
  {
    nickBox.style.display = '';
    connectPlainId.style.display = 'none';
  }
  else
  {
    nickBox.style.display = 'none';
    connectPlainId.style.display = '';
  }
  let msgBox = document.getElementById('msgbox');
  if(service === 'time')
  {
    msgBox.style.display = 'none';
  }
  else
  {
    msgBox.style.display = '';
  }
  clean();
  abled();
}

function abled()
{
  if(webSocket === undefined)
  {
    nicknameId.disabled = false;
    connectId.disabled = false;
    connectPlainId.disabled = false;
    disconnectId.disabled = true;
    messageId.disabled = true;
    sendId.disabled = true;
  }
  else
  {
    nicknameId.disabled = true;
    connectId.disabled = true;
    connectPlainId.disabled = true;
    disconnectId.disabled = false;
    messageId.disabled = false;
    sendId.disabled = false;
  }
}

function clean()
{
  outputId.innerHTML = '';
}

function two(number)
{
  if(number < 10)
  {
    return '0' + number;
  }
  return '' + number;
}

function stamp()
{
  let now = new Date();
  return two(now.getHours()) + ':' + two(now.getMinutes()) + ':' + two(now.getSeconds());
}

function write(label, message)
{
  let line = document.createElement('p');
  line.className = 'line';
  let stampSpan = document.createElement('span');
  stampSpan.className = 'stamp';
  stampSpan.textContent = '[' + stamp() + '] ';
  let labelSpan = document.createElement('span');
  labelSpan.className = 'system';
  labelSpan.textContent = label;
  line.appendChild(stampSpan);
  line.appendChild(labelSpan);
  if(message !== '')
  {
    let bodySpan = document.createElement('span');
    bodySpan.className = 'data';
    bodySpan.textContent = ' ' + message;
    line.appendChild(bodySpan);
  }
  outputId.insertBefore(line, outputId.firstChild);
}

function connectKey(event)
{
  if(event.key === 'Enter')
  {
    connect();
  }
}

function sendKey(event)
{
  if(event.key === 'Enter')
  {
    send();
  }
}

function connect()
{
  if(webSocket !== undefined)
  {
    write('already connected', '');
    return;
  }
  let path = '/';
  if(service === 'chat')
  {
    if(nicknameId.value === '')
    {
      write('nickname required', '');
      return;
    }
    path = '/' + nicknameId.value;
  }
  let url = serverScheme + serverName + ':' + serverPort + path;
  write('connecting ' + url, '');
  webSocket = new WebSocket(url);
  webSocket.onopen = function(event)
  {
    write('open', '');
    abled();
  };
  webSocket.onmessage = function(event)
  {
    write('message', event.data);
  };
  webSocket.onerror = function(event)
  {
    write('error: ' + event.type, '');
  };
  webSocket.onclose = function(event)
  {
    write('close: code=' + event.code + ' reason=' + event.reason + ' clean=' + event.wasClean, '');
    webSocket = undefined;
    abled();
  };
}

function disconnect()
{
  if(webSocket === undefined)
  {
    write('not connected', '');
    return;
  }
  write('disconnecting', '');
  webSocket.close();
}

function send()
{
  if(webSocket === undefined)
  {
    write('not connected', '');
    return;
  }
  if(messageId.value === '')
  {
    return;
  }
  write('send', messageId.value);
  webSocket.send(messageId.value);
  messageId.value = '';
}
