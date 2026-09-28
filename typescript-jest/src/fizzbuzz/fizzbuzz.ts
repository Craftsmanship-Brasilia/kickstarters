// Named import from a sibling module. With "moduleResolution": "bundler" no file extension is needed.
import { wordsFor } from './rules';

// Function declaration: hoisted, has its own `this`. Good for top-level functions.
export function fizzBuzz(n: number): string {
  if (!Number.isInteger(n) || n <= 0) {
    throw new RangeError(`Expected a positive integer, got ${n}`);
  }
  return wordsFor(n) || String(n);
}

// Arrow function in a const: not hoisted, inherits `this`. Common for small helpers.
export const sequence = (count: number): string[] =>
  Array.from({ length: count }, (_, index) => fizzBuzz(index + 1));

// Default export: the importer picks the name (`import whatever from './fizzbuzz'`).
// Tip: many teams prefer named exports only, because renames/auto-imports are safer.
export default fizzBuzz;
