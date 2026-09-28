# React + TypeScript kata kickstart (Vitest + Testing Library)

```bash
npm install
npm test
```

## Scripts (package.json can't hold comments, so here they are)

| Script                  | What it does                                            |
| ----------------------- | ------------------------------------------------------- |
| `npm run dev`           | Vite dev server with hot reload                         |
| `npm run build`         | type-check, then production bundle in `dist/`           |
| `npm test`              | all tests (unit + ui)                                   |
| `npm run test:unit`     | `tests/unit/**/*.test.ts`: pure logic, Node environment |
| `npm run test:ui`       | `tests/ui/**/*.test.tsx`: components in jsdom           |
| `npm run test:watch`    | rerun tests on every save (TDD loop)                    |
| `npm run test:coverage` | tests + coverage report in `coverage/`                  |
| `npm run typecheck`     | `tsc --noEmit`                                          |
| `npm run format`        | Prettier rewrites all files                             |

## Examples

| File                             | Shows                                                                   |
| -------------------------------- | ----------------------------------------------------------------------- |
| `src/tictactoe/logic.ts`         | pure rules, unit-tested in `tests/unit/`                                |
| `src/tictactoe/TicTacToe.tsx`    | `useState`, derived state, immutable updates, accessible buttons        |
| `src/fizzbuzz/FizzBuzzForm.tsx`  | controlled input, form submit, optional callback prop                   |
| `tests/ui/TicTacToe.test.tsx`    | `render`, `screen.getByRole`, `within`, `userEvent.click`               |
| `tests/ui/FizzBuzzForm.test.tsx` | `userEvent.type`, `{Enter}`, `getByLabelText`, mock callback, `queryBy` |

### Query priority (Testing Library)

1. `getByRole(role, { name })` — what users and screen readers see; prefer it
2. `getByLabelText` — form fields
3. `getByText` — non-interactive content
4. `getByTestId` — last resort

`getBy*` throws if missing, `queryBy*` returns `null` (assert absence), `findBy*` waits (async UI).
