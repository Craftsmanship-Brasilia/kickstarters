// Browser entry point (loaded by index.html). Tests never import this file:
// they render components directly with Testing Library.
import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { App } from './App';

// `!` = non-null assertion: we know index.html contains <div id="root">.
createRoot(document.getElementById('root')!).render(
  // StrictMode: dev-only checks (double-invokes renders/effects to surface side-effect bugs).
  <StrictMode>
    <App />
  </StrictMode>,
);
