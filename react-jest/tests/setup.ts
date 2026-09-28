// Runs before every UI test file (see setupFilesAfterEnv in jest.config.js).
// Adds DOM matchers to expect(): toBeInTheDocument, toBeDisabled, toHaveTextContent, ...
import '@testing-library/jest-dom';
// Note: React Testing Library unmounts rendered components after each test automatically in Jest.
