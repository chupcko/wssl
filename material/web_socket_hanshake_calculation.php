<?php

  function web_socket_hanshake_calculation($key)
  {
    return base64_encode(sha1($key.'258EAFA5-E914-47DA-95CA-C5AB0DC85B11', true));
  }

  echo web_socket_hanshake_calculation('sbGQkJAtBId0I7Shgyk2uQ==')."\n";
  echo web_socket_hanshake_calculation('x3JJHMbDL1EzLkh9GBhXDw==')."\n";

?>
