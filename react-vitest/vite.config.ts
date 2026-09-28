// One file configures the dev server, the build AND the tests (Vitest reuses Vite's pipeline,
// so tests compile exactly like the app does).
import { defineConfig } from 'vitest/config'; // like vite's defineConfig, plus types for `test`
import react from '@vitejs/plugin-react'; // JSX transform + React Fast Refresh (hot reload keeping state)

export default defineConfig({
  plugins: [react()],
  test: {
    // Expose describe/it/expect/vi as globals (Jest style) so tests don't need to import them.
    globals: true,
    // Two projects in one run, each with its own environment.
    // `npm run test:unit` / `npm run test:ui` select one with --project.
    projects: [
      {
        extends: true, // inherit plugins and the options above
        test: {
          name: 'unit',
          // Pure logic: plain Node, fast, no DOM.
          environment: 'node',
          include: ['tests/unit/**/*.test.ts'],
        },
      },
      {
        extends: true,
        test: {
          name: 'ui',
          // Components: jsdom simulates a browser DOM (document, window, events) inside Node.
          environment: 'jsdom',
          include: ['tests/ui/**/*.test.tsx'],
          // Runs before each UI test file (adds the jest-dom matchers).
          setupFiles: ['tests/setup.ts'],
        },
      },
    ],
  },
});
