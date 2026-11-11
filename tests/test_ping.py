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


def test_ping_frame_arrives(ping_server) :
  host, port = ping_server
  _, sock = helpers.ws_handshake(host, port)
  assert helpers.recv_frame(sock, timeout=5) == (0x9, b'')
  sock.close()
#$


def test_ping_frame_arrives_without_callback(ping_nocb_server) :
  host, port = ping_nocb_server
  _, sock = helpers.ws_handshake(host, port)
  assert helpers.recv_frame(sock, timeout=5) == (0x9, b'')
  sock.close()
#$


def test_pong_answers_keep_client_alive(ping_server) :
  host, port = ping_server
  _, sock = helpers.ws_handshake(host, port)
  for _ in range(6) :
    opcode, payload = helpers.recv_frame(sock, timeout=5)
    assert opcode == 0x9
    helpers.send_frame(sock, 0xA, payload)
    time.sleep(0.2)
  #$
  # 400ms of silence would have killed the client by now,
  # each answered ping restarted its clock instead
  assert_alive(sock)
  # no more answers from here, the timeout must fire
  assert_closed(sock)
#$


def test_manual_ping_sends_on_demand(ping_manual_server) :
  host, port = ping_manual_server
  _, sock = helpers.ws_handshake(host, port)
  # interval is 0, so no periodic ping may arrive on its own
  try :
    helpers.recv_frame(sock, timeout=0.6)
    assert False, 'no ping expected while interval is 0'
  except socket.timeout :
    pass
  #$
  # the server answers our text with a manual ping; its
  # on_send_ping_frame would disconnect us if it ever fired
  helpers.send_frame(sock, 0x1, b'ping me')
  assert helpers.recv_frame(sock, timeout=5) == (0x9, b'')
  assert_alive(sock)
  sock.close()
#$
