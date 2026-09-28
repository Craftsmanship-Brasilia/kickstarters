# TypeScript kata kickstart (Vitest)

```bash
npm install
npm test
```

## Scripts (package.json can't hold comments, so here they are)

| Script                  | What it does                                  |
| ----------------------- | --------------------------------------------- |
| `npm test`              | run all tests once                            |
| `npm run test:watch`    | rerun tests on every save (TDD loop)          |
| `npm run test:coverage` | tests + coverage report in `coverage/`        |
| `npm run typecheck`     | `tsc --noEmit`: type errors only, no output   |
| `npm run format`        | Prettier rewrites all files                   |
| `npm run format:check`  | Prettier reports unformatted files (CI-style) |

## Examples

| File                     | Shows                                                                 |
| ------------------------ | --------------------------------------------------------------------- |
| `src/fizzbuzz/`          | functions (declaration vs arrow), named/default exports, barrel       |
| `src/tictactoe/`         | classes, `#private`, getters, discriminated unions, custom errors     |
| `src/modern/features.ts` | `satisfies`, const generics, `NoInfer`, `using`, ES2023/24 APIs       |
| `tests/modules.test.ts`  | every import form: default, named, alias, namespace, type, dynamic    |
| `tests/fizzbuzz.test.ts` | table-driven tests with `it.each`                                     |
| `tests/mocks.test.ts`    | mocks, spies and fake timers (the only file that differs Jest/Vitest) |
