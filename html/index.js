import { ConfigurationClass } from './configuration.js';

document.getElementById('version').textContent = (new ConfigurationClass()).version();
