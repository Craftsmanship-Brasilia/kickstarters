// Jest configuration. CommonJS (`module.exports`) because package.json has no "type": "module".
/** @type {import('jest').Config} */ // <- JSDoc type: gives autocomplete for the options below
module.exports = {
  // Compile .ts files with ts-jest before running them. We override `module` to CommonJS
  // because Jest's default runtime expects CommonJS (native ESM in Jest is still experimental).
  transform: { '^.+\\.ts$': ['ts-jest', { tsconfig: { module: 'commonjs' } }] },
  // Run tests in Node (no browser DOM needed for plain TypeScript).
  testEnvironment: 'node',
  // Folders Jest scans for tests (*.test.ts / *.spec.ts by default).
  roots: ['<rootDir>/src', '<rootDir>/tests'],
};
