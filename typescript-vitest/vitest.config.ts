// Vitest configuration (same format as a Vite config, `test` holds the Vitest options).
import { defineConfig } from 'vitest/config'; // defineConfig only adds type-checking/autocomplete

export default defineConfig({
  test: {
    // Expose describe/it/expect/vi as globals (Jest style) so tests don't need to import them.
    globals: true,
    // Run tests in Node (no browser DOM needed for plain TypeScript).
    environment: 'node',
    // Which files are tests.
    include: ['tests/**/*.test.ts'],
  },
});
