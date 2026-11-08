let configuration = {
  ipv4: {
    ws:  '',
    wss: ''
  },
  ipv6: {
    ws:  '',
    wss: ''
  }
};

function serverHost(family, secure)
{
  let host;
  if(secure)
  {
    host = configuration[family]['wss'];
  }
  else
  {
    host = configuration[family]['ws'];
  }
  if(host === '')
  {
    host = location.hostname;
  }
  return host;
}
