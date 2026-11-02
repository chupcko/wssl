import socket
import struct

import helpers


def test_handshake_returns_101(echo_server) :
  host, port = echo_server
  response, sock = helpers.ws_handshake(host, port)
  assert response is not None
  assert b'101' in helpers.status_line(response)
  sock.close()
#$


def test_echo_small_and_medium(echo_server) :
  host, port = echo_server
  for payload in (b'hello', b'x' * 200) :
    _, sock = helpers.ws_handshake(host, port)
    helpers.send_frame(sock, 0x1, payload)
    opcode, echoed = helpers.recv_frame(sock)
    assert opcode == 0x1
    assert echoed == payload
    sock.close()
  #$
#$


def test_burst_handshakes(echo_server) :
  host, port = echo_server
  sockets = []
  try :
    for _ in range(10) :
      response, sock = helpers.ws_handshake(host, port)
      assert b'101' in helpers.status_line(response)
      sockets.append(sock)
    #$
  finally :
    for sock in sockets :
      sock.close()
    #$
  #$
#$


def test_abort_without_handshake_keeps_server_alive(echo_server) :
  host, port = echo_server
  for _ in range(3) :
    socket.create_connection((host, port), timeout=5).close()
  #$
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'alive')
  assert helpers.recv_frame(sock) == (0x1, b'alive')
  sock.close()
#$


def test_unknown_opcode_disconnects(echo_server) :
  host, port = echo_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x3, b'abc')
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_unmasked_frame_disconnects(echo_server) :
  host, port = echo_server
  _, sock = helpers.ws_handshake(host, port)
  sock.sendall(b'\x81\x05hello')
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_rsv_bit_disconnects(echo_server) :
  host, port = echo_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'abc', rsv=0x4)
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_huge_frame_declared_length_disconnects_and_server_survives(echo_server) :
  host, port = echo_server
  _, sock = helpers.ws_handshake(host, port)
  sock.sendall(b'\x81\xff' + struct.pack('!Q', 0xFFFFFFFFFFFFFFFF) + b'\x01\x02\x03\x04')
  sock.settimeout(2)
  assert sock.recv(100) == b''
  sock.close()
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x1, b'alive')
  assert helpers.recv_frame(sock) == (0x1, b'alive')
  sock.close()
#$


def test_too_many_header_fields_rejected(echo_server) :
  host, port = echo_server
  sock = socket.create_connection((host, port), timeout=5)
  request = (
    'GET / HTTP/1.1\r\nHost: x\r\nUpgrade: websocket\r\n'
    'Connection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n'
    'Sec-WebSocket-Version: 13\r\n'
  )
  for i in range(70) :
    request += 'X-Pad-%d: abcdefgh\r\n' % i
  #$
  sock.sendall((request + '\r\n').encode())
  sock.settimeout(2)
  assert sock.recv(4096) == b''
  sock.close()
#$


def test_bad_key_and_version_rejected(echo_server) :
  host, port = echo_server
  for key, version in (('abc', '13'), (None, '13'), (helpers.HANDSHAKE_KEY, '8'),
                       (helpers.HANDSHAKE_KEY, None), ('!!!!!!!!!!!!!!!!!!!!!!!!', '13'),
                       ('dGhlIHNhbXBsZSBub25jZQ=?', '13'),
                       ('dGhlIHNhbXBsZSBub25jZQAB', '13')) :
    response, sock = helpers.ws_handshake(host, port, key=key, version=version)
    assert response is not None
    assert b'400' in helpers.status_line(response)
    sock.close()
  #$
#$


def test_lowercase_upgrade_headers_accepted(echo_server) :
  host, port = echo_server
  response, sock = helpers.ws_handshake(
    host, port, upgrade='WebSocket', connection='keep-alive, upgrade')
  assert response is not None
  assert b'101' in helpers.status_line(response)
  sock.close()
#$


def test_close_frame_gets_reply(echo_server) :
  host, port = echo_server
  _, sock = helpers.ws_handshake(host, port)
  helpers.send_frame(sock, 0x8, b'')
  assert helpers.recv_frame(sock) == (0x8, b'')
  sock.close()
#$
