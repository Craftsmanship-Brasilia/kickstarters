/** @type {import('jest').Config} */
const shared = {
  transform: { '^.+\\.tsx?$': ['ts-jest', { tsconfig: { jsx: 'react-jsx', module: 'commonjs' } }] },
  moduleNameMapper: { '\\.(css|scss)$': '<rootDir>/tests/style-stub.cjs' },
};

export default {
  projects: [
    {
      ...shared,
      displayName: 'unit',
      testEnvironment: 'node',
      testMatch: ['<rootDir>/tests/unit/**/*.test.ts'],
    },
    {
      ...shared,
      displayName: 'ui',
      testEnvironment: 'jsdom',
      testMatch: ['<rootDir>/tests/ui/**/*.test.tsx'],
      setupFilesAfterEnv: ['<rootDir>/tests/setup.ts'],
    },
  ],
};
