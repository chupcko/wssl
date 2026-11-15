import os
import socket
import subprocess

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def test_logging_to_stderr_and_syslog() :
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  sock.bind(('127.0.0.1', 6012))
  sock.settimeout(5)
  try :
    proc = subprocess.run(
      [os.path.join(REPO_ROOT, 'tests', 'logging_sender.e'), '127.0.0.1:6012'],
      capture_output=True, text=True, timeout=30)
    assert proc.returncode == 0
    assert '[ERROR] where1: hello error' in proc.stderr
    assert '[DEBUG] where1: hello debug' in proc.stderr
    assert '[DEBUG] where1: only on stderr' in proc.stderr

    first, _ = sock.recvfrom(4096)
    second, _ = sock.recvfrom(4096)
    first = first.decode()
    second = second.decode()
    assert first.startswith('<27>1 ')
    assert ' logtest ' in first
    assert '[ERROR] where1: hello error' in first
    assert second.startswith('<31>1 ')
    assert ' logtest ' in second
    assert '[DEBUG] where1: hello debug' in second

    sock.settimeout(0.5)
    try :
      extra, _ = sock.recvfrom(4096)
      assert False, 'filtered message leaked to syslog: %r' % extra
    except socket.timeout :
      pass
  finally :
    sock.close()
  #$
#$
