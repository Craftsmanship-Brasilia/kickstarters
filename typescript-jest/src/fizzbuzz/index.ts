// "Barrel" file: re-exports the public API of the folder, so callers can write
//   import { fizzBuzz } from './fizzbuzz'     (resolves to ./fizzbuzz/index.ts)
// instead of reaching into internal files.
export { fizzBuzz, sequence } from './fizzbuzz';
export { RULES as rules, wordsFor } from './rules'; // re-export with a different name
export type { Rule } from './rules'; // re-export a type only
