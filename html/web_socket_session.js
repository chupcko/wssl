export class WebSocketSessionClass {
  constructor(logNoticeCallback, logErrorCallback, changedCallback) {
    this.logNoticeCallback = logNoticeCallback;
    this.logErrorCallback = logErrorCallback;
    this.changedCallback = changedCallback;
    this.webSocket = undefined;
  }

  connected() {
    return this.webSocket !== undefined;
  }

  connect(url) {
    if(this.webSocket !== undefined) {
      this.logErrorCallback(' ', 'already connected', '');
      return;
    }

    this.logNoticeCallback('>', 'connecting', `${ url }`);
    this.webSocket = new WebSocket(url);
    this.webSocket.onopen = (event) => {
      this.logNoticeCallback('<', 'open', '');
      this.changedCallback();
    };
    this.webSocket.onmessage = (event) => {
      this.logNoticeCallback('<', 'message', event.data);
    };
    this.webSocket.onerror = (event) => {
      this.logErrorCallback('<', 'error', '');
    };
    this.webSocket.onclose = (event) => {
      if(event.wasClean === false) {
        this.logErrorCallback('<', 'close', `code=${ event.code } reason=${ event.reason } clean=${ event.wasClean }`);
      } else {
        this.logNoticeCallback('<', 'close', `code=${ event.code } reason=${ event.reason } clean=${ event.wasClean }`);
      }
      this.webSocket = undefined;
      this.changedCallback();
    };
  }

  disconnect() {
    if(this.webSocket === undefined) {
      this.logErrorCallback(' ', 'not connected', '');
      return;
    }

    this.logNoticeCallback('>', 'disconnecting', '');
    this.webSocket.close();
  }

  send(message) {
    if(this.webSocket === undefined) {
      this.logErrorCallback(' ', 'not connected', '');
      return;
    }

    this.logNoticeCallback('>', 'send', message);
    this.webSocket.send(message);
  }

  destroy() {
    if(this.webSocket === undefined) {
      return;
    }

    let oldWebSocket = this.webSocket;
    this.webSocket = undefined;
    oldWebSocket.onopen = (event) => { };
    oldWebSocket.onmessage = (event) => { };
    oldWebSocket.onerror = (event) => { };
    oldWebSocket.onclose = (event) => { };
    oldWebSocket.close();
  }
}
