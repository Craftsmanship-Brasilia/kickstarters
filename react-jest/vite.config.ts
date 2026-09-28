// Vite configuration: dev server (`npm run dev`) and production build (`npm run build`).
// Tests do NOT use this file here — Jest has its own pipeline (jest.config.js + ts-jest).
import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react'; // JSX transform + React Fast Refresh (hot reload keeping state)

export default defineConfig({
  plugins: [react()],
});
