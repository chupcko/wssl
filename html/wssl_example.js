import { ConfigurationClass } from './configuration.js';
import { WebSocketSessionClass } from './web_socket_session.js';

export class WsslExampleClass extends HTMLElement {
  constructor() {
    super();
    this.configuration = new ConfigurationClass();
    this.service = undefined;
    this.url = undefined;
    this.session = undefined;
    this.serviceRadiosId = undefined;
    this.familyRadiosId = undefined;
    this.securityRadiosId = undefined;
    this.blurbId = undefined;
    this.serverId = undefined;
    this.warnLineId = undefined;
    this.httpLinkId = undefined;
    this.nicknameBoxId = undefined;
    this.nicknameId = undefined;
    this.connectId = undefined;
    this.connectPlainId = undefined;
    this.messageId = undefined;
    this.sendId = undefined;
    this.messageBoxId = undefined;
    this.disconnectId = undefined;
    this.outputId = undefined;
  }

  buildRadio(parentId, name, value, text, checked) {
    let pickId = document.createElement('label');
    pickId.className = 'pick';

    let radioId = document.createElement('input');
    radioId.type = 'radio';
    radioId.name = name;
    radioId.value = value;
    radioId.checked = checked;
    radioId.addEventListener(
      'change',
      () => {
        this.choose();
      }
    );
    pickId.appendChild(radioId);

    pickId.appendChild(document.createTextNode(text));

    parentId.appendChild(pickId);
    return radioId;
  }

  buildRadioGroup(parentId, label, name, options) {
    let groupId = document.createElement('span');
    groupId.className = 'group';
    groupId.textContent = label;

    let radioIds = [];
    for(let option of options) {
      radioIds.push(this.buildRadio(groupId, name, option[0], option[1], option[2]));
    }

    parentId.appendChild(groupId);
    return radioIds;
  }

  buildButton(parentId, text, action) {
    let buttonId = document.createElement('button');
    buttonId.className = 'button';
    buttonId.textContent = text;
    buttonId.addEventListener('click', action);

    parentId.appendChild(buttonId);
    return buttonId;
  }

  buildInput(parentId, placeholder, action) {
    let fieldId = document.createElement('input');
    fieldId.className = 'input';
    fieldId.type = 'text';
    fieldId.placeholder = placeholder;
    fieldId.addEventListener(
      'keydown',
      (event) => {
        if(event.key === 'Enter') {
          action();
        }
      }
    );

    parentId.appendChild(fieldId);
    return fieldId;
  }

  connectedCallback() {
    let subtitle = this.getAttribute('subtitle');
    if(subtitle === null) {
      subtitle = '';
    }

    let background = this.getAttribute('background');
    if(background === null) {
      background = 'transparent';
    }

    let frameId = document.createElement('div');
    frameId.className = 'frame';
    frameId.style.background = background;

    let titleId = document.createElement('p');
    titleId.className = 'subtitle';
    titleId.textContent = subtitle;
    frameId.appendChild(titleId);

    let formId = document.createElement('form');

    this.serviceRadiosId = this.buildRadioGroup(
      formId,
      'Service: ',
      'service',
      [
        [ 'echo', 'Echo', true ],
        [ 'time', 'Time', false ],
        [ 'chat', 'Chat', false ]
      ]
    );
    this.familyRadiosId = this.buildRadioGroup(
      formId,
      'Family: ',
      'family',
      [
        [ 'ipv4', 'IPv4', true ],
        [ 'ipv6', 'IPv6', false ]
      ]
    );
    this.securityRadiosId = this.buildRadioGroup(
      formId,
      'Security: ',
      'security',
      [
        [ 'wss', 'wss', true ],
        [ 'ws', 'ws', false ]
      ]
    );

    frameId.appendChild(formId);

    let infoId = document.createElement('p');

    this.blurbId = document.createElement('span');
    infoId.appendChild(this.blurbId);

    infoId.appendChild(document.createTextNode(' '));

    this.serverId = document.createElement('code');
    this.serverId.className = 'mono';
    infoId.appendChild(this.serverId);

    frameId.appendChild(infoId);

    this.warnLineId = document.createElement('p');
    this.warnLineId.className = 'warn';
    this.warnLineId.style.display = 'none';
    this.warnLineId.textContent = 'Warning: ws is blocked from an https page and will not work. Open the http page instead: ';

    this.httpLinkId = document.createElement('a');
    this.httpLinkId.href = '';
    this.warnLineId.appendChild(this.httpLinkId);

    frameId.appendChild(this.warnLineId);

    let buttonsId = document.createElement('div');

    this.buildButton(
      buttonsId,
      'Clean',
      () => {
        this.clean();
      }
    );

    this.nicknameBoxId = document.createElement('span');
    this.nicknameBoxId.className = 'group';
    buttonsId.appendChild(this.nicknameBoxId);

    this.nicknameId = this.buildInput(
      this.nicknameBoxId,
      'Nickname',
      () => {
        this.connect();
      }
    );
    this.connectId = this.buildButton(
      this.nicknameBoxId,
      'Connect',
      () => {
        this.connect();
      }
    );

    this.connectPlainId = this.buildButton(
      buttonsId,
      'Connect',
      () => {
        this.connect();
      }
    );

    this.messageBoxId = document.createElement('span');
    this.messageBoxId.className = 'group';
    buttonsId.appendChild(this.messageBoxId);

    this.messageId = this.buildInput(
      this.messageBoxId,
      'Message',
      () => {
        this.send();
      }
    );
    this.sendId = this.buildButton(
      this.messageBoxId,
      'Send',
      () => {
        this.send();
      }
    );

    this.disconnectId = this.buildButton(
      buttonsId,
      'Disconnect',
      () => {
        this.disconnect();
      }
    );

    frameId.appendChild(buttonsId);

    this.outputId = document.createElement('div');
    this.outputId.className = 'log';
    frameId.appendChild(this.outputId);

    this.appendChild(frameId);

    this.session = new WebSocketSessionClass(
      (direction, label, message) => {
        this.logNotice(direction, label, message);
      },
      (direction, label, message) => {
        this.logError(direction, label, message);
      },
      () => {
        this.abled();
      }
    );

    this.clean();
    this.choose();
  }

