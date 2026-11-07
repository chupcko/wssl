import os
import socket
import ssl
import subprocess
import time

import pytest

import helpers

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

PLAIN_IPV4_PORT = 5031
PLAIN_IPV6_PORT = 5032
SSL_IPV4_PORT = 5131
SSL_IPV6_PORT = 5132


def wait_for_port(host, port, timeout=15) :
  deadline = time.time() + timeout
  while time.time() < deadline :
    try :
      socket.create_connection((host, port), timeout=1).close()
      return
    except OSError :
      time.sleep(0.1)
    #$
  #$
  raise RuntimeError('server did not listen on %s:%d' % (host, port))
#$


def stop_server(proc) :
  proc.terminate()
  try :
    proc.wait(timeout=10)
  except subprocess.TimeoutExpired :
    proc.kill()
    proc.wait(timeout=10)
  #$
#$


def tls_handshake(host, port) :
  context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
  context.check_hostname = False
  context.verify_mode = ssl.CERT_NONE
  raw = socket.create_connection((host, port), timeout=5)
  sock = context.wrap_socket(raw, server_hostname=host)
  sock.sendall(
    b'GET / HTTP/1.1\r\nHost: x\r\nUpgrade: websocket\r\n'
    b'Connection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n'
    b'Sec-WebSocket-Version: 13\r\n\r\n'
  )
  response = b''
  while b'\r\n\r\n' not in response :
    response += sock.recv(4096)
  #$
  assert b'101' in response.split(b'\r\n', 1)[0]
  return sock
#$


@pytest.fixture(scope='module')
def echo_four_servers(tmp_path_factory) :
  cert_dir = tmp_path_factory.mktemp('cert')
  subprocess.run(
    [os.path.join(REPO_ROOT, 'tools', 'make_ssl_certificate'), str(cert_dir)],
    check=True,
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  config = tmp_path_factory.mktemp('config') / 'servers.conf'
  config.write_text(
    'ipv4 0.0.0.0 %d\n'
    'ipv6 :: %d\n'
    'ipv4 0.0.0.0 %d ssl %s %s\n'
    'ipv6 :: %d ssl %s %s\n' % (
      PLAIN_IPV4_PORT, PLAIN_IPV6_PORT,
      SSL_IPV4_PORT, cert_dir / 'server.crt', cert_dir / 'server.key',
      SSL_IPV6_PORT, cert_dir / 'server.crt', cert_dir / 'server.key',
    )
  )
  proc = subprocess.Popen(
    [os.path.join(REPO_ROOT, 'examples', 'wssl_echo.e'), str(config)],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
  )
  try :
    wait_for_port('127.0.0.1', PLAIN_IPV4_PORT)
    wait_for_port('::1', PLAIN_IPV6_PORT)
    wait_for_port('127.0.0.1', SSL_IPV4_PORT)
    wait_for_port('::1', SSL_IPV6_PORT)
    yield
  finally :
    stop_server(proc)
  #$
#$


def test_echo_on_all_four_configured_servers(echo_four_servers) :
  for host, port in (('127.0.0.1', PLAIN_IPV4_PORT),
                     ('::1', PLAIN_IPV6_PORT)) :
    _, sock = helpers.ws_handshake(host, port)
    helpers.send_frame(sock, 0x1, b'four')
    assert helpers.recv_frame(sock) == (0x1, b'four')
    sock.close()
  #$
  for host, port in (('127.0.0.1', SSL_IPV4_PORT),
                     ('::1', SSL_IPV6_PORT)) :
    sock = tls_handshake(host, port)
    try :
      helpers.send_frame(sock, 0x1, b'four-ssl')
      assert helpers.recv_frame(sock) == (0x1, b'four-ssl')
    finally :
      sock.close()
    #$
  #$
#$


def test_bad_stack_name_exits_nonzero(tmp_path) :
  config = tmp_path / 'servers.conf'
  config.write_text('unix /tmp/x.sock 5001\n')
  proc = subprocess.run(
    [os.path.join(REPO_ROOT, 'examples', 'wssl_echo.e'), str(config)],
    stdout=subprocess.DEVNULL,
    stderr=subprocess.DEVNULL,
    timeout=30,
  )
  assert proc.returncode != 0
#$
