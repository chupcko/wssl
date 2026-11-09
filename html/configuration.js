export class ConfigurationClass {
  constructor() {
    this.servers = {
      ipv4: {
        ws:  '',
        wss: ''
      },
      ipv6: {
        ws:  '',
        wss: ''
      }
    };
    this.services = {
      echo: {
        blurb: 'Returns every message back to the sender.',
        ports: {
          ws: 5001,
          wss: 5011
        }
      },
      time: {
        blurb: 'Broadcasts the server time, twice a second.',
        ports: {
          ws: 5002,
          wss: 5012
        }
      },
      chat: {
        blurb: 'Talk to everyone connected to the server.',
        ports: {
          ws: 5003,
          wss: 5013
        }
      }
    };
  }

  version() {
    return '20261109';
  }

  serviceBlurb(service) {
    return this.services[service].blurb;
  }

  serverPort(service, protocol) {
    return this.services[service].ports[protocol];
  }

  serverHost(family, security) {
    let host = this.servers[family][security];
    if(host === '') {
      return location.hostname;
    }
    return host;
  }
}
