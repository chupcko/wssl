import socket
import time

import helpers


def assert_alive(sock) :
  sock.settimeout(0.3)
  try :
    data = sock.recv(100)
  except socket.timeout :
    return
  assert data != b'', 'expected an open connection'
  #$
  # any data (for example a pong) also proves the connection is alive
#$


def assert_closed(sock) :
  sock.settimeout(3)
  assert sock.recv(100) == b''
  sock.close()
#$


def test_timeout_ignores_twice_then_disconnects(timeout_server) :
  host, port = timeout_server
  _, sock = helpers.ws_handshake(host, port)
  time.sleep(0.6)
  assert_alive(sock)
  assert_closed(sock)
#$


def test_timeout_activity_resets_clock(timeout_server) :
  host, port = timeout_server
  _, sock = helpers.ws_handshake(host, port)
  for _ in range(6) :
    helpers.send_frame(sock, 0x9, b'x')
    assert helpers.recv_frame(sock) == (0xA, b'x')
    time.sleep(0.2)
  #$
  assert_closed(sock)
#$


def test_no_callback_kills_idle_client(timeout_nocb_server) :
  host, port = timeout_nocb_server
  _, sock = helpers.ws_handshake(host, port)
  assert_closed(sock)
#$
