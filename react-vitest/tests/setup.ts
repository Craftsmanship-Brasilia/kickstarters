// Runs before every UI test file (see `setupFiles` in vite.config.ts).
// Adds DOM matchers to expect(): toBeInTheDocument, toBeDisabled, toHaveTextContent, ...
// The `/vitest` entry point registers them on Vitest's expect instead of Jest's.
import '@testing-library/jest-dom/vitest';
// Note: with `globals: true`, React Testing Library unmounts components after each test automatically.