  disconnectedCallback() {
    this.session.destroy();
  }

  picked(radioIds) {
    for(let radioId of radioIds) {
      if(radioId.checked) {
        return radioId.value;
      }
    }
    return '';
  }

  choose() {
    if(this.session.connected()) {
      this.logNotice(' ', 'switching, closing old connection', '');
      this.session.destroy();
      this.session = new WebSocketSessionClass(
        (direction, label, message) => {
          this.logNotice(direction, label, message);
        },
        (direction, label, message) => {
          this.logError(direction, label, message);
        },
        () => {
          this.abled();
        }
      );
    }
    this.service = this.picked(this.serviceRadiosId);
    let family = this.picked(this.familyRadiosId);
    let security = this.picked(this.securityRadiosId);
    this.url = `${ security }://${ this.configuration.serverHost(family, security) }:${ this.configuration.serverPort(this.service, security) }`;

    this.blurbId.textContent = this.configuration.serviceBlurb(this.service);
    this.serverId.textContent = this.url;

    if((location.protocol === 'https:') && (security === 'ws')) {
      this.httpLinkId.href = `http://${ location.host }${ location.pathname }`;
      this.httpLinkId.textContent = `http://${ location.host }${ location.pathname }`;
      this.warnLineId.style.display = '';
    } else {
      this.warnLineId.style.display = 'none';
    }

    switch(this.service) {
      case 'echo': {
        this.nicknameBoxId.style.display = 'none';
        this.connectPlainId.style.display = '';
        this.messageBoxId.style.display = '';
        break;
      }
      case 'time': {
        this.nicknameBoxId.style.display = 'none';
        this.connectPlainId.style.display = '';
        this.messageBoxId.style.display = 'none';
        break;
      }
      case 'chat': {
        this.nicknameBoxId.style.display = '';
        this.connectPlainId.style.display = 'none';
        this.messageBoxId.style.display = '';
        break;
      }
    }
    this.clean();
    this.abled();
  }

  abled() {
    if(this.session.connected() === false) {
      this.nicknameId.disabled = false;
      this.connectId.disabled = false;
      this.connectPlainId.disabled = false;
      this.messageId.disabled = true;
      this.sendId.disabled = true;
      this.disconnectId.disabled = true;
    } else {
      this.nicknameId.disabled = true;
      this.connectId.disabled = true;
      this.connectPlainId.disabled = true;
      this.messageId.disabled = false;
      this.sendId.disabled = false;
      this.disconnectId.disabled = false;
    }
  }

  clean() {
    this.outputId.innerHTML = '';
  }

  print(kind, direction, label, message) {
    let lineId = document.createElement('p');
    lineId.className = 'log-line';

    let headId = document.createElement('span');
    headId.className = kind;
    let now = new Date();
    headId.textContent = `[${ String(now.getHours()).padStart(2, '0') }:${ String(now.getMinutes()).padStart(2, '0') }:${ String(now.getSeconds()).padStart(2, '0') }] ${ direction } ${ label }`;
    lineId.appendChild(headId);

    if(message !== '') {
      let messageId = document.createElement('span');
      messageId.className = 'log-message';
      messageId.textContent = ` ${ message }`;
      lineId.appendChild(messageId);
    }

    this.outputId.insertBefore(lineId, this.outputId.firstChild);
  }

  logNotice(direction, label, message) {
    this.print('log-notice', direction, label, message);
  }

  logError(direction, label, message) {
    this.print('log-error', direction, label, message);
  }

  connect() {
    if(this.session.connected()) {
      this.logError(' ', 'already connected', '');
      return;
    }

    let path = '/';
    if(this.service === 'chat') {
      if(this.nicknameId.value === '') {
        this.logError(' ', 'nickname required', '');
        return;
      }
      path = `/${ this.nicknameId.value }`;
    }
    this.session.connect(`${ this.url }${ path }`);
  }

  disconnect() {
    this.session.disconnect();
  }

  send() {
    if(this.session.connected() === false) {
      this.logError(' ', 'not connected', '');
      return;
    }

    if(this.messageId.value === '') {
      return;
    }
    this.session.send(this.messageId.value);
    this.messageId.value = '';
  }
}

customElements.define('wssl-example', WsslExampleClass);
