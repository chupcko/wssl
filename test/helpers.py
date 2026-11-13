import socket
import struct

HANDSHAKE_KEY = 'dGhlIHNhbXBsZSBub25jZQ=='


def ws_handshake(host, port, key=HANDSHAKE_KEY, version='13',
                 upgrade='websocket', connection='Upgrade', timeout=5) :
  sock = socket.create_connection((host, port), timeout=timeout)
  request = (
    'GET / HTTP/1.1\r\n'
    'Host: %s\r\n'
    'Upgrade: %s\r\n'
    'Connection: %s\r\n' % (host, upgrade, connection)
  )
  if key is not None :
    request += 'Sec-WebSocket-Key: %s\r\n' % key
  #$
  if version is not None :
    request += 'Sec-WebSocket-Version: %s\r\n' % version
  #$
  request += '\r\n'
  sock.sendall(request.encode())
  response = b''
  while b'\r\n\r\n' not in response :
    chunk = sock.recv(4096)
    if not chunk :
      return None, sock
    #$
    response += chunk
  #$
  return response, sock
#$


def status_line(response) :
  return response.split(b'\r\n', 1)[0]
#$


def send_frame(sock, opcode, payload=b'', masked=True, rsv=0) :
  first = 0x80 | (rsv << 4) | opcode
  mask_bit = 0x80 if masked else 0
  header = bytes([first])
  length = len(payload)
  if length <= 125 :
    header += bytes([mask_bit | length])
  elif length <= 0xFFFF :
    header += bytes([mask_bit | 126]) + struct.pack('!H', length)
  else :
    header += bytes([mask_bit | 127]) + struct.pack('!Q', length)
  #$
  if masked :
    mask = b'\x01\x02\x03\x04'
    body = bytes(b ^ mask[i % 4] for i, b in enumerate(payload))
    sock.sendall(header + mask + body)
  else :
    sock.sendall(header + payload)
  #$
#$


def recv_frame(sock, timeout=5) :
  sock.settimeout(timeout)
  header = sock.recv(2)
  if len(header) < 2 :
    return None
  #$
  assert header[0] & 0x80, 'server must set FIN'
  assert not header[1] & 0x80, 'server must not mask'
  length = header[1] & 0x7F
  if length == 126 :
    length = struct.unpack('!H', _recvall(sock, 2))[0]
  elif length == 127 :
    length = struct.unpack('!Q', _recvall(sock, 8))[0]
  #$
  return header[0] & 0x0F, _recvall(sock, length)
#$


def _recvall(sock, count) :
  data = b''
  while len(data) < count :
    chunk = sock.recv(count - len(data))
    if not chunk :
      raise ConnectionError('connection closed mid-frame')
    #$
    data += chunk
  #$
  return data
#$
