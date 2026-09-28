// Jest configuration. ESM syntax (`export default`) because package.json has "type": "module".
/** @type {import('jest').Config} */ // <- JSDoc type: gives autocomplete for the options below
const shared = {
  // Compile .ts/.tsx with ts-jest: JSX via the automatic runtime, output CommonJS for Jest.
  transform: { '^.+\\.tsx?$': ['ts-jest', { tsconfig: { jsx: 'react-jsx', module: 'commonjs' } }] },
  // Jest can't load CSS: any `import './x.css'` gets an empty object instead.
  moduleNameMapper: { '\\.(css|scss)$': '<rootDir>/tests/style-stub.cjs' },
};

export default {
  // Two "projects" in one run, each with its own environment.
  // `npm run test:unit` / `npm run test:ui` select one with --selectProjects.
  projects: [
    {
      ...shared,
      displayName: 'unit',
      // Pure logic: plain Node, fast, no DOM.
      testEnvironment: 'node',
      testMatch: ['<rootDir>/tests/unit/**/*.test.ts'],
    },
    {
      ...shared,
      displayName: 'ui',
      // Components: jsdom simulates a browser DOM (document, window, events) inside Node.
      testEnvironment: 'jsdom',
      testMatch: ['<rootDir>/tests/ui/**/*.test.tsx'],
      // Runs before each UI test file (adds the jest-dom matchers).
      setupFilesAfterEnv: ['<rootDir>/tests/setup.ts'],
    },
  ],
};
